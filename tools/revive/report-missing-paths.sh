#!/usr/bin/env bash
set -euo pipefail

repo_root="$(git rev-parse --show-toplevel 2>/dev/null || pwd)"
cd "$repo_root"

python3 - <<'PY'
import os
import subprocess
from collections import Counter

def hex_bytes(b: bytes) -> str:
    out = []
    for x in b:
        if 32 <= x <= 126 and x != 92:
            out.append(chr(x))
        else:
            out.append(f"\\x{x:02X}")
    return "".join(out)

p = subprocess.run(["git", "ls-tree", "-r", "-z", "--name-only", "HEAD"], check=True, stdout=subprocess.PIPE)
paths = [x for x in p.stdout.split(b"\x00") if x]

missing = []
for pb in paths:
    if not os.path.lexists(pb):
        missing.append(pb)

print(f"total={len(paths)}")
print(f"missing={len(missing)}")
print(f"exists={len(paths)-len(missing)}")

by_top = Counter()
for pb in missing:
    top = pb.split(b"/", 1)[0].decode("ascii", "replace")
    by_top[top] += 1

if missing:
    print("missing_by_top:")
    for k in sorted(by_top):
        print(f"  {k}: {by_top[k]}")

print("missing_paths:")
for pb in missing:
    try:
        cp949 = pb.decode("cp949")
    except UnicodeDecodeError:
        cp949 = "<decode-fail>"
    print(f"{hex_bytes(pb)}\t{cp949}")
PY
