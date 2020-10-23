#include <stdint.h>

struct gdt_pointer{
	uint16_t limit;
	uint64_t addr;
}__attribute__((packed));
typedef struct gdt_pointer gdt_pointer_t;

struct gdt_entry{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t flags;
	uint8_t base_high;
}__attribute__((packed));
typedef struct gdt_entry gdt_entry_t;

struct tss_entry{
	uint16_t limit_low;
        uint16_t base_low;
        uint8_t base_mid;
        uint8_t access;
        uint8_t flags;
        uint8_t base_high;
	uint32_t base_higher;
	uint32_t resv;
}__attribute__((packed));

typedef struct tss_entry tss_entry_t;

struct gdt{
	gdt_entry_t entries[5];
	tss_entry_t tss;
	gdt_pointer_t pointer;
}__attribute__((packed));
typedef struct gdt gdt_t;

struct tss{
	uint32_t resv1;
	uint64_t rsp0;
	uint64_t rsp1;
	uint64_t rsp2;
	uint64_t resv2;
	uint64_t ist1;
	uint64_t ist2;
	uint64_t ist3;
	uint64_t ist4;
	uint64_t ist5;
	uint64_t ist6;
	uint64_t ist7;
	uint64_t resv3;
	uint16_t resv4;
	uint16_t iopb_offset;
}__attribute__((packed));

typedef struct tss tss_t;

void init_tss();
