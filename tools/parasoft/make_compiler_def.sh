#!/usr/bin/env bash
# Derive a C/C++test compiler definition for loongarch64-linux-gnu-gcc-14 from
# the installed GNU GCC 14 x86_64 definition.  The vendor definition files are
# not stored in this repository; only the differences below are.
#
# Usage: tools/parasoft/make_compiler_def.sh <cpptest-install-dir> <output-dir>
#
# Differences from gcc_14-64, each checked against
# `loongarch64-linux-gnu-gcc-14 -mabi=lp64d -dM -E` (see docs/safety/tools.md):
#   long double     IEEE binary128 (__LDBL_MANT_DIG__ 113), 16 bytes
#   target          not x86_64
#   built-in macros LoongArch macros added to the probe list, so the real
#                   compiler's values are used (tx_port.h checks them)
#   executables     loongarch64-linux-gnu-gcc[-14]
# Unchanged and identical on LA64 LP64D: char signed, short 2, int 4, long 8,
# long long 8, pointer 8, size_t unsigned long, ptrdiff_t/intmax_t long,
# wchar_t int, wint_t unsigned int.
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT
set -euo pipefail

cpptest="$1"
out_root="$2"
base="${cpptest}/bin/engine/etc/compilers/gcc_14-64"
id=gcc_14-loongarch64
out="${out_root}/${id}"

[[ -f "${base}/c.psrc" ]] || { echo "missing ${base}/c.psrc" >&2; exit 1; }
mkdir -p "${out}"

for f in c.psrc cpp.psrc; do
    sed -e 's/^edg\.targ_ldbl_mant_dig .*/edg.targ_ldbl_mant_dig 113/' \
        -e 's/^edg\.targ_supports_x86_64 .*/edg.targ_supports_x86_64 FALSE/' \
        "${base}/${f}" > "${out}/${f}"
    for m in __loongarch__ __loongarch64 __loongarch_lp64 __loongarch_grlen \
             __loongarch_frlen __loongarch_hard_float __loongarch_double_float \
             __loongarch_single_float __loongarch_soft_float __loongarch_sx \
             __loongarch_asx __loongarch_arch __loongarch_tune \
             __loongarch_version_major __loongarch_version_minor \
             _LOONGARCH_ARCH _LOONGARCH_TUNE _LOONGARCH_SIM _LOONGARCH_SZINT \
             _LOONGARCH_SZLONG _LOONGARCH_SZPTR _ABILP64; do
        echo "edgtk.builtInMacroToCheck ${m}" >> "${out}/${f}"
    done
    grep -q '^edg.targ_ldbl_mant_dig 113$' "${out}/${f}"
    grep -q '^edg.targ_supports_x86_64 FALSE$' "${out}/${f}"
done

sed -e 's/^name=.*/name=GNU GCC 14.x (LoongArch64, ESL)/' \
    -e 's/^cExecutable=.*/cExecutable=loongarch64-linux-gnu-gcc-14/' \
    -e 's/^cppExecutable=.*/cppExecutable=loongarch64-linux-gnu-g++-14/' \
    -e 's/^linkerExecutable=.*/linkerExecutable=loongarch64-linux-gnu-gcc-14/' \
    -e 's/^cExecutablePattern=.*/cExecutablePattern=loongarch64-linux-gnu-gcc.*/' \
    -e 's/^cppExecutablePattern=.*/cppExecutablePattern=loongarch64-linux-gnu-g\\\\+\\\\+.*/' \
    -e 's/^linkerExecutablePattern=.*/linkerExecutablePattern=loongarch64-linux-gnu-gcc.*/' \
    -e 's/^asmToolAcronym=.*/asmToolAcronym=/' \
    -e 's/ -lcpptestruntime64//' -e 's/^cpptestDebug=.*/cpptestDebug=/' \
    "${base}/gui.properties" > "${out}/gui.properties"

echo "${id}"
