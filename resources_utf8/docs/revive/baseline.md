# Muhan Revival Baseline

## Snapshot
- Repository: `/Users/comfuture/Project/muhan`
- Commit (HEAD): `4266c30752d5ab8351b29d5b9c4897654cf0741d`
- Date: 2026-02-08

## Filesystem vs HEAD
- Total tracked paths in HEAD: `3803`
- Existing on macOS checkout: `3711`
- Missing due filename/encoding constraints: `92`
- Missing by top-level directory:
- `help`: `1`
- `objmon`: `89`
- `player`: `2`

Reference command:
```bash
tools/revive/report-missing-paths.sh
```

## Resource normalization outputs
- Normalized resource tree: `resources_utf8/`
- Manifest: `resources_manifest/resource-map.v1.json`
- Alias table: `resources_manifest/path-alias.v1.tsv`
- Manifest entries: `3803`
- Paths containing non-ASCII bytes in legacy tree: `92`

Reference command:
```bash
tools/revive/extract-legacy-blobs.sh resources_utf8 resources_manifest
```

## Known compile baseline (before fixes)
- Legacy code requires GNU89 compatibility behavior.
- Primary hard blockers observed:
- `Port` multiple definition (`src/mtype.h` defines storage in header).
- `enemy_status` function pointer signature mismatch in command table.
- Many implicit function declaration/header issues on modern compilers.

## Repro commands
```bash
# Missing path report
tools/revive/report-missing-paths.sh

# Generate normalized resources + manifests
tools/revive/extract-legacy-blobs.sh resources_utf8 resources_manifest

# Linux container compile (legacy baseline)
docker run --rm --platform linux/amd64 \
  -v "$(pwd)":/work -w /work gcc:14 bash -lc \
  'make -C src clean >/dev/null 2>&1 || true; make -C src CC=gcc -j4'
```
