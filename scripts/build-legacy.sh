#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "$0")/.." && pwd)"
mode="${1:-c}"

if [[ "$mode" == "c" ]]; then
  exec docker run --rm --platform linux/amd64 \
    -v "$repo_root:/work" \
    -w /work \
    gcc:14 bash -lc '
      set -euo pipefail
      make -C src clean >/dev/null 2>&1 || true
      make -C src -j4 CC=gcc
    '
fi

if [[ "$mode" == "rust" ]]; then
  docker run --rm --platform linux/amd64 \
    -v "$repo_root:/work" \
    -w /work/rust \
    rust:1.77 bash -lc '
      set -euo pipefail
      export PATH="/usr/local/cargo/bin:$PATH"
      cargo build -p muhan-resource-ffi --release
    '

  exec docker run --rm --platform linux/amd64 \
    -v "$repo_root:/work" \
    -w /work \
    gcc:14 bash -lc '
      set -euo pipefail
      make -C src clean >/dev/null 2>&1 || true
      make -C src -j4 CC=gcc USE_RUST_RESOLVER=1 RUST_FFI_LIB=/work/rust/target/release/libmuhan_resource_ffi.a
    '
fi

echo "unknown mode: $mode (expected: c|rust)" >&2
exit 2
