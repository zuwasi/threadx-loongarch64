# Tool Classification and Qualification

| | |
|---|---|
| Document ID | ESL-TXLA64-TCQ |
| Standard | IEC 61508-3:2010, 7.4.4 |
| Revision | r2, 2026-10-20 (Parasoft validation results, section 3.2) |

## 1. Classes

| Class | Definition (IEC 61508-4, 3.2.11-3.2.13) |
|---|---|
| T1 | Generates no output that can directly or indirectly contribute to the executable code. |
| T2 | Supports test or verification; an error can fail to reveal a defect but cannot create one. |
| T3 | Generates output that can directly or indirectly contribute to the executable code. |

## 2. Tool list

| Tool | Version | Use | Class | Confidence measures |
|---|---|---|---|---|
| GCC `loongarch64-linux-gnu` | 14.2.0 (Ubuntu 14.2.0-4ubuntu2~24.04) | Compile C and assembly | T3 | See 3.1 |
| GNU binutils (as, ld, objcopy) | Ubuntu 24.04 package | Assemble, link | T3 | See 3.1 |
| CMake | Ubuntu 24.04 package | Generate build | T3 | Link map and object list reviewed per release |
| Ninja | 1.x | Run build | T1 | Output is fully determined by CMake |
| Parasoft C/C++test Standard | 2026.1.3, MISRA C 2025 config (rule map 9.5.0) | MISRA and flow analysis, coverage | T2 | See 3.2 |
| QEMU `qemu-system-loongarch64` | 8.2.2 (Ubuntu 1:8.2.2+ds-0ubuntu1.18) | Test execution platform | T2 | See 3.3 |
| CTest | with CMake | Run tests, collect results | T2 | Results cross-checked against console logs |
| git, GitHub Actions | | Configuration management, CI | T1 | |

## 3. Confidence measures

### 3.1 GCC and binutils (T3)

IEC 61508-3 7.4.4.4 requires evidence that the tool conforms to its
specification, or that failures have been mitigated. Measures:

1. **Restricted use.** C99 only (`-std=c99`), no GNU extensions in C sources,
   freestanding, no C library (`-nostdinc -nostdlib`), fixed flags in
   `cmake/loongarch64_gnu.cmake`.
2. **Optimisation levels.** The regression suite runs on the exact binaries
   for both `-O0` and `-O2`; both must pass.
3. **Object inspection.** Port assembly output is checked by disassembly
   review for the context save/restore paths.
4. **Known issues.** The GCC 14 bug list for the LoongArch back end is reviewed
   at each tool update; relevant issues are recorded here.
5. **Version pinning.** The exact package version is recorded and checked in CI.

### 3.2 Parasoft C/C++test (T2)

1. Vendor qualification kit (TÜV certificate for C/C++test) to be referenced
   for the assessment.
2. **Known-violation samples.** `test/tool_validation/parasoft/seeded_violations.c`
   seeds one violation each of Rules 2.3, 8.4, 8.13, 10.2, 10.8, 11.4, 17.7
   and 18.4. `tools/parasoft/validate.sh` analyses it with the project
   configuration; every seeded violation must be reported. Result
   (2026-10-20): 8 of 8 reported; record in
   `test/tool_validation/parasoft/README.md`. Rerun at every tool or
   configuration change.
3. **Compiler configuration.** C/C++test has no built-in definition for the
   LoongArch cross compiler. `tools/parasoft/make_compiler_def.sh` derives
   `gcc_14-loongarch64` from the vendor `gcc_14-64` definition (LP64 data
   model, same GCC front end) and points it at
   `loongarch64-linux-gnu-gcc-14`; the build tracer is restricted to that
   compiler (`CPPTEST_SCAN_TRACE_COMMAND`). The definition is validated by the
   known-violation run above and by the product and test-harness analyses,
   which parse every port, BSP and kernel translation unit without
   preprocessing or parse errors.
4. **Analysis scopes.** `tools/misra.sh product` (compliance scope: kernel,
   port and BSP, Release build) and `tools/misra.sh tests` (test harness,
   including the port's `TX_REGRESSION_TEST` code). Reports are kept in
   `docs/misra/reports/`.
### 3.3 QEMU (T2)

1. Tests check behaviour (ordering, counts, results), not wall-clock time.
2. A real-hardware run on a LoongArch board is planned after release 1.0 to
   confirm the QEMU results.
3. QEMU deviations from the LoongArch reference manual found during the project
   are recorded here.

## 4. Approval

| Role | Name | Date | Decision |
|---|---|---|---|
| Technical lead | Osnat Gropper, ESL | | Pending |
| Safety manager | Daniel Liezrowice, Managing Partner & Founder, ESL | 2026-10-19 | Approved |
