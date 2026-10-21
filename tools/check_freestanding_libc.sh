#!/usr/bin/env bash
# Verify the freestanding memory functions generate no undefined references
# (DEV-001, docs/misra/deviations/DEV-001.md, sections 3-4).
#
# The port declares memcpy/memmove/memset/memcmp under their reserved names
# because GCC and TX_MEMSET require them in a freestanding build.  The object
# that defines them must be self-contained: an undefined symbol would mean
# the compiler replaced a loop with a call back into the library (or pulled
# in an unexpected dependency), which is the hazard DEV-001 guards against.
#
# Usage: tools/check_freestanding_libc.sh [<build-dir>]
#   <build-dir>: any build tree containing tx_la64_string.c.obj
#               (default: build/release)
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build="${1:-${root}/build/release}"

obj="$(find "${build}" -name 'tx_la64_string.c.obj' -print -quit 2>/dev/null)"
if [[ -z "${obj}" ]]; then
    echo "tx_la64_string.c.obj not found under ${build}" >&2
    exit 1
fi

nm=loongarch64-linux-gnu-nm
if ! command -v "${nm}" >/dev/null 2>&1; then
    nm=nm
fi

undefined="$("${nm}" -u "${obj}")"
if [[ -n "${undefined}" ]]; then
    echo "FAIL: ${obj} has undefined symbols (DEV-001 sections 3-4):" >&2
    printf '%s\n' "${undefined}" >&2
    exit 1
fi
echo "OK: ${obj} has no undefined symbols (DEV-001 sections 3-4)"
