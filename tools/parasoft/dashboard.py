#!/usr/bin/env python3
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT
"""Generate the MISRA C:2025 compliance dashboard (self-contained HTML).

Inputs: docs/misra/reports/{product,tests}/report.sarif, docs/misra/grp.md,
docs/misra/deviations/DEV-*.md and docs/misra/reports/history.json (scan
history, kept by hand because earlier reports are not retained).

Usage: tools/parasoft/dashboard.py > docs/misra/reports/dashboard.html
"""
import glob
import json
import re
from collections import Counter

R = "docs/misra/reports/"


def guideline(rule_id):
    m = re.match(r"MISRAC2025-(RULE|DIR)_(\d+)_(\d+)", rule_id or "")
    if not m:
        return None
    return ("Rule" if m.group(1) == "RULE" else "Dir") + " " + m.group(2) + "." + m.group(3)


def uri(r):
    loc = (r.get("locations") or [{}])[0].get("physicalLocation") or {}
    return (loc.get("artifactLocation", {}).get("uri", "") or "").replace("\\", "/")


grp = {}
titles = {}
for line in open("docs/misra/grp.md", encoding="utf-8"):
    m = re.match(r"^\|\s*(Dir|Rule)\s+(\d+\.\d+)\s*\|\s*(\w+)\s*\|\s*(\w+)\s*\|\s*[^|]*\|\s*(.*)\|\s*$", line)
    if m:
        key = m.group(1) + " " + m.group(2)
        grp[key] = m.group(4)
        titles[key] = m.group(5).strip()

devs = []
for path in sorted(glob.glob("docs/misra/deviations/DEV-*.md")):
    text = open(path, encoding="utf-8").read()
    g = re.search(r"\|\s*Guideline\s*\|\s*([^|]*)\|", text)
    s = re.search(r"\|\s*Status\s*\|\s*([^|]*)\|", text)
    t = re.search(r"^#\s*(DEV-\d+):\s*(.*)$", text, re.M)
    gl = (g.group(1).strip() if g else "")
    ck = re.search(r"\|\s*Parasoft checkers\s*\|\s*([^|]*)\|", text)
    covered = set(re.findall(r"(?:Rule|Dir)\s+\d+\.\d+", gl))
    covered |= {guideline(c) for c in re.findall(r"MISRAC2025-\w+?-[a-z]", ck.group(1) if ck else "")} - {None}
    devs.append({
        "id": t.group(1) if t else path[-10:-3],
        "title": t.group(2).strip() if t else "",
        "guideline": ", ".join(sorted(covered)) or re.split(r":", gl)[0].strip(),
        "covered": sorted(covered),
        "required": "Required" in gl or "Mandatory" in gl,
        "status": s.group(1).strip() if s else "",
    })

deviated = set()
for d in devs:
    deviated.update(d["covered"])

prod = json.load(open(R + "product/report.sarif", encoding="utf-8"))["runs"][0]
results = prod.get("results", [])
rule_desc = {}
for rule in prod.get("tool", {}).get("driver", {}).get("rules", []):
    rule_desc[rule.get("id")] = (rule.get("shortDescription") or {}).get("text", "")

in_scope = [r for r in results if "/ports/loongarch64/" in uri(r) or "/bsp/" in uri(r)]
kernel = [r for r in results if "/external/threadx/" in uri(r)]

kernel_guidelines = Counter(guideline(r.get("ruleId")) for r in kernel)
kernel_files = Counter(uri(r).split("/")[-1] for r in kernel)

cats = ["Mandatory", "Required", "Advisory"]
active = {c: [k for k, v in grp.items() if v == c] for c in cats}
port_cat = {c: {"total": len(active[c]),
                "deviated": len([k for k in active[c] if k in deviated]),
                "violated": 0} for c in cats}
kern_cat = {c: {"total": len(active[c]),
                "violated": len([k for k in active[c] if k in kernel_guidelines])} for c in cats}

top_rules = []
for g, n in kernel_guidelines.most_common(10):
    top_rules.append({"guideline": g, "count": n, "category": grp.get(g, "?"),
                      "title": titles.get(g, "")})

tests = json.load(open(R + "tests/report.sarif", encoding="utf-8"))["runs"][0].get("results", [])

data = json.load(open(R + "history.json", encoding="utf-8"))
data.update({
    "generated": "2026-10-22",
    "tool": "Parasoft C/C++test Standard 2026.1.3",
    "compiler": "GCC 14.2.0 loongarch64-linux-gnu",
    "guidelines_total": len(grp),
    "guidelines_active": sum(len(v) for v in active.values()),
    "disapplied": len([k for k, v in grp.items() if v == "Disapplied"]),
    "deviated_guidelines": sorted(deviated),
    "in_scope_findings": len(in_scope),
    "product_total": len(results),
    "tests_total": len(tests),
    "kernel_total": len(kernel),
    "kernel_violated": len(kernel_guidelines),
    "port_cat": port_cat,
    "kern_cat": kern_cat,
    "top_rules": top_rules,
    "top_files": [{"file": f, "count": n} for f, n in kernel_files.most_common(8)],
    "devs": devs,
})

html = open(__file__.replace("dashboard.py", "dashboard_template.html"), encoding="utf-8").read()
print(html.replace("__DATA__", json.dumps(data)))
