// pwm.c
// Pulse Width Modulator has 5 lines P1.3 to P1.7, labeled as CEX0 to CEX4
// RGB lines are P1_4/P1_5/P1_3 or CEX1/CEX2/CEX0
// White is P1_6 or CEX3


#include <at89c51xd2.H>
#include "pwm.h"
#include "timer.h"
//#include "wdt.h"
#include "konstante.h"

void InitPWM (void)
{
	CR = 0;	// Stop PWM Counter
	init_timer0();

	CMOD &= ~(1 << 0);		// Cleared PWM interrupt on Clock Overflow

	CMOD &= ~(1 << 1);		// Timer0 overflow selected as PWM clock
	CMOD |= (1 << 2);		// Timer0 overflow selected as PWM clock
//	CMOD &= ~(1 << 1);		// Fperiph / 6 selected as PWM clock
//	CMOD &= ~(1 << 2);		// Fperiph / 6 selected as PWM clock

	CMOD &= ~(1 << 6);		// WDTE Disabled
	CMOD &= ~(1 << 7);		// PWM continues function in Idle mode

	CCAPM0 = 0x42;			// CEX0 is set to be PWM
	CCAPM1 = 0x42;			// CEX1 is set to be PWM
	CCAPM2 = 0x42;			// CEX2 is set to be PWM
	CCAPM3 = 0x42;			// CEX3 is set to be PWM

	CCAP0H = 0;				// Inicijalna vrednost za duzinu LOW dela signala. Ovako signal ostale uvek HIGH, kao u resetu
	CCAP1H = 0;				// Inicijalna vrednost za duzinu LOW dela signala. Ovako signal ostale uvek HIGH, kao u resetu
	CCAP2H = 0;				// Inicijalna vrednost za duzinu LOW dela signala. Ovako signal ostale uvek HIGH, kao u resetu
	CCAP3H = 0;				// Inicijalna vrednost za duzinu LOW dela signala. Ovako signal ostale uvek HIGH, kao u resetu
	
	CR = 1;	// Start PWM Counter

	return;
}

void SetPWMline0_HighLen (unsigned char value)
{
	CCAP0H = 255 - value;
	return;
}

void SetPWMline1_HighLen (unsigned char value)
{
	CCAP1H = 255 - value;
	return;
}

void SetPWMline2_HighLen (unsigned char value)
{
	CCAP2H = 255 - value;
	return;
}

void SetPWMline3_HighLen (unsigned char value)
{
	CCAP3H = 255 - value;
	return;
}
