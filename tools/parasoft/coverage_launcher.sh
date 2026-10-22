#!/usr/bin/env bash
# Instrument only ESL-owned port and BSP C translation units. All other
# sources and assembly are compiled by the original compiler unchanged.
set -euo pipefail

compiler="$1"
shift
source_file=""
for argument in "$@"; do
    case "${argument}" in
        *.c) source_file="${argument}" ;;
    esac
done

case "${source_file}" in
    "${ESL_REPO_ROOT}/ports/loongarch64/gnu/"*.c|"${ESL_REPO_ROOT}/bsp/qemu_virt/"*.c)
        exec "${CPPTESTCC}" \
            -compiler gcc_14-64 \
            -line-coverage \
            -statement-coverage \
            -decision-coverage \
            -mcdc-coverage \
            -coverage-data-variants \
            -coverage-early-init \
            -workspace "${CPPTEST_COVERAGE_WORKSPACE}" \
            -- "${compiler}" "$@"
        ;;
    *)
        exec "${compiler}" "$@"
        ;;
esac
