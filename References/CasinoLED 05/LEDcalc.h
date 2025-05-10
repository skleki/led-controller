// LEDcalc.h

extern unsigned char Intensity;
extern code unsigned char RGBcolors[7][3];

extern void InitLEDcalc (void);

extern void SetLightRGB (unsigned char Rcomp, unsigned char Gcomp, unsigned char Bcomp);
void SetLightWHITE (unsigned char Intensity);
extern void LEDcalc_main (void);
extern void LEDcalc_1ms (void);
extern void NormalizeRGB (unsigned char* R,  unsigned char* G,  unsigned char* B);

extern void TasterHigherIntensityPressed (void);
extern void TasterLowerIntensityPressed (void);
extern void TasterModePressed (void);
