#!/usr/bin/env bash
# Parasoft C/C++test tool validation with seeded MISRA C:2025 violations
# (docs/safety/tools.md, section 3.2).
#
# Runs the project analysis configuration on
# test/tool_validation/parasoft/seeded_violations.c and writes the SARIF
# report to <output-dir>/report.  Every seeded violation listed in the
# file's header must appear in the report; the record is kept in
# test/tool_validation/parasoft/README.md.
#
# Usage: tools/parasoft/validate.sh [<output-dir>]
#   CPPTEST_HOME, CPPTEST_SETTINGS as in tools/misra.sh
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cpptest="${CPPTEST_HOME:-${HOME}/parasoft-standard-2026.1.3/cpptest}"
out="${1:-${root}/build/tool-validation}"

rm -rf "${out}"
mkdir -p "${out}"
# Absolute from here on: cpptestcli is invoked after cd "${out}", so a
# relative output directory must not leak into -property/-report values.
out="$(cd "${out}" && pwd)"

compiler_id="$(bash "${root}/tools/parasoft/make_compiler_def.sh" "${cpptest}" "${out}/compilers")"

settings=()
if [[ -n "${CPPTEST_SETTINGS:-}" ]]; then
    settings=(-localsettings "${CPPTEST_SETTINGS}")
fi

# The build tracer only records compilers it recognises by name.
export CPPTEST_SCAN_TRACE_COMMAND='/loongarch64-linux-gnu-gcc(-[0-9]+)?$'

cat > "${out}/compile_seeded.sh" <<EOF
#!/usr/bin/env bash
loongarch64-linux-gnu-gcc-14 -std=c99 -c "${root}/test/tool_validation/parasoft/seeded_violations.c" -o "${out}/seeded_violations.o"
EOF

cd "${out}"
"${cpptest}/cpptestcli" "${settings[@]}" \
    -config "${root}/tools/parasoft/ESL_MISRA_C_2025.properties" \
    -compiler "${compiler_id}" \
    -property "cpptest.compiler.dir.user=${out}/compilers" \
    -property "report.format=sarif" \
    -report "${out}/report" \
    -trace bash "${out}/compile_seeded.sh"
