#pragma once
#include <stdbool.h>

int tick();
void reset_state();
void power_up_state();

unsigned char regx, regy, regsp, regac;
unsigned short regpc;  

/* brk is not a flag or a storage unit. its a status this structure needs to implemented diffrently*/
struct status_register_t{
	bool carry:1; // bit 0
	bool zero:1;
	bool interrupt:1;
	bool decimal:1;
	bool brk:1; // "break" is a reserved word :(
	bool unused:1;
	bool overflow:1;
	bool sign:1; // bit 7
}status_r;
