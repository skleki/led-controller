#include <intrins.h>
#include "delay.h"
#include "wdt.h"

/*
void G_delay_s (unsigned char len) // za OSC Frequency - 18.432 MHz
{
	unsigned int i;
	for (i = 0; i < (len << 2); i++)
	{
		G_delay_ms (249);
	}
}
*/

void G_delay_ms (unsigned int len) // za OSC Frequency - 18.432 MHz
{
	unsigned int i, j;
	for (i = 0; i < len; i++)
	{
		for (j = 0; j < 60; j++);	
		for (j = 0; j < 252; j++);	
		ResetWDT();
	}
}

/*
void G_Kas_11us(int mikros)   //jedno traje 11.1979 mikro sekunde za OSC Frequency - 18.432 MHz
{
	int i;
	for (i=0;i<mikros;i++)
		_nop_();
}
*/

/*
void G_Kas_2us()        	// kasnjenje od 2us sa sve pozivom za OSC Frequency - 18.432 MHz
{
	_nop_();
	_nop_();
	_nop_();
}
*/

/*
void G_Kas_5us(unsigned char mikros)     //jedno traje 5.24735 mikro sekunde za OSC Frequency - 18.432 MHz
{
	unsigned char i;
	for (i=0;i<mikros;i++);
}
*/
