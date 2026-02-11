# 무한대전(MUHAN) 레거시 MUD 복구 프로젝트

무한대전은 1990년대 스타일의 텍스트 기반 MUD(Multi-User Dungeon) 게임입니다.
이 저장소는 오래된 C 코드베이스와 게임 리소스를 현대 개발환경(특히 Linux 컨테이너)에서 다시 구동할 수 있도록 복구하는 작업을 담고 있습니다.

## 게임 개요
- 장르: 텍스트 기반 온라인 RPG(MUD)
- 핵심 요소: 방/이동/전투/NPC 대화/플레이어 저장
- 기술 특성: 레거시 C 소스 + 다수의 파일 기반 리소스

## 복구 배경
기존 코드/데이터는 다음 문제를 가지고 있었습니다.
- 비ASCII(주로 CP949 계열) 파일명으로 인해 macOS에서 체크아웃 불가 파일 발생
- 레거시 컴파일 관행(구식 선언/시그니처/헤더 누락)으로 최신 툴체인에서 빌드 실패
- 리소스 경로가 과거 바이트 경로에 강하게 결합되어 이식성 저하

## 이번 복구의 핵심
1. **리소스 복원 및 정규화**
- Git blob 기반으로 누락 리소스를 복구
- `resources_utf8/`에 UTF-8 안전 경로 트리 구성
- `resources_manifest/resource-map.v1.json` + `path-alias.v1.tsv` 생성

2. **C 런타임 경로 해석 계층 도입**
- 레거시 바이트 경로 -> 정규화 경로 해석기 추가
- 주요 파일 접근 지점(`talk/ddesc/help/player`)에 해석 계층 적용
- `MUHAN_HOME` 환경변수로 런타임 루트 오버라이드 지원

3. **현대 빌드/실행 복원**
- GNU89 기반 빌드로 복원
- Linux amd64 컨테이너에서 서버 빌드 및 부팅 가능
- 스모크 테스트 스크립트로 핵심 시나리오 자동 검증

4. **Rust 2단계 기반 마련**
- 리소스 인덱스/경로 해석을 Rust 크레이트로 분리
- C <-> Rust FFI 경로 해석 인터페이스 연결
- C resolver와 Rust resolver 결과 동치 검증 도구 제공

## 빠른 실행
```bash
# 1) 누락 경로 리포트
./tools/revive/report-missing-paths.sh

# 2) 리소스 정규화/manifest 재생성
./tools/revive/extract-legacy-blobs.sh resources_utf8 resources_manifest

# 3) 통합 스모크(C-only + Rust resolver)
./scripts/run-smoke.sh
```

## 디렉터리 가이드
- `tools/revive/`: 복구 유틸리티 스크립트
- `resources_utf8/`: 정규화된 리소스 트리
- `resources_manifest/`: 경로/리소스 매핑 데이터
- `src/`: 레거시 C 서버 소스 + 경로 해석 계층
- `rust/`: 리소스 인덱스/FFI 크레이트
- `scripts/`, `tests/smoke/`: 빌드/부팅/회귀 검증 자동화
- `docs/revive/`: 기준선/복구 메모

## 참고
- 원본 레거시 데이터의 바이트 경로 정보는 manifest에 보존됩니다.
- macOS 네이티브보다 Linux 컨테이너 기반 실행을 우선 권장합니다.

## macOS 호환 패치 내역
- 브랜치: `feat/macos-compat`
- 목적: macOS(Apple clang)에서 네이티브 빌드/기동이 가능하도록 레거시 C 코드 호환성 보강
주요 수정:
- 누락 함수 선언 보강(`scwrite`, `broadcast_eaves`, `find_enm_crt`, `special_cmd`, `log_fl`, `log_dmcmd`)
- `qsort` 비교 함수 시그니처를 표준형(`const void *`)으로 맞춤
- `command8.c`에 `<ctype.h>` 추가(문자 분류 함수 선언 누락 해결)
- `isnumber` 이름을 `is_number_str`로 변경(macOS libc 심볼 충돌 회피)
- `load_family()`에서 `fopen` 실패 시 `NULL` 체크 추가(시작 시 segfault 방지)
- macOS에서 clang 경고를 오류로 승격하지 않도록 `src/Makefile`에 Darwin 전용 완화 플래그 추가

## 플랫폼별 컴파일/실행 명령
아래 명령은 모두 저장소 루트에서 실행합니다.

### macOS 네이티브 (Apple clang)
```bash
# 컴파일
make -C src -j1 CC=cc

# 실행
MUHAN_HOME="$PWD" ./src/frp.new -r 4102
```

### Linux (Docker, C-only 권장)
```bash
# 컴파일
./scripts/build-legacy.sh c

# 실행(스모크 포함)
./scripts/run-smoke.sh
```

### Linux (Docker, Rust resolver 포함)
```bash
# 컴파일(Rust FFI + C 서버)
./scripts/build-legacy.sh rust

# 실행 및 검증(C-only + Rust resolver 통합 스모크)
./scripts/run-smoke.sh
```
