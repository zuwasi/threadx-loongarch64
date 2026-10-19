# ThreadX for LoongArch64

An [Eclipse ThreadX](https://github.com/eclipse-threadx/threadx) port for the
LoongArch64 (LA64) architecture, with a board support package for the QEMU
`virt` machine.

- C code written to **MISRA C:2025**, verified with Parasoft C/C++test
- Developed following an **IEC 61508-3** software process (Systematic Capability 3)
- Validated with the ThreadX regression suite on QEMU

> Status: in development. See [docs/project-plan.md](docs/project-plan.md).

## Getting the source

```sh
git clone --recurse-submodules https://github.com/zuwasi/threadx-loongarch64.git
```

ThreadX is included as a submodule pinned to `v6.5.2.202603_rel`.

## Prerequisites (Ubuntu 24.04)

```sh
sudo apt install gcc-14-loongarch64-linux-gnu binutils-loongarch64-linux-gnu \
                 qemu-system-misc cmake ninja-build
```
