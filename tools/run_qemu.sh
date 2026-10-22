#!/usr/bin/env bash
# Run a bare-metal ELF on QEMU loongarch64 virt and judge the result from the
# console output.
#
# Usage: tools/run_qemu.sh [--expect <regex>] [--timeout <s>]
#                          [--rx-file <file> --rx-after <regex>] <elf>
#   Default expectation: "*** TEST PASSED ***".  A FATAL EXCEPTION or
#   TEST FAILED line fails the run unless it is what --expect asks for.
#   --rx-file feeds the file's bytes to the guest's serial input once the
#   console log matches --rx-after (the guest enables its receive path
#   first).  The two flags must be given together.
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT
set -uo pipefail

expect='\*\*\* TEST PASSED \*\*\*'
timeout_s=60
rx_file=""
rx_after=""
qemu="${QEMU_LOONGARCH64:-qemu-system-loongarch64}"

while [[ $# -gt 1 ]]; do
    case "$1" in
        --expect)  expect="$2"; shift 2 ;;
        --timeout) timeout_s="$2"; shift 2 ;;
        --rx-file) rx_file="$2"; shift 2 ;;
        --rx-after) rx_after="$2"; shift 2 ;;
        *) echo "unknown option: $1" >&2; exit 2 ;;
    esac
done
elf="$1"
log="${elf}.log"

qemu_cmd=(-M virt -cpu la464 -smp 1 -m 1G -nographic
          -monitor none -serial stdio -no-reboot -nic none
          -kernel "${elf}")

if [[ -n "${rx_file}" ]]; then
    # Feed the guest's serial input from a FIFO while logging stdout.  The
    # write end stays open so the UART never sees host EOF.
    if [[ -z "${rx_after}" ]]; then
        echo "run_qemu: --rx-file requires --rx-after" >&2
        exit 2
    fi
    rx_dir="$(mktemp -d)"
    rx_fifo="${rx_dir}/serial.in"
    mkfifo "${rx_fifo}"
    timeout --foreground "${timeout_s}" "${qemu}" "${qemu_cmd[@]}" \
        < "${rx_fifo}" > "${log}" 2>&1 &
    qemu_pid=$!
    exec 3>"${rx_fifo}"
    # Give the guest up to 10 s to announce readiness, then send the file.
    for _ in $(seq 1 100); do
        if grep -Eq "${rx_after}" "${log}" 2>/dev/null; then
            break
        fi
        sleep 0.1
    done
    cat "${rx_file}" >&3
    wait "${qemu_pid}"
    qemu_status=$?
    exec 3>&-
    rm -rf "${rx_dir}"
else
    timeout --foreground "${timeout_s}" "${qemu}" "${qemu_cmd[@]}" \
        > "${log}" 2>&1
    qemu_status=$?
fi
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
