#include <at89c51xd2.H>
#include <intrins.h>
#include <stdlib.h>
#include "timer.h"
#include "delay.h"
#include "wdt.h"
#include "pwm.h"
#include "uart.h"
#include "at_PC_com.h"
#include "LEDcalc.h"
#include "konstante.h"


//#define DEBUG_COLOR
//#define DEBUG_PWM
//#define DEBUG_SHADES
//#define DEBUG_NORMALISATION
//#define DEBUG_WHITE_INTENSITY


#ifdef DEBUG_COLOR
	unsigned char debug_color_counter = 0;
	code unsigned char debug_RGBcolors[7][3]={{235, 29, 139},
											  {187, 29, 235},
											  {29, 60, 235},
											  {29, 235, 77},
											  {247, 219, 10},
											  {247, 109, 10},
											  {255, 255, 255}};
#endif
#ifdef DEBUG_SHADES
	unsigned int debug_shades_counter = 0;
#endif
#ifdef DEBUG_NORMALISATION
	code unsigned char debug_normalisation_RGBcolors[7][3]={{235, 29, 139},
														  {187, 29, 235},
														  {29, 60, 235},
														  {29, 235, 77},
														  {247, 219, 10},
														  {247, 109, 10},
														  {255, 255, 255}};
	unsigned char debug_normalisation_rn, debug_normalisation_gn, debug_normalisation_bn;
	unsigned char debug_normalisation_i;
#endif
#ifdef DEBUG_WHITE_INTENSITY
	unsigned int debug_white_intensity_i;
#endif


#define TASTER_SETTLE_TIME 2
unsigned char idata TASTERI_OldState;	// tasteri 0 do 2
unsigned char idata tmp, tmpTS, i;
bit T0_pressed;
bit T1_pressed;
bit T2_pressed;

// ****************************************************************************
// ****      Initialisation      *****
// ****************************************************************************
void init()
{
	AUXR &= ~(1 << 1);	// access XRAM ONLY
	AUXR &= ~(1 << 2);	// XRAM is 1792 bytes
	AUXR &= ~(1 << 3);	// XRAM is 1792 bytes
	AUXR |= (1 << 4);	// XRAM is 1792 bytes

	TASTERI = 0xFF;	// Input lines
	TASTERI_OldState = 0xFF;	// nepritisnut nijedan taster ima za rezultat da je stanje na liniji 1
	T0_pressed = 0;
	T1_pressed = 0;
	T2_pressed = 0;

	init_timer1 ();
	InitCOM ();
	init_wdt ();
	InitLEDcalc ();
	G_delay_ms(20);
}



// ****************************************************************************
// ****      M A I N      *****
// ****************************************************************************
void main(void)
{
	unsigned char idata i;

	init();
	ResetWDT();

	data_buf[0] = 0xFF;		  		// ATMEL just RESETED message
	SendDataToPC (data_buf, 1);

#ifdef DEBUG_PWM
	SetPWMline2_HighLen (0);
	G_delay_s (2);
	SetPWMline2_HighLen (63);
	G_delay_s (2);
	SetPWMline2_HighLen (127);
	G_delay_s (2);
	SetPWMline2_HighLen (192);
	G_delay_s (2);
	SetPWMline2_HighLen (255);
#endif
#ifdef DEBUG_SHADES
while (1)
	for (debug_shades_counter = 0; debug_shades_counter <= 255; debug_shades_counter++)
	{
		SetPWMline2_HighLen ((unsigned char)debug_shades_counter);
		G_delay_ms(10);
	}
#endif
#ifdef DEBUG_NORMALISATION
	for (debug_normalisation_i = 0; debug_normalisation_i < 7; debug_normalisation_i++)
	{
		NormalizeRGB (debug_normalisation_RGBcolors[debug_normalisation_i][0], debug_normalisation_RGBcolors[debug_normalisation_i][1], debug_normalisation_RGBcolors[debug_normalisation_i][2],
					   &debug_normalisation_rn,  &debug_normalisation_gn, &debug_normalisation_bn);
		SetLightRGB (debug_normalisation_RGBcolors[debug_normalisation_i][0], debug_normalisation_RGBcolors[debug_normalisation_i][1], debug_normalisation_RGBcolors[debug_normalisation_i][2]);
		G_delay_s (2);
		SetLightRGB (debug_normalisation_rn, debug_normalisation_gn, debug_normalisation_bn);
		G_delay_s (2);
	}
#endif
#ifdef DEBUG_WHITE_INTENSITY
	for (debug_white_intensity_i = 255; debug_white_intensity_i > 0; debug_white_intensity_i--)
	{
		SetLightWHITE (debug_white_intensity_i);
		G_delay_ms(5);
	}
	SetLightWHITE (0);
#endif




	// ***********************************************************************************
	// *****************     M A I N      L O O P     ************************************
	// ***********************************************************************************
	while (1)
	{				
																		 
		LEDcalc_main ();


	// *************************************************************
	// OCITAVANJE PROMENE STANJA TASTERA: Tasteri 0 do 1
		if (TASTERI_OldState != TASTERI)
		{
			G_delay_ms (TASTER_SETTLE_TIME);
			if ((tmpTS = TASTERI) != TASTERI_OldState)
			{
				tmp = TASTERI_OldState ^ tmpTS;	// sada se u tmp nalazi jedinica samo na mestima tastera cije je stanje promenjeno

				if ((tmp & (1 << 0)) && (!(tmp & tmpTS & (1 << 0))))	// ako stanje pina tastera cije je stanje promenjeno nije logicka "1"
					T0_pressed = 1;
				if ((tmp & (1 << 1)) && (!(tmp & tmpTS & (1 << 1))))	// ako stanje pina tastera cije je stanje promenjeno nije logicka "1"
					T1_pressed = 1;
				if ((tmp & (1 << 2)) && (!(tmp & tmpTS & (1 << 2))))	// ako stanje pina tastera cije je stanje promenjeno nije logicka "1"
					T2_pressed = 1;

				TASTERI_OldState = tmpTS;
			}
		}

		if (T0_pressed)
		{
			T0_pressed = 0;
			TasterModePressed();
//			SetLightWHITE (255);
//			G_delay_ms(50);
//			SetLightWHITE (0);
		}
		if (T1_pressed)
		{
			T1_pressed = 0;
			TasterHigherIntensityPressed ();
//			SetLightWHITE (255);
//			G_delay_ms(100);
//			SetLightWHITE (0);
		}
		if (T2_pressed)
		{
			T2_pressed = 0;
			TasterLowerIntensityPressed ();
//			SetLightWHITE (255);
//			G_delay_ms(200);
//			SetLightWHITE (0);
		}




	// *************************************************************
	// Resetovanje EWDT-a i BLINKANJE diode
		if (EWDT_ResetTimer >= EWDT_RESET_INTERVAL)
		{
			EWDT_ResetTimer = 0;
			ResetWDT();

#ifdef DEBUG_COLOR
	debug_color_counter++;
	if (debug_color_counter == 7)
		debug_color_counter = 0;
	SetLightRGB (debug_RGBcolors[debug_color_counter][0], debug_RGBcolors[debug_color_counter][1], debug_RGBcolors[debug_color_counter][2]);
#endif
		}

	// *************************************************************
	// Parsiranje poruka od PC-ja i izvrsavanje akcija
	    if (RxBufTempNotEmpty)
	    {
//	    	Disable_UART;
			if (RxBufFree >= RxBufTempInP)
			{
		    	for (i = 0; i < RxBufTempInP; i++)
		    	{
			        RxBuf[RxBufInP++] = RxBufTemp[i];
			        if (RxBufInP == RX_BUF_SIZE)
			          RxBufInP = 0;
			        RxBufFree--;
			        RxParseFlag = 1;
		    	}
		    	RxBufTempInP = 0;
		    	RxBufTempNotEmpty = 0;
			}
//	    	Enable_UART;	
	    }

		if (RxParseFlag)	// Nesto je stiglo i treba da se isparsira
		{
			RxParseFlag = MsgFromPCParser();
		}
 	}	// za while (1)

}	// za main
