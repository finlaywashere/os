#include <arch/x86_64/mbr.h>
mbr_table_t* init_mbr(){
	return (mbr_table_t*) 0xffff800000007c00;
}
