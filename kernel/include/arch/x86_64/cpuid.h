#include <stdint.h>

struct cpuid{
	uint64_t rax,rbx,rcx,rdx;
}__attribute__((packed));
typedef struct cpuid cpuid_t;

struct cpu_model_info{
	int reserved:4;
	int extFamily:8;
	int extModel:4;
	int reserved2:4;
	int family:4;
	int model:4;
	int step:4;
}__attribute__((packed));
typedef struct cpu_model_info cpu_model_info_t;
struct cpu_hardware_info{
	uint8_t localApicId;
	uint8_t logicalProcessorCount;
	uint8_t clFlush;
	uint8_t brandId;
}__attribute__((packed));
typedef struct cpu_hardware_info cpu_hardware_info_t;
struct cpu_instructions_info{
	int guest:1;
	int rdrand:1;
	int halfPrecision:1;
	int avx:1;
	int xsaveEnabled:1;
	int xsaveSupported:1;
	int aes:1;
	int reserved:1;
	int popcnt:1;
	int movbe:1;
	int reserved2:1;
	int sse42:1;
	int sse41:1;
	int reserved3:5;
	int cmpxchg16b:1;
	int fma:1;
	int reserved4:2;
	int ssse3:1;
	int reserved5:5;
	int monitor:1;
	int reserved6:1;
	int pclmulqdq:1;
	int sse3:1;
}__attribute__((packed));
typedef struct cpu_instructions_info cpu_instructions_info_t;
struct cpu_instructions2_info{
	int reserved:3;
	int hyperthreading:1;
	int reserved2:1;
	int sse2:1;
	int sse:1;
	int fxsr:1;
	int mmx:1;
	int reserved3:3;
	int clFlush:1;
	int reserved4:1;
	int pse36:1;
	int pat:1;
	int cmov:1;
	int mca:1;
	int pge:1;
	int mtrr:1;
	int syscalls:1;
	int reserved5:1;
	int apic:1;
	int cmpxchg8b:1;
	int mce:1;
	int pae:1;
	int msr:1;
	int rsc:1;
	int pse:1;
	int de:1;
	int vme:1;
	int fpu:1;
}__attribute__((packed));
typedef struct cpu_instructions2_info cpu_instructions2_info_t;

struct cpu_info{
	cpu_model_info_t model_info;
	cpu_hardware_info_t hardware_info;
	cpu_instructions_info_t instructions_info;
	cpu_instructions2_info_t instructions2_info;
}__attribute__((packed));
typedef struct cpu_info cpu_info_t;

extern void call_cpuid(uint64_t rax, uint64_t retStruct);

char* getCPUManufacturer();
cpu_info_t* getCPUInfo();
