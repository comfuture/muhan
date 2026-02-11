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

## UTF-8 First migration constraints (2026-02-11)
- Scope: 텍스트 파일은 UTF-8로 통일, 바이너리 파일은 기존 바이트 유지.
- Input policy: 소켓 입력은 UTF-8 strict 처리, invalid sequence는 버림.
- Backspace policy: 입력 버퍼 편집은 바이트 단위가 아니라 UTF-8 코드포인트 단위.
- String-length policy: CP949 2바이트 가정 대신 Unicode 코드포인트 길이 기준 사용.
- Name/path policy: 플레이어 이름은 유니코드 허용(금지문자 차단), 저장 경로는 SHA-1 2자리 샤딩.
- Compatibility policy: CP949 플레이어 데이터/입력 호환은 제공하지 않음.
