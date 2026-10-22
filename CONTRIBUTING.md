# Contributing

Thanks for your interest in the ThreadX LoongArch64 port. Bug reports, fixes,
tests, documentation and review comments are all welcome.

## Ground rules

- **Do not modify `external/threadx`.** It is the upstream Eclipse ThreadX
  submodule, pinned to `v6.5.2.202603_rel`. Port-specific code lives in
  `ports/loongarch64/gnu` and `bsp/qemu_virt`.
- **Keep the safety evidence consistent.** This project follows an IEC 61508-3
  process. A change to port or BSP code normally also needs:
  - the requirement tag (`LA-REQ-nnn`) in the code comment, and
    `tools/trace.py --check` still reporting zero gaps;
  - a test, or an update to an existing one;
  - an entry in the relevant document under `docs/` (design, verification,
    deviations) when behaviour changes.
- **MISRA C:2025.** C code in `ports/` and `bsp/` must stay free of MISRA
  findings, or carry a deviation record under `docs/misra/deviations/`.
  Maintainers run Parasoft C/C++test; you do not need a licence to contribute,
  and findings will be reported back on your pull request.
- **No safety claims.** Do not add text that claims certification, approval
  or compliance that has not been formally granted.

## Before you open a pull request

On Ubuntu 24.04 (see the README for packages):

```sh
tools/build.sh Debug   && ctest --test-dir build/debug   --output-on-failure
tools/build.sh Release && ctest --test-dir build/release --output-on-failure
tools/regression.sh all          # full upstream ThreadX regression on QEMU
python3 tools/trace.py --check   # requirement traceability
```

CI runs the same build, QEMU tests and regression on every pull request.

## Developer Certificate of Origin (DCO)

Every commit must be signed off to certify that you wrote it or otherwise have
the right to submit it under the project's MIT licence, as described in the
[Developer Certificate of Origin 1.1](https://developercertificate.org/):

```sh
git commit -s -m "Fix interrupt frame alignment"
```

This adds a line such as:

```
Signed-off-by: Your Name <you@example.com>
```

The name and email must match the commit author. Pull requests with unsigned
commits fail the DCO check; fix them with `git rebase --signoff main` and a
force-push to your branch.

## Reporting bugs and asking questions

- Bugs and feature requests: open an [issue](https://github.com/zuwasi/threadx-loongarch64/issues/new/choose).
- Questions and design discussion: use [Discussions](https://github.com/zuwasi/threadx-loongarch64/discussions).
- Security issues: do **not** open a public issue; see [SECURITY.md](SECURITY.md).

By participating you agree to follow the [Code of Conduct](CODE_OF_CONDUCT.md).
