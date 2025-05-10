// LEDcalc.c
// RGB lines are P1_4/P1_5/P1_3 or CEX1/CEX2/CEX0

#include "LEDcalc.h"
#include "pwm.h"
#include "delay.h"
#include <stdlib.h>
			
#define OneColorTimeLength 5000
#define TransitionSpeed 20
#define TransitionSteps 200
#define INTENSITY_STEP 5
#define INTENSITY_MIN 2

#define MODE_OFF 0
#define MODE_WHITE 1
#define MODE_RGB 2

#define NumOfColors 7

code unsigned char RGBcolors[NumOfColors][3]={{235, 29, 139},
											  {187, 29, 235},
											  {29, 60, 235},
											  {29, 235, 77},
											  {247, 219, 10},
											  {247, 109, 10},
											  {255, 255, 255}};

/*
code unsigned char RGBcolors[NumOfColors][3]={{255, 0, 0},
											  {0, 255, 0},
											  {0, 0, 255},
											  {255, 255, 0},
											  {255, 0, 255},
											  {0, 255, 255},
											  {255, 255, 255}};
*/

unsigned char Intensity;
unsigned char mode;

unsigned char led_color_counter;
unsigned int led_timer_counter;
bit led_color_change_flag;
bit led_color_transition_flag;
unsigned int led_color_transition_counter;
bit led_start_trans;
bit led_color_trans_step;

unsigned char OldRcomp, OldGcomp, OldBcomp;
unsigned char NewRcomp, NewGcomp, NewBcomp;
unsigned char CurRcomp, CurGcomp, CurBcomp;
char Rstep, Gstep, Bstep;
double R_transstep, G_transstep, B_transstep;
unsigned int StepState;

unsigned char RGBcolorsNormalized[NumOfColors][3];

void NormalizeRGB (unsigned char* R,  unsigned char* G,  unsigned char* B);
void GetNewColor (unsigned char* R, unsigned char* G, unsigned char* B);
void SetIntensity (unsigned char* R, unsigned char* G, unsigned char* B, unsigned char Intens);
unsigned int random(unsigned int upperbound);

void InitLEDcalc (void)
{
	unsigned char i;

	for (i = 0; i < NumOfColors; i++)
	{
		RGBcolorsNormalized[i][0] = RGBcolors[i][0];
		RGBcolorsNormalized[i][1] = RGBcolors[i][1];
		RGBcolorsNormalized[i][2] = RGBcolors[i][2];
		NormalizeRGB(&RGBcolorsNormalized[i][0], &RGBcolorsNormalized[i][1], &RGBcolorsNormalized[i][2]);
	}

	CurRcomp = RGBcolors[0][0];
	CurGcomp = RGBcolors[0][1];
	CurBcomp = RGBcolors[0][2];
	NewRcomp = RGBcolors[0][0];
	NewGcomp = RGBcolors[0][1];
	NewBcomp = RGBcolors[0][2];

	InitPWM();

	led_color_counter = 0;
	led_timer_counter = 0;
	led_color_change_flag = 0;
	led_color_transition_flag = 0;
	led_color_transition_counter = 0;
	led_start_trans = 0;
	led_color_trans_step = 0;

	Intensity = 50;
	mode = MODE_WHITE;
	srand(0);


	SetLightWHITE (255);
	G_delay_ms(250);
	SetLightWHITE (0);
	SetLightRGB (255, 0, 0);
	G_delay_ms(250);
	SetLightRGB (0, 255, 0);
	G_delay_ms(250);
	SetLightRGB (0, 0, 255);
	G_delay_ms(250);
	SetLightRGB (0, 0, 0);

	TasterModePressed();

	return;
}

void SetLightWHITE (unsigned char Intensity)
{
	SetPWMline3_HighLen(255 - Intensity);
	return;
}

void SetLightRGB (unsigned char Rcomp, unsigned char Gcomp, unsigned char Bcomp)
{	// RGB lines are P1_4/P1_5/P1_3 or CEX1/CEX2/CEX0

	SetIntensity (&Rcomp, &Gcomp, &Bcomp, Intensity);
	SetPWMline0_HighLen(255 - Bcomp);
	SetPWMline1_HighLen(255 - Rcomp);
	SetPWMline2_HighLen(255 - Gcomp);
	return;
}

void LEDcalc_main (void)
{
	rand();
	
	if (mode == MODE_RGB)
	{
		if (led_start_trans)
		{
			OldRcomp = NewRcomp;
			OldGcomp = NewGcomp;
			OldBcomp = NewBcomp;
	
			GetNewColor (&NewRcomp, &NewGcomp, &NewBcomp);
	
			StepState = 0;
			R_transstep = ((double)NewRcomp - (double)OldRcomp) / (double)TransitionSteps;
			G_transstep = ((double)NewGcomp - (double)OldGcomp) / (double)TransitionSteps;
			B_transstep = ((double)NewBcomp - (double)OldBcomp) / (double)TransitionSteps;
	
			led_start_trans = 0;
		}
		
		if (led_color_trans_step)
		{
			if (StepState < TransitionSteps)
			{
				StepState++;
	
				CurRcomp = (unsigned char)((double)OldRcomp + (double)StepState * R_transstep);
				CurGcomp = (unsigned char)((double)OldGcomp + (double)StepState * G_transstep);
				CurBcomp = (unsigned char)((double)OldBcomp + (double)StepState * B_transstep);
			}
			else
			{
				led_color_transition_flag = 0;
				led_timer_counter = 0;
				CurRcomp = NewRcomp;
				CurGcomp = NewGcomp;
				CurBcomp = NewBcomp;
			}
	//			SetLightRGB (NewRcomp, NewGcomp, NewBcomp);
			SetLightRGB (CurRcomp, CurGcomp, CurBcomp);
	//			SetLightRGB (RGBcolorsNormalized[led_color_counter][0], RGBcolorsNormalized[led_color_counter][1], RGBcolorsNormalized[led_color_counter][2]);
	
			led_color_trans_step = 0;
		}
	}
	return;
}

// 1ms Call function
void LEDcalc_1ms (void)
{
	if (mode == MODE_RGB)
	{
		led_timer_counter++;
		if ((led_color_transition_flag == 0) && (led_timer_counter >= OneColorTimeLength))
		{
			led_timer_counter = 0;
			led_start_trans = 1;
			led_color_transition_flag = 1;
		}
	
		if (led_color_transition_flag)
		{
			led_color_transition_counter++;
			if (led_color_transition_counter > TransitionSpeed)
			{
				led_color_transition_counter = 0;
				led_color_trans_step = 1;
			}
		}
	}
	return;
}

void NormalizeRGB (unsigned char* R,  unsigned char* G,  unsigned char* B)
{
	unsigned char RGBmax;
	double sf;

	RGBmax = *R;
	if (RGBmax < *G)
		RGBmax = *G;
	if (RGBmax < *B)
		RGBmax = *B;

	sf = 255.0 / (double)RGBmax;
	*R = (unsigned char)((double)(*R) * sf);
	*G = (unsigned char)((double)(*G) * sf);
	*B = (unsigned char)((double)(*B) * sf);

	return;
}

void SetIntensity (unsigned char* R, unsigned char* G, unsigned char* B, unsigned char Intens)
{
	double sf;
	sf = (double)Intens / (double)100;
	*R = (unsigned char)((double)(*R) * sf);
	*G = (unsigned char)((double)(*G) * sf);
	*B = (unsigned char)((double)(*B) * sf);

	return;
}

void TasterHigherIntensityPressed (void)
{
	if (Intensity <= 100 - 2*INTENSITY_STEP)
	{
		if (Intensity <= 50)
			Intensity += INTENSITY_STEP;
		else
			Intensity += 2 * INTENSITY_STEP;
	}
	else
		Intensity = 100;

	if (mode == MODE_WHITE)
		SetLightWHITE (Intensity);
	else if (mode == MODE_RGB)
		SetLightRGB (CurRcomp, CurGcomp, CurBcomp);

	return;
}

void TasterLowerIntensityPressed (void)
{
	if (Intensity >= INTENSITY_STEP + INTENSITY_MIN)
	{
		if (Intensity >= 60)
			Intensity -= 2 * INTENSITY_STEP;
		else
			Intensity -= INTENSITY_STEP;
	}
	else
		Intensity = INTENSITY_MIN;


	if (mode == MODE_WHITE)
		SetLightWHITE (Intensity);
	else if (mode == MODE_RGB)
		SetLightRGB (CurRcomp, CurGcomp, CurBcomp);

	return;
}

void TasterModePressed (void)
{
	if (mode == MODE_OFF)
	{
		mode = MODE_WHITE;
		SetLightRGB (0, 0, 0);
		SetLightWHITE (Intensity);
	}
	else if (mode == MODE_WHITE)
	{
		mode = MODE_RGB;
		GetNewColor (&NewRcomp, &NewGcomp, &NewBcomp);
		SetLightWHITE (0);
		SetLightRGB (NewRcomp, NewGcomp, NewBcomp);
	}
	else if (mode == MODE_RGB)
	{
		mode = MODE_OFF;
		SetLightRGB (0, 0, 0);
		SetLightWHITE (0);
	}

	return;
}

unsigned int random(unsigned int upperbound)
{
	return ((unsigned long)rand() * upperbound) >> 15;
}


//a f-ja proverava da li je boja neka sarena (return 1) ili je bliska beloj (return 0)
bit CheckColorIsNice(unsigned char* R, unsigned char* G, unsigned char* B)
{
	unsigned char Cmin, Cmax;

	Cmin = *R;
	if (Cmin > *G)
		Cmin = *G;
	if (Cmin > *B)
		Cmin = *B;
	Cmax = *R;
	if (Cmax < *G)
		Cmax = *G;
	if (Cmax < *B)
		Cmax = *B;

	if ((Cmax - Cmin) > 200)
		return 1;
	else
		return 0;
}

void GetNewColor (unsigned char* R, unsigned char* G, unsigned char* B)
{// Ovde postoje dva nacina izbora boja, iz predefinisanog niza i nasumicno

	bit newcolor = 0;
// Izbor boja NASUMICNO
	while (!newcolor)
	{
		*R = (unsigned char)random(256);
		*G = (unsigned char)random(256);
		*B = (unsigned char)random(256);
		NormalizeRGB(R, G, B);
		newcolor = CheckColorIsNice(R, G, B);
	}



// Izbor boja iz predefinisanog niza
/*	led_color_counter++;
	if (led_color_counter == NumOfColors)
		led_color_counter = 0;
	*R = RGBcolorsNormalized[led_color_counter][0];
	*G = RGBcolorsNormalized[led_color_counter][1];
	*B = RGBcolorsNormalized[led_color_counter][2];
*/

	return;
}

