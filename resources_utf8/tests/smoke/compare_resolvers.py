#!/usr/bin/env python3
import argparse
import json
import os
import pathlib
import subprocess
import sys


def run_probe(probe: str, hex_lines: list[str]) -> dict[str, tuple[int, str]]:
    payload = ("\n".join(hex_lines) + "\n").encode("ascii")
    p = subprocess.run([probe], input=payload, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
    out = p.stdout.decode("utf-8", errors="replace").splitlines()

    result: dict[str, tuple[int, str]] = {}
    for line in out:
        parts = line.split("\t", 2)
        if len(parts) < 3:
            continue
        key, rc_s, resolved = parts[0], parts[1], parts[2]
        try:
            rc = int(rc_s)
        except ValueError:
            rc = -999
        result[key] = (rc, resolved)
    return result


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--probe-a", required=True)
    ap.add_argument("--probe-b", required=True)
    ap.add_argument("--manifest", default="resources_manifest/resource-map.v1.json")
    ap.add_argument("--muhan-home", default=os.environ.get("MUHAN_HOME", "/work"))
    args = ap.parse_args()

    manifest = json.loads(pathlib.Path(args.manifest).read_text(encoding="utf-8"))
    hex_lines = [row["legacy_path_hex"] for row in manifest]

    a = run_probe(args.probe_a, hex_lines)
    b = run_probe(args.probe_b, hex_lines)

    mismatches = []
    missing = []

    for row in manifest:
        h = row["legacy_path_hex"]
        expected_utf8 = os.path.realpath(os.path.join(args.muhan_home, "resources_utf8", row["normalized_utf8_path"]))
        raw = bytes.fromhex(h)
        has_non_ascii = any(b >= 0x80 for b in raw)

        if h not in a or h not in b:
            missing.append(h)
            continue

        a_rc, a_path = a[h]
        b_rc, b_path = b[h]

        a_real = os.path.realpath(a_path) if a_path else ""
        b_real = os.path.realpath(b_path) if b_path else ""

        if a_rc != 0 or b_rc != 0 or a_real != b_real:
            mismatches.append((h, a_rc, a_path, b_rc, b_path, expected_utf8))
            continue

        if has_non_ascii and a_real != expected_utf8:
            mismatches.append((h, a_rc, a_path, b_rc, b_path, expected_utf8))

    if missing:
        print(f"resolver parity failed: missing probe rows={len(missing)}")
        for h in missing[:20]:
            print(f"  missing: {h}")
        return 1

    if mismatches:
        print(f"resolver parity failed: mismatches={len(mismatches)}")
        for h, a_rc, a_path, b_rc, b_path, expected in mismatches[:20]:
            print(f"  {h}")
            print(f"    c:    rc={a_rc} path={a_path}")
            print(f"    rust: rc={b_rc} path={b_path}")
            print(f"    exp:  {expected}")
        return 1

    print(f"resolver parity ok: {len(manifest)} paths")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
