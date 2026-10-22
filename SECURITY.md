# Security policy

## Supported versions

| Version | Supported |
|---|---|
| 1.0.0-rc1 (`main`) | Yes |

## Reporting a vulnerability

Please report security issues privately. Do not open a public issue.

- Preferred: GitHub [private vulnerability reporting](https://github.com/zuwasi/threadx-loongarch64/security/advisories/new).
- Or email **daniel.l@eswlab.com** with "ThreadX LoongArch64 security" in the subject.

Include the affected version or commit, a description of the issue, and steps
or a test case to reproduce it. We aim to acknowledge reports within five
working days and will keep you informed while a fix is prepared.

## Scope

This policy covers the LoongArch64 port (`ports/loongarch64`), the QEMU `virt`
BSP (`bsp/qemu_virt`), the tools in `tools/` and the published documentation.
Issues in Eclipse ThreadX itself (`external/threadx`) should be reported to the
[Eclipse ThreadX project](https://github.com/eclipse-threadx/threadx/security).

This software is not certified for use in safety-related systems; see the
README and the [safety manual](docs/safety/safety-manual.md).
