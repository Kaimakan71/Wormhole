OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(kernelMain)

PHDRS
{
    text   PT_LOAD FLAGS((1 << 2) | (1 << 0)); /* Read + Execute */
    data   PT_LOAD FLAGS((1 << 2) | (1 << 1)); /* Read + Write */
    rodata PT_LOAD FLAGS((1 << 2));            /* Read-only */
}

SECTIONS
{
    . = 0xffffffff80000000;

    .text : ALIGN(CONSTANT(MAXPAGESIZE))
    {
        *(.text .text.*)
    } :text

    .rodata : ALIGN(CONSTANT(MAXPAGESIZE))
    {
        *(.rodata .rodata.*)
    } :rodata

    .data : ALIGN(CONSTANT(MAXPAGESIZE))
    {
        *(.data)
    } :data

    .bss : ALIGN(CONSTANT(MAXPAGESIZE))
    {
        *(COMMON)
        *(.bss .bss.*)
    } :data

    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
        *(.note .note.*)
    }
}
