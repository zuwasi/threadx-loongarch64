#!/usr/bin/env bash
# Configure and build one configuration.
# Usage: tools/build.sh [Debug|Release] [build-dir]
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build_type="${1:-Debug}"
build_dir="${2:-${root}/build/${build_type,,}}"

cmake -S "${root}" -B "${build_dir}" -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE="${root}/cmake/loongarch64_gnu.cmake" \
      -DCMAKE_BUILD_TYPE="${build_type}"
cmake --build "${build_dir}"
