# Parasoft C/C++test tool validation: seeded MISRA C:2025 violations

Confidence measure for Parasoft C/C++test (class T2), defined in
`docs/safety/tools.md`, section 3.2.  `seeded_violations.c` contains one
deliberate violation per listed guideline.  It is never built into a product
or test image.

Run: `tools/parasoft/validate.sh` (report in `build/tool-validation/report`).
The run uses the project configuration
(`tools/parasoft/ESL_MISRA_C_2025.properties`) and the custom LoongArch
compiler definition (`tools/parasoft/make_compiler_def.sh`), so it also
validates that the compiler definition lets the analyser parse
LoongArch-compiled sources.

## Record

Run 2026-10-20, Parasoft C/C++test Standard 2026.1.3, GCC 14.2.0
`loongarch64-linux-gnu`.

| Seeded guideline | Function | Reported by |
|---|---|---|
| Rule 2.3 | `seeded_unused_type_check` | `RULE_2_3-b` |
| Rule 11.4 | `seeded_mmio` | `RULE_11_4-b` |
| Rule 10.2 | `seeded_char_arith` | `RULE_10_2-a` |
| Rule 10.8 | `seeded_widen` | `RULE_10_8-a` |
| Rule 18.4 | `seeded_pointer_walk` | `RULE_18_4-a` |
| Rule 8.13 | `seeded_read_only` | `RULE_8_13-a` |
| Rule 8.4 | `seeded_no_prototype` (and every other function) | `RULE_8_4-a` |
| Rule 17.7 | `seeded_discard` | `RULE_17_7-a` |

Result: **8 of 8 seeded guidelines reported.**  17 findings in total; the
extra findings (Rule 8.4 on every function, Rule 8.7, a second Rule 10.8) are
real consequences of the seeded code, not false positives.

The first run used `'a' + index` with an unsigned `index` as the Rule 10.2
seed and reported nothing for it.  That expression is a permitted use under
Rule 10.2 (character plus unsigned), so the seed was wrong, not the tool.  The
seed now adds two character operands.
