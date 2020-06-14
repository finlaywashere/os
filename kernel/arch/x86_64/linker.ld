ENTRY(_start)

SECTIONS{
	. = 0x100000;
	_kernel_low_start = .;
	.low_text BLOCK(4K) : ALIGN(4K){
		*(.multiboot)
		*(.low_text)
	}
	.low_data BLOCK(4K) : ALIGN(4K){
		*(.low_data);
	}
	.low_bss BLOCK(4K) : ALIGN(4K){
		*(.low_bss);
	}
	.low_rodata BLOCK(4K) : ALIGN(4K){
		*(.low_rodata);
	}
	_kernel_low_end = .;
	. = 0xFFFFFFFFC0200000;
	_kernel_virtual_start = .;
	.text ALIGN (4K) : AT (ADDR (.text) - 0xFFFFFFFFC0000000){
		*(.text)
	}
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xFFFFFFFFC0000000){
		*(.rodata)
	}
	.data ALIGN (4K) : AT (ADDR (.data) - 0xFFFFFFFFC0000000){
		*(.data)
	}
	.bss ALIGN (4K) : AT (ADDR (.bss) - 0xFFFFFFFFC0000000){
		*(COMMON)
		*(.bss)
	}
	_kernel_virtual_end = .;
}
