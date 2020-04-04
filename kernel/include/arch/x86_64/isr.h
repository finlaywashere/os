#include <stdint.h>

struct idt_entry{
   uint16_t   baseLow;
   uint16_t   selector;
   uint8_t   reservedIst;
   uint8_t   flags;
   uint16_t   baseMid;
   uint32_t   baseHigh;
   uint32_t   reserved;
} __attribute__((packed));

typedef struct idt_entry idt_entry_t;

struct idt_ptr{
	uint16_t limit;
	uint64_t base;
}__attribute__((packed));

typedef struct idt_ptr idt_ptr_t;

struct registers{
    uint64_t ds;
    uint64_t rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;
    uint64_t intNo, errCode;
    uint64_t rip, cs, eflags, useresp, ss;
}__attribute__((packed));
typedef struct registers registers_t;

void init_idt();
void set_idt(uint8_t, uint64_t, uint16_t, uint8_t);
void isr_handler(registers_t);

extern void load_idt();
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
