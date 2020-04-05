#include <kernel/timer.h>
#include <arch/x86_64/pit.h>

void init_timer(){
	init_pit();
}
extern uint64_t ticks;

#define TPMS FREQUENCY/60000

void sleep(uint64_t ms){
	uint64_t destTicks = ticks+TPMS*ms;

	while(ticks < destTicks){}
	return;
}
