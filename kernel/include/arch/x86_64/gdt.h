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
	uint32_t rsp0_low;
	uint32_t rsp0_high;
	uint32_t rsp1_low;
	uint32_t rsp1_high;
	uint32_t rsp2_low;
	uint32_t rsp2_high;
	uint64_t resv2;
	uint32_t ist1_low;
	uint32_t ist1_high;
	uint32_t ist2_low;
	uint32_t ist2_high;
	uint32_t ist3_low;
        uint32_t ist3_high;
	uint32_t ist4_low;
        uint32_t ist4_high;
	uint32_t ist5_low;
        uint32_t ist5_high;
	uint32_t ist6_low;
        uint32_t ist6_high;
	uint32_t ist7_low;
        uint32_t ist7_high;
	uint64_t resv3;
	uint16_t resv4;
	uint16_t iopb_offset;
}__attribute__((packed));

typedef struct tss tss_t;

void init_tss();
