// wdt.c
#include <at89c51xd2.H>
#include <wdt.h>
#include "konstante.h"

// WDT is 14-bit counter which increments every machine cycle (1/12 of oscilator freq)
// 16383 clocks has to pass without reseting WDT for it to reset MCU
// 			WDT_interval = 2^(14 + prescaler) / (Fosc / 12)  sekundi
// at Fosc = 18.432MHz		WDT_interval = 1.365 sec

unsigned int xdata EWDT_ResetTimer;

void init_wdt (void)
{

	WDTPRG = 7;			// WDT_interval = 2.09s na 12MHz

 	WDTRST = 0x1E;
 	WDTRST = 0xE1;

	EWDT_ResetTimer = 0;	// Brojac za resetovanje EWDTa
	EWDT_RESET_LINE_SIG_LED = 0;	// External WDT Reset PIN (low as default)
}

void ResetWDT(void)
{

 	WDTRST = 0x1E;
 	WDTRST = 0xE1;

	EWDT_RESET_LINE_SIG_LED = !EWDT_RESET_LINE_SIG_LED;
}

/*
void AutoResetMCU (void)
{
	WDTPRG = 0;			// WDT_interval je minimalni

 	WDTRST = 0x1E;
 	WDTRST = 0xE1;

	while (1);
}
*/
