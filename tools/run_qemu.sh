#!/usr/bin/env bash
# Run a bare-metal ELF on QEMU loongarch64 virt and judge the result from the
# console output.
#
# Usage: tools/run_qemu.sh [--expect <regex>] [--timeout <s>] <elf>
#   Default expectation: "*** TEST PASSED ***".  A FATAL EXCEPTION or
#   TEST FAILED line fails the run unless it is what --expect asks for.
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT
set -uo pipefail

expect='\*\*\* TEST PASSED \*\*\*'
timeout_s=60
qemu="${QEMU_LOONGARCH64:-qemu-system-loongarch64}"

while [[ $# -gt 1 ]]; do
    case "$1" in
        --expect)  expect="$2"; shift 2 ;;
        --timeout) timeout_s="$2"; shift 2 ;;
        *) echo "unknown option: $1" >&2; exit 2 ;;
    esac
done
elf="$1"
log="${elf}.log"

timeout --foreground "${timeout_s}" \
    "${qemu}" -M virt -cpu la464 -smp 1 -m 1G -nographic \
              -monitor none -serial stdio -no-reboot -nic none \
              -kernel "${elf}" > "${log}" 2>&1
qemu_status=$?
cat "${log}"

if [[ ${qemu_status} -eq 124 ]]; then
    echo "run_qemu: timeout after ${timeout_s}s" >&2
    exit 1
fi
if ! grep -Eq "${expect}" "${log}"; then
    echo "run_qemu: expected output not found: ${expect}" >&2
    exit 1
fi
if [[ "${expect}" == '\*\*\* TEST PASSED \*\*\*' ]] && grep -Eq 'TEST FAILED|FATAL EXCEPTION' "${log}"; then
    echo "run_qemu: failure reported" >&2
    exit 1
fi
exit 0
