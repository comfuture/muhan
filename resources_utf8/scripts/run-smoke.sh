#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "$0")/.." && pwd)"

run_server_smoke() {
  local image="$1"
  local label="$2"
  local port="$3"

  echo "[${label}] boot + session smoke (port=${port})"
  docker run --rm --platform linux/amd64 \
    -e MUD_PORT="$port" \
    -v "$repo_root:/work" \
    -w /work \
    "$image" bash -lc '
      set -euo pipefail
      mkdir -p /home
      ln -sfn /work /home/muhan
      export MUHAN_HOME=/work

      ./src/frp.new -r "$MUD_PORT" >/tmp/frp.log 2>&1 &
      pid=$!
      trap "kill -9 $pid >/dev/null 2>&1 || true" EXIT

      sleep 1
      if ! kill -0 "$pid" >/dev/null 2>&1; then
        echo "smoke failed: server exited immediately" >&2
        echo "----- /tmp/frp.log (tail) -----" >&2
        tail -n 200 /tmp/frp.log >&2 || true
        exit 1
      fi

      if ! python3 tests/smoke/session_smoke.py; then
        echo "smoke failed: session scenario" >&2
        echo "----- /tmp/frp.log (tail) -----" >&2
        tail -n 200 /tmp/frp.log >&2 || true
        exit 1
      fi

      if ! kill -0 "$pid" >/dev/null 2>&1; then
        echo "smoke failed: server exited early" >&2
        echo "----- /tmp/frp.log (tail) -----" >&2
        tail -n 200 /tmp/frp.log >&2 || true
        exit 1
      fi
    '
}

echo "[c-only] build"
"$repo_root/scripts/build-legacy.sh" c
run_server_smoke gcc:14 "c-only" 4100

echo "[rust-resolver] build"
"$repo_root/scripts/build-legacy.sh" rust

echo "[rust-resolver] resolver parity"
docker run --rm --platform linux/amd64 \
  -v "$repo_root:/work" \
  -w /work \
  gcc:14 bash -lc '
    set -euo pipefail
    export MUHAN_HOME=/work

    gcc -std=gnu89 -fcommon -finput-charset=cp949 -I src \
      tests/smoke/rp_probe.c src/resource_path.c -o /tmp/rp_probe_c

    gcc -std=gnu89 -fcommon -finput-charset=cp949 -DUSE_RUST_RESOLVER -I src \
      tests/smoke/rp_probe.c src/resource_path.c \
      /work/rust/target/release/libmuhan_resource_ffi.a -ldl -lpthread \
      -o /tmp/rp_probe_rust

    python3 tests/smoke/compare_resolvers.py \
      --probe-a /tmp/rp_probe_c \
      --probe-b /tmp/rp_probe_rust \
      --manifest /work/resources_manifest/resource-map.v1.json \
      --muhan-home /work
  '

run_server_smoke gcc:14 "rust-resolver" 4101

echo "[rust] unit tests"
docker run --rm --platform linux/amd64 \
  -v "$repo_root:/work" \
  -w /work/rust \
  rust:1.77 bash -lc '
    set -euo pipefail
    export PATH="/usr/local/cargo/bin:$PATH"
    cargo test --workspace
  '

echo "smoke completed"
