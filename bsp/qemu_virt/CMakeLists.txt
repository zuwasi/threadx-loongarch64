# BSP: QEMU loongarch64 "virt" machine
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT

add_library(bsp_qemu_virt STATIC
    src/startup.S
    src/bsp_cpu.S
    src/uart.c
    src/intr.c
    src/timer.c
    src/power.c
    src/fatal.c
)

target_include_directories(bsp_qemu_virt PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc)
target_compile_options(bsp_qemu_virt PRIVATE $<$<COMPILE_LANGUAGE:C>:${LA64_STRICT_WARNINGS}>)

set(BSP_QEMU_VIRT_LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/link.ld CACHE INTERNAL "")

# la64_add_qemu_executable(<target> <sources>...)
# Builds a bare-metal ELF for QEMU virt with the BSP startup and linker script.
function(la64_add_qemu_executable target)
    add_executable(${target} ${ARGN})
    target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C>:${LA64_STRICT_WARNINGS}>)
    target_link_options(${target} PRIVATE
        -T${BSP_QEMU_VIRT_LINKER_SCRIPT}
        -Wl,--undefined=_start
        -Wl,-Map=$<TARGET_FILE:${target}>.map
    )
    target_link_libraries(${target} PRIVATE
        -Wl,--start-group bsp_qemu_virt threadx -Wl,--end-group
    )
    set_target_properties(${target} PROPERTIES SUFFIX ".elf" LINK_DEPENDS ${BSP_QEMU_VIRT_LINKER_SCRIPT})
endfunction()
