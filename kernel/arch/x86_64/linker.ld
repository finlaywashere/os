ENTRY(_start)

SECTIONS{
	. = 0x100000;
	_kernel_low_start = .;
	_ro_start = .;
	.low_text BLOCK(4K) : ALIGN(4K){
		*(.multiboot)
		*(.low_text)
	}
	.low_rodata BLOCK(4K) : ALIGN(4K){
                *(.low_rodata);
        }
	_ro_end = .;
	.low_data BLOCK(4K) : ALIGN(4K){
		*(.low_data);
	}
	.low_bss BLOCK(4K) : ALIGN(4K){
		*(.low_bss);
	}
	_kernel_low_end = .;
	. += 0xFFFFFFFF80000000;
	_kernel_virtual_start = .;
	_ro2_start = .;
	.text ALIGN (4K) : AT (ADDR (.text) - 0xFFFFFFFF80000000){
		*(.text)
	}
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xFFFFFFFF80000000){
		*(.rodata)
	}
	_ro2_end = .;
	.data ALIGN (4K) : AT (ADDR (.data) - 0xFFFFFFFF80000000){
		*(.data)
	}
	.bss ALIGN (4K) : AT (ADDR (.bss) - 0xFFFFFFFF80000000){
		*(COMMON)
		*(.bss)
	}
	_kernel_virtual_end = .;
}
