ENTRY(start);

. = 0x80200000;

SECTIONS {
    .text : ALIGN(4K) {
        PROVIDE(text_start = .);
       *(.init);
       *(.text);
       . = ALIGN(4);
        PROVIDE(text_end = .);
    }
   .bss : ALIGN(4K) {
   		PROVIDE(bss_start = .);
   		*(.bss);
   		. += 4096;
   		PROVIDE(stack_top = .);
   		. += 4096;
   		PROVIDE(global_pointer = .);
   		PROVIDE(bss_end = .);
   	}
    .rodata : ALIGN(4K) {
        PROVIDE(rodata_start = .);
       *(.rodata);
        PROVIDE(rodata_end = .);
    }
    .data : ALIGN(4K) {
        PROVIDE(data_start = .);
       *(.data);
        PROVIDE(data_end = .);
    }
    PROVIDE(end = .);
}