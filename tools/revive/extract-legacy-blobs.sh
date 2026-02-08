#!/usr/bin/env bash
set -euo pipefail

repo_root="$(git rev-parse --show-toplevel 2>/dev/null || pwd)"
cd "$repo_root"

out_root="${1:-resources_utf8}"
manifest_dir="${2:-resources_manifest}"

python3 - "$out_root" "$manifest_dir" <<'PY'
import json
import hashlib
import pathlib
import shutil
import subprocess
import sys
import unicodedata
from typing import Tuple

out_root = pathlib.Path(sys.argv[1])
manifest_dir = pathlib.Path(sys.argv[2])

if out_root.exists():
    shutil.rmtree(out_root)
if manifest_dir.exists():
    shutil.rmtree(manifest_dir)
out_root.mkdir(parents=True, exist_ok=True)
manifest_dir.mkdir(parents=True, exist_ok=True)


def run_git(*args: str) -> bytes:
    return subprocess.run(["git", *args], check=True, stdout=subprocess.PIPE).stdout


def safe_seg(seg: bytes) -> Tuple[str, bool]:
    try:
        return seg.decode("cp949"), True
    except UnicodeDecodeError:
        return f"__legacy_hex_{seg.hex().upper()}", False


def classify_blob(data: bytes) -> Tuple[str, str]:
    if b"\x00" in data:
        return "binary", "binary"
    ctrl = sum(1 for b in data if (b < 9) or (13 < b < 32))
    if data and (ctrl / len(data)) > 0.02:
        return "binary", "binary"
    try:
        txt = data.decode("utf-8")
        if all(ord(ch) < 128 for ch in txt):
            return "text", "ascii"
        return "text", "utf-8"
    except UnicodeDecodeError:
        pass
    try:
        data.decode("cp949")
        return "text", "cp949"
    except UnicodeDecodeError:
        return "binary", "binary"


raw = run_git("ls-tree", "-r", "-z", "HEAD")
entries = [e for e in raw.split(b"\x00") if e]
manifest = []
used_paths = {}
used_fs_keys = {}

for entry in entries:
    meta, path_b = entry.split(b"\t", 1)
    _, typ_b, sha_b = meta.split(b" ", 2)
    if typ_b != b"blob":
        continue

    sha = sha_b.decode("ascii")
    segs = path_b.split(b"/")
    norm_segs = []
    for s in segs:
        decoded, _ = safe_seg(s)
        norm_segs.append(decoded)

    candidate = "/".join(norm_segs)
    # macOS default filesystems are case-insensitive + Unicode-normalized.
    # Reserve path keys in that view as well to avoid silent overwrite.
    while True:
        fs_key = unicodedata.normalize("NFD", candidate).casefold()
        exact_conflict = candidate in used_paths and used_paths[candidate] != path_b
        fs_conflict = fs_key in used_fs_keys and used_fs_keys[fs_key] != path_b
        if not exact_conflict and not fs_conflict:
            break

        p = pathlib.PurePosixPath(candidate)
        stem = p.stem if p.stem else p.name
        suffix = p.suffix if p.suffix else ""
        parent = str(p.parent)
        alt_name = f"{stem}__{sha[:8]}{suffix}"
        candidate = f"{parent}/{alt_name}" if parent != "." else alt_name

    used_paths[candidate] = path_b
    used_fs_keys[unicodedata.normalize("NFD", candidate).casefold()] = path_b
    blob = run_git("cat-file", "-p", sha)

    out_path = out_root / pathlib.PurePosixPath(candidate)
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_bytes(blob)

    kind, encoding = classify_blob(blob)
    try:
        cp949_path = path_b.decode("cp949")
    except UnicodeDecodeError:
        cp949_path = ""

    manifest.append({
        "legacy_path_hex": path_b.hex().upper(),
        "legacy_path_cp949": cp949_path,
        "normalized_utf8_path": candidate,
        "blob_sha1": sha,
        "sha256": hashlib.sha256(blob).hexdigest(),
        "size": len(blob),
        "kind": kind,
        "text_encoding": encoding,
    })

manifest.sort(key=lambda x: x["normalized_utf8_path"])
(manifest_dir / "resource-map.v1.json").write_text(json.dumps(manifest, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
with (manifest_dir / "path-alias.v1.tsv").open("w", encoding="utf-8", newline="") as fp:
    fp.write("legacy_path_hex\tlegacy_path_cp949\tnormalized_utf8_path\tblob_sha1\n")
    for row in manifest:
        cp949 = row["legacy_path_cp949"].replace("\t", " ").replace("\n", " ")
        norm = row["normalized_utf8_path"].replace("\t", " ").replace("\n", " ")
        fp.write(f"{row['legacy_path_hex']}\t{cp949}\t{norm}\t{row['blob_sha1']}\n")

print(f"generated_entries={len(manifest)}")
PY
