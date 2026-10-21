#!/usr/bin/env bash
# Configure, build, and run the ThreadX upstream regression suite for one
# configuration on the QEMU loongarch64 "virt" machine.
#
# Usage: tools/regression.sh [<config>] [<build-dir>]
#   <config>: default | optimized | disable_notify_callbacks | stack_checking |
#             stack_checking_rand_fill | trace | all
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
config="${1:-default}"

configs=(default optimized disable_notify_callbacks stack_checking
         stack_checking_rand_fill trace)

run_one()
{
    local cfg="$1"
    local build_dir="${2:-${root}/build/regression_${cfg}}"

    cmake -S "${root}/test/regression" -B "${build_dir}" -G Ninja \
          -DCMAKE_TOOLCHAIN_FILE="${root}/cmake/loongarch64_gnu.cmake" \
          -DCMAKE_BUILD_TYPE=Debug \
          -DLA64_REGRESSION_CONFIG="${cfg}"
    cmake --build "${build_dir}"
    ctest --test-dir "${build_dir}" --output-on-failure -j "$(nproc)"
}

if [[ "${config}" == "all" ]]; then
    for cfg in "${configs[@]}"; do
        run_one "${cfg}" "${2:-}"
    done
else
    run_one "${config}" "${2:-}"
fi
