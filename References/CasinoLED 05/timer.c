// Timer

//#include <reg8253.h>
#include <at89c51xd2.H>
#include "timer.h"
#include "wdt.h"
#include "LEDcalc.h"
#include "at_pc_com.h"
#include "konstante.h"

/*
void init_timer0 (void)
{
// Ovo je standardna inicijalizacija timera u MOD1 gde je on 16bitni brojac i na svaki
// overflow se izvrsava interrupt rutina
//	PT0 = 1;		// Timer0 is HIGH PRIORITY
	EA = 0;			// Disable All Interrupts

	TR0=0;			// Stop Timer0
	ET0=0;			// Disable Timer0 Interrupt

	TMOD |= (1 << 0);	// Timer0 is in TimerMode 1, internal clocking
	TMOD &= ~(1 << 1);	// Timer0 is in TimerMode 1, internal clocking

	TH0 = 250;		// Timer0 is in 16-bit counter mode and with TL0 = 0 makes 1ms interrupts
					// Fclk = 18432000 Hz
					// Ft0 = Fclk / 12 * (2^16 - 0xFA00) = 1000 Hz

//	TH0 = 0xF8;		// Fclk = 24000000
//	TL0 = 0x30;		// Ft0 = Fclk / 12 * (2^16 - 0xF830) = 1000 Hz



	ET0 = 1;		// Enable Timer0 Interrupt
	TR0=1;			// Start Timer0

	EA = 1;			// Enable All Interrupts

	return;
}
*/
void init_timer0 (void)
{
// Ovo je inicijalizacija timera u MOD2 gde je on 8bitni brojac sa AutoReloadom
// Na overflow se NE izvrsava interrupt rutina
//	PT0 = 1;		// Timer0 is HIGH PRIORITY
	EA = 0;			// Disable All Interrupts

	TR0=0;			// Stop Timer0
	ET0=0;			// Disable Timer0 Interrupt

	TMOD &= ~(1 << 0);	// Timer0 is in TimerMode 2, internal clocking
	TMOD |= (1 << 1);	// Timer0 is in TimerMode 2, internal clocking

  	TH0 = 196;		// Timer0 is in 8-bit counter mode with Ft0 = 
					// Fclk = 18432000 Hz
					// Ft0 = Fclk / 12 * (2^8 - TL0)

//	TH0 = 250;		// Timer0 is in 16-bit counter mode and with TL0 = 0 makes 1ms interrupts
					// Fclk = 18432000 Hz
					// Ft0 = Fclk / 12 * (2^16 - 0xFA00) = 1000 Hz

//	TH0 = 0xF8;		// Fclk = 24000000
//	TL0 = 0x30;		// Ft0 = Fclk / 12 * (2^16 - 0xF830) = 1000 Hz

	TR0=1;			// Start Timer0

	EA = 1;			// Enable All Interrupts

	return;
}


void init_timer1 (void)
// Ovo je standardna inicijalizacija timera u MOD1 gde je on 16bitni brojac i na svaki
// overflow se izvrsava interrupt rutina
{
//	PT0 = 1;		// Timer0 is HIGH PRIORITY
	EA = 0;			// Disable All Interrupts

	TR1=0;			// Stop Timer0
	ET1=0;			// Disable Timer0 Interrupt

	TMOD |= (1 << (0 + 4));	// Timer0 is in TimerMode 1, internal clocking
	TMOD &= ~(1 << (1 + 4));	// Timer0 is in TimerMode 1, internal clocking

	TH1 = 250;		// Timer0 is in 16-bit counter mode and with TL0 = 0 makes 1ms interrupts
					// Fclk = 18432000 Hz
					// Ft0 = Fclk / 12 * (2^16 - 0xFA00) = 1000 Hz

//	TH1 = 0xF8;		// Fclk = 24000000
//	TL1 = 0x30;		// Ft0 = Fclk / 12 * (2^16 - 0xF830) = 1000 Hz

	ET1 = 1;		// Enable Timer0 Interrupt
	TR1=1;			// Start Timer0

	EA = 1;			// Enable All Interrupts

	return;
}


/******************************************************************************/
/*       timerint:  timer0 interrupt                                          */
/******************************************************************************/
timer0int() interrupt 1 using 2 		// Timer0 is interrupt 1
{
	TH0 = 0xFA;			// Timer0 is in 16-bit counter mode and with TL0 = 0 makes 1ms interrupts
						// Fclk = 18432000 Hz
						// Ft0 = Fclk / 12 * (2^16 - 0xFA00) = 1000 Hz

//	TL0 = 0x30;		// Ft0 = Fclk / 12 * (2^16 - 0xF830) = 1000 Hz
//	TH0 = 0xF8;		// Fclk = 24000000

//	EWDT_ResetTimer++;	// Tacnije je da je to nesto malo sporije tj okida na nesto malo vise od 1 ms
	return;
}

/******************************************************************************/
/*       timerint:  timer0 interrupt                                          */
/******************************************************************************/
timer1int() interrupt 3 using 2 		// Timer1 is interrupt 3
{
	TH1 = 0xFA;			// Timer0 is in 16-bit counter mode and with TL0 = 0 makes 1ms interrupts
						// Fclk = 18432000 Hz
						// Ft0 = Fclk / 12 * (2^16 - 0xFA00) = 1000 Hz

//	TL1 = 0x30;		// Ft0 = Fclk / 12 * (2^16 - 0xF830) = 1000 Hz
//	TH1 = 0xF8;		// Fclk = 24000000

	EWDT_ResetTimer++;	// Tacnije je da je to nesto malo sporije tj okida na nesto malo vise od 1 ms
	LEDcalc_1ms();
	return;
}
