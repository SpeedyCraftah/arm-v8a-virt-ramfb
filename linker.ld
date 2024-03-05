/* Where our code will begin execution */
ENTRY(_start)
 
SECTIONS {
    /* I believe in aarch64 the interrupt table resides at address 0x0 until 0xC8, and so it's best not to overwrite it with our code */
    . = 0xC8;
 
    /* Standard binary sections with .text being placed first since our code will begin execution at 0xC8 without a bootloader */
    .text   : { *(.text) }
    .rodata : { *(.rodata) }
    .data   : { *(.data) }
    .bss    : { *(.bss COMMON) }
 
    /* Qemu's Virt platform starts general RAM at 1GB+ and so we have to place our stack there, in this case at 1GB + 8KB */
    STACK_BASE = .;
    . += 0x40000000 + 8192;
    STACK_TOP = .;
}