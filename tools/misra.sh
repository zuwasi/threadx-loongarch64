#!/usr/bin/env bash
# MISRA C:2025 analysis of the port and BSP with Parasoft C/C++test
# (docs/misra/gep.md).
#
# Usage: tools/misra.sh [<scope>] [<output-dir>]
#   <scope>:  product (default) | tests
#
#     product  Compliance scope: the threadx library (kernel sources plus
#              the port) and the BSP library, Release build.  Test code is
#              not built (docs/misra/gep.md, scope table).
#     tests    Test-harness scope, reported separately: bring-up, demo and
#              the full regression harness build (default configuration),
#              which also compiles the port with TX_REGRESSION_TEST.
#
#   CPPTEST_HOME      C/C++test install directory (contains cpptestcli)
#   CPPTEST_SETTINGS  optional local settings file with the license settings;
#                     kept outside the repository (it holds credentials)
#
# Reports: <output-dir>/report (HTML, SARIF, XML).
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cpptest="${CPPTEST_HOME:-${HOME}/parasoft-standard-2026.1.3/cpptest}"
scope="${1:-product}"
out="${2:-${root}/build/misra}"

case "${scope}" in
product|tests) ;;
*)
    echo "usage: tools/misra.sh [product|tests] [<output-dir>]" >&2
    exit 2
    ;;
esac

rm -rf "${out}"
mkdir -p "${out}"
# Absolute from here on: cpptestcli is invoked after cd "${out}", so a
# relative output directory must not leak into -property/-report values.
out="$(cd "${out}" && pwd)"

compiler_id="$(bash "${root}/tools/parasoft/make_compiler_def.sh" "${cpptest}" "${out}/compilers")"

toolchain_args=(-DCMAKE_TOOLCHAIN_FILE="${root}/cmake/loongarch64_gnu.cmake")

cmake -S "${root}" -B "${out}/build" -G Ninja \
      "${toolchain_args[@]}" \
      -DCMAKE_BUILD_TYPE=Release > "${out}/configure.log"

if [[ "${scope}" == "product" ]]; then
    trace=(cmake --build "${out}/build" --target threadx bsp_qemu_virt)
else
    # Regression harness build (default configuration, as run by
    # tools/regression.sh).  TX_REGRESSION_TEST is defined here, so the port
    # hook code compiled only into regression builds is analysed too.
    cmake -S "${root}/test/regression" -B "${out}/regression" -G Ninja \
          "${toolchain_args[@]}" \
          -DCMAKE_BUILD_TYPE=Debug \
          -DLA64_REGRESSION_CONFIG=default > "${out}/configure-regression.log"

    cat > "${out}/build_tests.sh" <<EOF
set -euo pipefail
cmake --build "${out}/build" --target bringup_test bringup_fatal_test bringup_memory_test bringup_long_idle_test bringup_relinquish_test bringup_uart_rx_test threadx_demo
cmake --build "${out}/regression"
EOF

    trace=(bash "${out}/build_tests.sh")
fi

settings=()
if [[ -n "${CPPTEST_SETTINGS:-}" ]]; then
    settings=(-localsettings "${CPPTEST_SETTINGS}")
fi

# The build tracer only records compilers it recognises by name.
export CPPTEST_SCAN_TRACE_COMMAND='/loongarch64-linux-gnu-gcc(-[0-9]+)?$'

cd "${out}"
"${cpptest}/cpptestcli" "${settings[@]}" \
    -config "${root}/tools/parasoft/ESL_MISRA_C_2025.properties" \
    -compiler "${compiler_id}" \
    -property "cpptest.compiler.dir.user=${out}/compilers" \
    -property "report.format=html,sarif,xml" \
    -module "port=${root}/ports/loongarch64" \
    -module "bsp=${root}/bsp" \
    -workspace "${out}/workspace" \
    -report "${out}/report" \
    -showdetails \
    -trace "${trace[@]}"
