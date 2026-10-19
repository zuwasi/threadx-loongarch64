/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  Linker script, QEMU loongarch64 "virt" (low RAM 0x0-0x0FFFFFFF).
    The image is placed at 2 MiB, run in direct address mode.  */

OUTPUT_ARCH(loongarch)
ENTRY(_start)

__system_stack_size = 0x4000;

MEMORY
{
    RAM (rwx) : ORIGIN = 0x00200000, LENGTH = 0x0FE00000
}

PHDRS
{
    text PT_LOAD FLAGS(5);   /* R X */
    data PT_LOAD FLAGS(6);   /* R W */
}

SECTIONS
{
    .text :
    {
        KEEP(*(.text.start))
        KEEP(*(.text.trap .text.trap.*))
        *(.text .text.*)
    } > RAM :text

    .rodata : ALIGN(8)
    {
        *(.rodata .rodata.*)
    } > RAM :text

    .data : ALIGN(4096)
    {
        *(.data .data.*)
        *(.sdata .sdata.*)
    } > RAM :data

    .bss (NOLOAD) : ALIGN(8)
    {
        __bss_start = .;
        *(.sbss .sbss.*)
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end = .;
    } > RAM :data

    .stack (NOLOAD) : ALIGN(16)
    {
        . += __system_stack_size;
        __system_stack_top = .;
    } > RAM :data

    /* First free memory, handed to ThreadX by tx_application_define.  */
    . = ALIGN(16);
    __free_memory_start = .;

    /DISCARD/ : { *(.note.*) *(.comment) *(.eh_frame*) }
}
