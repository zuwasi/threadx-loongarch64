# CMake toolchain file: Eclipse ThreadX on LoongArch64, GNU toolchain.
#
# Bare-metal build with the Linux-targeted GNU cross compiler. The compiler is
# used freestanding: no C library, no start files, own linker script.
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR loongarch64)

set(THREADX_ARCH "loongarch64")
set(THREADX_TOOLCHAIN "gnu")

set(LOONGARCH64_PREFIX "loongarch64-linux-gnu-" CACHE STRING "Cross toolchain prefix")
set(LOONGARCH64_GCC_SUFFIX "-14" CACHE STRING "Suffix of the versioned gcc driver")

set(CMAKE_C_COMPILER   ${LOONGARCH64_PREFIX}gcc${LOONGARCH64_GCC_SUFFIX})
set(CMAKE_ASM_COMPILER ${LOONGARCH64_PREFIX}gcc${LOONGARCH64_GCC_SUFFIX})
set(CMAKE_AR           ${LOONGARCH64_PREFIX}ar)
set(CMAKE_RANLIB       ${LOONGARCH64_PREFIX}ranlib)
set(CMAKE_OBJCOPY      ${LOONGARCH64_PREFIX}objcopy)
set(CMAKE_OBJDUMP      ${LOONGARCH64_PREFIX}objdump)
set(CMAKE_SIZE         ${LOONGARCH64_PREFIX}size)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# -msimd=none: the port saves the 64-bit FP register file only, so the compiler
# must not emit LSX/LASX code that would use the wider vector registers.
# -mstrict-align: device registers and early-boot memory must not see
# unaligned accesses.
set(LA64_ARCH_FLAGS "-march=loongarch64 -mabi=lp64d -msimd=none -mcmodel=normal -mstrict-align")
# -nostdinc: no C library headers at all, only GCC's own freestanding headers
# (stddef.h, stdint.h, ...) plus the port's libc/include directory. This keeps
# host or target glibc headers out of the build.
execute_process(
    COMMAND ${CMAKE_C_COMPILER} -print-file-name=include
    OUTPUT_VARIABLE LA64_GCC_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(LA64_COMMON_FLAGS "-ffreestanding -nostdinc -isystem ${LA64_GCC_INCLUDE_DIR} -fno-common -fno-pic -fno-pie -ffunction-sections -fdata-sections -g")

set(CMAKE_C_FLAGS   "-std=c99 ${LA64_ARCH_FLAGS} ${LA64_COMMON_FLAGS}" CACHE INTERNAL "C flags")
set(CMAKE_ASM_FLAGS "${LA64_ARCH_FLAGS} ${LA64_COMMON_FLAGS} -D__ASSEMBLER__" CACHE INTERNAL "ASM flags")
set(CMAKE_EXE_LINKER_FLAGS "${LA64_ARCH_FLAGS} -nostdlib -nostartfiles -static -no-pie -Wl,--gc-sections -Wl,--build-id=none" CACHE INTERNAL "Linker flags")

set(CMAKE_C_FLAGS_DEBUG     "-O0" CACHE INTERNAL "")
set(CMAKE_ASM_FLAGS_DEBUG   ""    CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE   "-O2" CACHE INTERNAL "")
set(CMAKE_ASM_FLAGS_RELEASE ""    CACHE INTERNAL "")
