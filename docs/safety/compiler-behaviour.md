# Compiler Behaviour (MISRA C:2025 Dir 1.1)

| | |
|---|---|
| Document ID | ESL-TXLA64-CB |
| Revision | r1, 2026-10-22 |
| Compiler | GCC 14.2.0 `loongarch64-linux-gnu` (Ubuntu 14.2.0-4ubuntu2~24.04) |
| Options | `-std=c99 -march=loongarch64 -mabi=lp64d -msimd=none -mcmodel=normal -mstrict-align -ffreestanding -nostdinc -fno-common -fno-pic -fno-pie` (`cmake/loongarch64_gnu.cmake`) |

Dir 1.1 requires that every implementation-defined behaviour the program
relies on is documented and understood. The values below were obtained
from the compiler's predefined macros with the project options
(`gcc -dM -E`), and they follow the LoongArch ELF psABI (LP64D).

## 1. Data representation

| Item | Value | Relied on by |
|---|---|---|
| `CHAR_BIT` | 8 | Byte-wise memory functions, UART registers |
| Plain `char` | signed (`__CHAR_UNSIGNED__` not defined) | Only text is stored in `char`; byte data uses `uint8_t` |
| `short`, `int`, `long`, `long long` | 16, 32, 64, 64 bits | `tx_port.h` data types (LA-REQ-001), checked at run time by `bringup_timer_and_posture` |
| Pointer, `size_t`, `ptrdiff_t` | 64 bits (`unsigned long`, `long`) | LP64 model, `ALIGN_TYPE`; pointer width checked at compile time in `tx_port.h` |
| `wchar_t` | 32 bits, `int` | Not used |
| Integer representation | Two's complement | Free-running ring counters use unsigned wrap-around only |
| Byte order | Little-endian | Coverage frame encoding, IOCSR and MMIO register layouts |
| Floating point | IEEE 754 (`__GCC_IEC_559 2`), `FLT_EVAL_METHOD 0`, `long double` 128 bits | The port saves FP state only; no FP arithmetic in port or BSP |
| Largest alignment | 16 bytes | Context frame and stack alignment (LA-REQ-008) |

## 2. Behaviour relied on

| Behaviour (C99 annex J.3) | GCC behaviour | Use in this project |
|---|---|---|
| Conversion of an integer to a narrower signed type (J.3.5) | Truncation modulo 2^N | Not relied on: all narrowing conversions are to unsigned types, which C99 defines |
| Right shift of a negative signed value (J.3.5) | Arithmetic shift | Not used; shifts are on unsigned operands (MISRA Rule 10.1) |
| Conversion between pointers and integers (J.3.7) | Value preserved (same width) | MMIO and IOCSR register addresses (DEV-007) |
| Volatile access (J.3.10) | Each access in the source is one access of the declared width | Device register access through `volatile uint8_t`/`uint32_t`; with `-mstrict-align`, no access is split or merged |
| Structure layout and padding (J.3.9) | psABI natural alignment | `TX_THREAD` offsets used by assembly, checked at compile time (`tx_la64_port_assert.c`) |
| `__builtin_offsetof` | GCC builtin behind `offsetof` | Compile-time checks only (`tx_la64_port_assert.c`) |
| `__attribute__((weak))` | Weak symbol | Regression builds only (`TX_REGRESSION_TEST`), not in delivered code |

## 3. Freestanding environment

`-ffreestanding -nostdinc` gives only GCC's own headers (`stdint.h`,
`stddef.h`, `stdbool.h`). The four memory functions are provided by the
port (LA-REQ-010, DEV-001). GCC may emit calls to them for structure
copies and initialisation. The CI job `tools/check_freestanding_libc.sh`
confirms that the images have no other undefined library references.

## 4. Re-validation

This document must be re-checked whenever the compiler version, the
options or the ABI change (tool change request, `docs/safety/tools.md`).
