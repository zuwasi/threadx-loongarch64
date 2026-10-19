# Verification Plan

| | |
|---|---|
| Document ID | ESL-TXLA64-SVP |
| Revision | r1, 2026-10-19 |

## 1. Verification activities

| Activity | Method | Covers | Pass criterion |
|---|---|---|---|
| MISRA C:2025 analysis | Parasoft C/C++test | All port and BSP C code | No Mandatory violation; every other violation covered by an approved deviation or the GRP |
| Flow analysis | Parasoft BD-* checkers | All port and BSP C code | No unjustified finding |
| Unit tests | Host-independent tests run on QEMU | LA-REQ-001, 010, BSP drivers | All pass |
| Structural coverage | gcov / Parasoft coverage | Port and BSP C code | 100% statement and branch, MC/DC for multi-condition decisions, or justified gaps |
| Integration tests | ThreadX regression suite on QEMU | LA-REQ-002 to 009, 011, 012, 101 to 107 | 100% pass in Debug (`-O0`), Release (`-O2`) and trace builds |
| Code review | Checklist, independent reviewer | All source, including assembly | All findings closed |
| Design review | Independent reviewer | `design.md` | All findings closed |

## 2. Test environment

| Item | Value |
|---|---|
| Emulator | QEMU 8.2.2, `qemu-system-loongarch64 -M virt -cpu la464 -smp 1` |
| Build | `tools/build.sh Debug` and `tools/build.sh Release` |
| CI | GitHub Actions, Ubuntu 24.04 |

## 3. Records

Test results, coverage reports and Parasoft reports for each baseline are
committed under `docs/verification/` and `docs/misra/reports/`, tagged with the
commit they belong to.
