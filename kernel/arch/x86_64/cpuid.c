#include <arch/x86_64/cpuid.h>
#include <kernel/vmm.h>

char* getCPUManufacturer(){
	cpuid_t* id = kmalloc_p(sizeof(cpuid_t));
        call_cpuid(0,&id);
	char* name = kmalloc_p(sizeof(char)*13);
	name[0] = (char) (id->rbx);
	name[1] = (char) ((id->rbx) >> 8);
	name[2] = (char) ((id->rbx) >> 16);
        name[3] = (char) ((id->rbx) >> 24);
	name[4] = (char) (id->rdx);
        name[5] = (char) ((id->rdx) >> 8);
        name[6] = (char) ((id->rdx) >> 16);
        name[7] = (char) ((id->rdx) >> 24);
	name[8] = (char) (id->rcx);
        name[9] = (char) ((id->rcx) >> 8);
        name[10] = (char) ((id->rcx) >> 16);
        name[11] = (char) ((id->rcx) >> 24);
	name[12] = 0x0;
	return name;
}
cpu_info_t* getCPUInfo(){
	cpuid_t* id = kmalloc_p(sizeof(cpuid_t));
        call_cpuid(1,&id);
	return (cpu_info_t*)id;
}
