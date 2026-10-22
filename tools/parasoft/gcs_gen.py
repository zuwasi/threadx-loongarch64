#!/usr/bin/env python3
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT
"""Generate the Guideline Compliance Summary (GEP section 6).

Reads docs/misra/grp.md (the guideline list) and, for a completeness check,
the product-scope SARIF report.

Usage: tools/parasoft/gcs_gen.py [<sarif>] > docs/misra/reports/guideline-compliance-summary.md  Every guideline gets one of: Compliant,
Deviations (with record IDs), or Disapplied.
"""
import json
import re
import sys

GRP = "docs/misra/grp.md"
SARIF = sys.argv[1] if len(sys.argv) > 1 else "build/misra/report/report.sarif"

# Guidelines deviated by approved-track records (status: draft, approval
# pending).  Derived from docs/misra/deviations/DEV-*.md.
DEVIATED = {
    "Dir 4.1": "DEV-014",
    "Dir 4.6": "DEV-003, DEV-008",
    "Dir 4.9": "DEV-004",
    "Rule 1.1": "DEV-005",
    "Rule 2.3": "DEV-010",
    "Rule 2.8": "DEV-013",
    "Rule 5.10": "DEV-001, DEV-002",
    "Rule 8.6": "DEV-006, DEV-013",
    "Rule 8.7": "DEV-011",
    "Rule 11.4": "DEV-007",
    "Rule 11.5": "DEV-009",
    "Rule 20.5": "DEV-012",
}
DISAPPLIED = {"Rule 15.5": "Disapplied by MISRA C:2025 (GRP section 2)"}

rows = []
with open(GRP) as fh:
    for line in fh:
        m = re.match(r"^\|\s*(Dir|Rule)\s+(\d+\.\d+)\s*\|\s*([A-Za-z]+)\s*\|\s*([A-Za-z]+)\s*\|\s*([^|]*)\|\s*(.*)\|\s*$", line)
        if not m:
            continue
        kind, num, misra_cat, proj_cat, checkers, title = m.groups()
        key = kind + " " + num
        rows.append((key, misra_cat, proj_cat, checkers.strip(), title.strip()))

in_scope_findings = []
try:
    with open(SARIF) as fh:
        data = json.load(fh)
    for r in data["runs"][0].get("results", []):
        locs = r.get("locations") or [{}]
        uri = (locs[0].get("physicalLocation") or {}).get("artifactLocation", {}).get("uri", "")
        uri = (uri or "").replace("\\", "/")
        if "/ports/loongarch64/" in uri or "/bsp/" in uri:
            in_scope_findings.append((r.get("ruleId", "?"), uri))
except FileNotFoundError:
    print("WARNING: SARIF not found, completeness check skipped", file=sys.stderr)

# Completeness: any finding in scope must belong to a deviated guideline.
def guideline_of(rule_id):
    # MISRAC2025-RULE_5_10-a / MISRAC2025-DIR_4_6-a -> "Rule 5.10"/"Dir 4.6"
    m = re.match(r"MISRAC2025-(RULE|DIR)_(\d+)_(\d+)", rule_id or "")
    if not m:
        return None
    kind = "Rule" if m.group(1) == "RULE" else "Dir"
    return kind + " " + m.group(2) + "." + m.group(3)

uncovered = []
for rid, uri in in_scope_findings:
    g = guideline_of(rid)
    if g not in DEVIATED:
        uncovered.append((rid, g, uri))

print("# Guideline Compliance Summary - MISRA C:2025")
print()
print("| | |")
print("|---|---|")
print("| Project | Eclipse ThreadX port for LoongArch64 (ESL) |")
print("| Standard | MISRA C:2025, compliance per MISRA Compliance:2020 |")
print("| Scope | `ports/loongarch64/` and `bsp/` C sources and headers (GEP section 2) |")
print("| Analysis | Parasoft C/C++test Standard 2026.1.3, configuration under version control (`tools/parasoft/ESL_MISRA_C_2025.properties`) |")
print("| Report | [product/report.sarif](product/report.sarif), [product/report.html](product/report.html) |")
print("| Baseline | Release candidate 1.0.0-rc1 (tag `v1.0.0-rc1`) |")
print()
print("## 1. Result for the compliance scope")
print()
n_compliant = 0
n_dev = 0
n_dis = 0
for key, misra_cat, proj_cat, checkers, title in rows:
    if key in DEVIATED:
        n_dev += 1
    elif key in DISAPPLIED:
        n_dis += 1
    else:
        n_compliant += 1
print("All {} guidelines in the GRP were evaluated.  {} are compliant in the"
      .format(len(rows), n_compliant) + " compliance scope without deviation, {} are complied with".format(n_dev))
print("through approved-track deviation records, and {} is disapplied by the GRP.".format(n_dis if n_dis else "none"))
print()
if uncovered:
    print("**WARNING - findings in scope not covered by a deviation record:**")
    for rid, g, uri in uncovered:
        print("- {} ({}) at {}".format(rid, g, uri))
    print()
else:
    print("The product-scope analysis reports no violation in `ports/loongarch64/` or")
    print("`bsp/` that is not covered by a deviation record.")
    print()
print("Findings under `external/threadx/` ({} in the product-scope report) are the"
      .format(len(data["runs"][0].get("results", [])) - len(in_scope_findings)))
print("input to the separate kernel gap analysis (M7); the upstream test sources in")
print("the `tests/` report are analysed but reported separately (GEP scope table).")
print()
print("## 2. Deviations")
print()
print("| Guideline | Category | Records |")
print("|---|---|---|")
for key, misra_cat, proj_cat, checkers, title in rows:
    if key in DEVIATED:
        print("| {} ({}) | {} | {} |".format(key + " " + title, misra_cat, misra_cat, DEVIATED[key]))
print()
print("All deviation records are in [deviations/](../deviations/); their approval")
print("status is recorded in each record.  The compliance claim of this baseline")
print("becomes effective when the record approvals are signed (GEP section 5.3).")
print()
print("## 3. Guideline-by-guideline result")
print()
print("| Guideline | MISRA category | Project category | Checkers | Result |")
print("|---|---|---|---|---|")
for key, misra_cat, proj_cat, checkers, title in rows:
    if key in DISAPPLIED:
        result = "Disapplied (" + DISAPPLIED[key] + ")"
    elif key in DEVIATED:
        result = "Deviations (" + DEVIATED[key] + ")"
    else:
        result = "Compliant"
    print("| {} | {} | {} | {} | {} |".format(key, misra_cat, proj_cat, checkers or "-", result))
