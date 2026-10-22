#!/usr/bin/env bash
# Build, execute and report Parasoft structural coverage for ESL-owned
# LoongArch64 port and QEMU BSP C code. Build and coverage data are kept in a
# fresh output directory; no existing files are removed.
#
# Every scope (root images, each regression configuration) is configured,
# built, tested, collected and analysed with its own coverage workspace, so
# each report is computed from a single variant-free compilation of every
# source file.  The regression configurations redefine ThreadX TX_* macros,
# so merging their coverage data into one report collides on shared function
# names (e.g. _tx_timer_interrupt) and suppresses that data instead.
#
# Usage: tools/parasoft/coverage.sh [fresh-output-dir] [root|all]
#   root  the root QEMU images (bring-up tests and demo)
#   all   root plus all six regression configurations, one report per scope
# Requires CPPTEST_HOME and, for report generation, CPPTEST_SETTINGS.
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cpptest_home="${CPPTEST_HOME:-${HOME}/parasoft-standard-2026.1.3/cpptest}"
out="${1:-${root}/build/parasoft-coverage}"
scope="${2:-all}"
gcc="loongarch64-linux-gnu-gcc-14"

if [[ "${scope}" != "root" && "${scope}" != "all" ]]; then
    echo "scope must be 'root' or 'all'" >&2
    exit 2
fi

if [[ -e "${out}" ]]; then
    echo "output directory already exists; choose a fresh path: ${out}" >&2
    exit 2
fi
if [[ ! -x "${cpptest_home}/bin/cpptestcc" || ! -x "${cpptest_home}/cpptestcli" ]]; then
    echo "C/C++test installation not found under ${cpptest_home}" >&2
    exit 2
fi
if ! command -v "${gcc}" >/dev/null 2>&1; then
    echo "LoongArch GCC not found: ${gcc}" >&2
    exit 2
fi

mkdir -p "${out}"
out="$(cd "${out}" && pwd)"
runtime_dir="${out}/runtime"
runtime_source="${runtime_dir}/cpptest_uart_runtime.c"
runtime_object="${runtime_dir}/cpptest_uart_runtime.o"
mkdir -p "${runtime_dir}"

python3 "${root}/tools/parasoft/prepare_coverage_runtime.py" \
    "${cpptest_home}/runtime/src/cpptest.c" "${runtime_source}"

gcc_include="$("${gcc}" -print-file-name=include)"
"${gcc}" \
    -std=c99 -march=loongarch64 -mabi=lp64d -msimd=none \
    -mcmodel=normal -mstrict-align -ffreestanding \
    -isystem "${gcc_include}" \
    -I "${cpptest_home}/runtime/include" \
    -I "${root}/bsp/qemu_virt/inc" \
    -DCPPTEST_CUSTOM_COMMUNICATION=1 \
    -DCPPTEST_INTERNAL_BUFFER=1 -DCPPTEST_INTERNAL_BUFFER_SIZE=65536U \
    -DCPPTEST_MULTI_PROCESSES_LOG=0 \
    -DCPPTEST_NO_THREADS=1 -DCPPTEST_SHARED=0 \
    -fno-common -fno-pic -fno-pie -ffunction-sections -fdata-sections \
    -c "${runtime_source}" -o "${runtime_object}"

export ESL_REPO_ROOT="${root}"
export CPPTESTCC="${cpptest_home}/bin/cpptestcc"

common_cmake_args=(
    -G Ninja
    "-DCMAKE_TOOLCHAIN_FILE=${root}/cmake/loongarch64_gnu.cmake"
    "-DCMAKE_C_COMPILER_LAUNCHER=${root}/tools/parasoft/coverage_launcher.sh"
    "-DCMAKE_PROJECT_INCLUDE=${root}/tools/parasoft/coverage_build.cmake"
    "-DCPPTEST_RUNTIME_OBJECT=${runtime_object}"
    "-DCPPTEST_RUNTIME_INCLUDE_DIR=${cpptest_home}/runtime/include"
    -DCMAKE_BUILD_TYPE=Debug
)

# run_scope <name> <source-dir> [extra cmake args...]
# Configures, builds and tests one scope, then collects its QEMU logs into a
# CLOG and generates one report with its own coverage workspace.
run_scope()
{
    local name="$1"
    local source_dir="$2"
    shift 2
    local build_dir="${out}/${name}"
    local ws="${out}/workspace-${name}"
    local clog="${out}/cpptest_results-${name}.clog"
    local report="${out}/report-${name}"
    local settings=()
    local logs=()

    echo "Configuring and testing coverage scope: ${name}"
    mkdir -p "${ws}"
    CPPTEST_COVERAGE_WORKSPACE="${ws}" cmake -S "${source_dir}" -B "${build_dir}" \
        "${common_cmake_args[@]}" "$@" > "${out}/configure-${name}.log"
    CPPTEST_COVERAGE_WORKSPACE="${ws}" cmake --build "${build_dir}" --parallel 1 \
        > "${out}/build-${name}.log"
    ctest --test-dir "${build_dir}" --output-on-failure --parallel 1 \
        > "${out}/ctest-${name}.log"

    while IFS= read -r -d '' f; do
        logs+=("${f}")
    done < <(find "${build_dir}" -type f -name '*.elf.log' -print0)
    if [[ ${#logs[@]} -eq 0 ]]; then
        echo "no QEMU console logs found under ${build_dir}" >&2
        exit 1
    fi
    python3 "${root}/tools/parasoft/collect_coverage.py" "${clog}" "${logs[@]}"

    if [[ -n "${CPPTEST_SETTINGS:-}" ]]; then
        settings=(-localsettings "${CPPTEST_SETTINGS}")
    fi
    "${cpptest_home}/cpptestcli" "${settings[@]}" \
        -config "builtin://Coverage" \
        -input "${clog}" \
        -workspace "${ws}" \
        -report "${report}" \
        -property "report.format=html,xml" > "${out}/report-${name}.log" 2>&1
    echo "Coverage report for ${name}: ${report}"
}

run_scope root "${root}"

if [[ "${scope}" == "all" ]]; then
    configs=(default optimized disable_notify_callbacks stack_checking
             stack_checking_rand_fill trace)
    for configuration in "${configs[@]}"; do
        run_scope "regression-${configuration}" "${root}/test/regression" \
            "-DLA64_REGRESSION_CONFIG=${configuration}"
    done
fi

echo "All coverage reports written under ${out}"
