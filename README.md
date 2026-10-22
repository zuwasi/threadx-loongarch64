# ThreadX for LoongArch64

An [Eclipse ThreadX](https://github.com/eclipse-threadx/threadx) port for the
LoongArch64 (LA64) architecture, with a board support package for the QEMU
`virt` machine.

- C code written to **MISRA C:2025**, verified with Parasoft C/C++test
- Developed following an **IEC 61508-3** software process (Systematic
  Capability 3), producing evidence for a future certification effort
- Validated on QEMU with the upstream ThreadX regression suite
  (96 tests x 6 configurations, 576/576), six bring-up test images and an
  eight-thread demo

> **Status: 1.0.0-rc1, pre-release.** All automated verification passes.
> Approvals, independent reviews, full structural coverage (statement,
> branch, MC/DC) and real-hardware validation are still open; see the
> [approval package](docs/approval-package.md). This release is not
> certified and makes no safety-certification claim.

Documentation is published at
<https://zuwasi.github.io/threadx-loongarch64/> (start with the
[documentation index](docs/index.md)).

## Quick start

Ubuntu 24.04 prerequisites:

```sh
sudo apt install gcc-14-loongarch64-linux-gnu binutils-loongarch64-linux-gnu \
                 qemu-system-misc cmake ninja-build
```

Build and run the bring-up tests and the demo on QEMU:

```sh
git clone --recurse-submodules https://github.com/zuwasi/threadx-loongarch64.git
cd threadx-loongarch64
tools/build.sh Debug
ctest --test-dir build/debug --output-on-failure     # 7/7
tools/build.sh Release
ctest --test-dir build/release --output-on-failure   # 7/7
```

Run the full upstream ThreadX regression suite (96 tests in each of the six
kernel configurations `default`, `optimized`, `disable_notify_callbacks`,
`stack_checking`, `stack_checking_rand_fill`, `trace`):

```sh
tools/regression.sh all     # 576/576; or pass one configuration name
```

Static analysis on a licensed Parasoft C/C++test workstation:

```sh
tools/misra.sh product build/misra         # -> build/misra/report
tools/misra.sh tests   build/misra-tests   # -> build/misra-tests/report
```

ThreadX is included as a submodule pinned to `v6.5.2.202603_rel`; its sources
are never modified.

## Documentation

| Area | Entry points |
|---|---|
| Overview | [Documentation index](docs/index.md) - [project plan](docs/project-plan.md) - [release notes](docs/release-notes.md) |
| Safety (IEC 61508-3) | [Safety plan](docs/safety/safety-plan.md) - [requirements](docs/safety/requirements.md) - [design](docs/safety/design.md) - [safety manual](docs/safety/safety-manual.md) |
| Verification | [Verification report](docs/verification/verification-report.md) - [regression results](docs/verification/regression.md) - [coverage](docs/verification/coverage.md) - [traceability](docs/safety/traceability.md) |
| MISRA C:2025 | [Compliance summary](docs/misra/reports/guideline-compliance-summary.md) - [compliance dashboard](https://zuwasi.github.io/threadx-loongarch64/misra/reports/dashboard.html) - [kernel gap analysis](docs/misra/kernel-gap-analysis.md) |
| Release decisions | [Approval package](docs/approval-package.md) - [review log](docs/reviews/review-log.md) |
| Presentation | [Slide deck: the port, MISRA C:2025 and SIL 3 evidence in four days](https://zuwasi.github.io/threadx-loongarch64/presentation/) ([source](docs/presentation/index.html)) |

## Contributing

Contributions are welcome. Read [CONTRIBUTING.md](CONTRIBUTING.md) first:
commits need a DCO sign-off (`git commit -s`), and `external/threadx` is never
modified. Use [issues](https://github.com/zuwasi/threadx-loongarch64/issues)
for bugs, [Discussions](https://github.com/zuwasi/threadx-loongarch64/discussions)
for questions, and [SECURITY.md](SECURITY.md) for vulnerabilities. This
project follows the [Code of Conduct](CODE_OF_CONDUCT.md).

## License

MIT. Copyright (c) 2026 Engineering Software Lab Israel (ESL).
See [LICENSE](LICENSE) and [NOTICE](NOTICE). Eclipse ThreadX is licensed
separately (MIT).
