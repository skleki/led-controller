// *****************************************************************************************
// *******************              RGB Controller Code              ***********************
// *****************************************************************************************

#include <math.h>

#define PWM_RANGE 5000	// Default is 1023
#define PWM_FREQ 100	// Default is 1kHz
#define INTENSITY_RANGE 100


typedef enum LedsMode {
  StaticLeds,
  RgbRandomSlow,
  RgbRandomFast,
  Mode3,
  Mode4
};

// Current state variables
LedsMode CurrentLedsMode = StaticLeds;
int CurrentWhiteIntensity = 100;
bool CurrentWhiteIsOn = true;
int CurrentRed = 255;
int CurrentGreen = 255;
int CurrentBlue = 255;
int CurrentRGBIntensity = 100;
bool CurrentRGBIsOn = true;
String CurrentRGBMode = RGB_MODE_RANDOM_SLOW;

inline void RGBW_Initialise(void)
{
  pinMode(RGB_POWER_LINE, INPUT);
  digitalWrite(RGB_POWER_LINE, LOW);
  pinMode(WHITE_POWER_LINE, INPUT);
  digitalWrite(WHITE_POWER_LINE, LOW);

  PwmForRGB_Initialise();
  pinMode(RED_LINE, OUTPUT);
  pinMode(GREEN_LINE, OUTPUT);
  pinMode(BLUE_LINE, OUTPUT);
  pinMode(WHITE_LINE, OUTPUT);
  SetWhite(100, CurrentWhiteIsOn);
  SetRGB(255, 255, 255, 100, true, CurrentRGBIsOn);

  CurrentLedsMode = StaticLeds;

  RandomRGB_Initialise();
}

inline void RGBW_1000msHandler()
{
  RandomRGB_1000msHandler();
}

inline void RGBW_1msHandler()
{
  RandomRGB_1msHandler();
}

inline void RGBW_MainLoop()
{
  RandomRGB_MainLoop();
}

void SetWhite(int intensity, bool isOn)
{
  Serialprint("Set WHITE intensity: ");
  Serialprintln(intensity);
  if (intensity < 0) intensity = 0;
  if (intensity > INTENSITY_RANGE) intensity = INTENSITY_RANGE;
  CurrentWhiteIntensity = intensity;
  PwmForLED_SetLineDutyCycle(WHITE_LINE, isOn ? intensity : 0);
}

void SetRGB(int red, int green, int blue, int intensity, bool normalize, bool isOn)
{
  if (red == 0 && green == 0 && blue == 0)
  {
    red = 255;
    green = 255;
    blue = 255;
  }
  
//  Serialprint("Set RGB(");
//  Serialprint(red);
//  Serialprint(", ");
//  Serialprint(green);
//  Serialprint(", ");
//  Serialprint(blue);
//  Serialprint("), intensity: ");
//  Serialprintln(intensity);
  CurrentRed = red;
  CurrentGreen = green;
  CurrentBlue = blue;
  CurrentRGBIntensity = intensity;
  if (intensity < 0) intensity = 0;
  if (intensity > INTENSITY_RANGE) intensity = INTENSITY_RANGE;

  if (normalize)
  {
    NormalizeRGB(&red, &green, &blue);
  }
  double redAsDouble = red;
  double greenAsDouble = green;
  double blueAsDouble = blue;
  ConvertNormalizedRGBcolorToPercentageValues(&redAsDouble, &greenAsDouble, &blueAsDouble, intensity);
  PwmForLED_SetLineDutyCycle(RED_LINE, isOn ? redAsDouble : 0);
  PwmForLED_SetLineDutyCycle(GREEN_LINE, isOn ? greenAsDouble : 0);
  PwmForLED_SetLineDutyCycle(BLUE_LINE, isOn ? blueAsDouble : 0);
}



// PRIVATE METHODS

//#define RGB_POWER_LINE D0
//#define WHITE_POWER_LINE D1
bool GetPowerLine(int line)
{
  return digitalRead(line);  
}

void PwmForRGB_Initialise(void)
{
	analogWriteRange(PWM_RANGE);
	analogWriteFreq(PWM_FREQ);
}

void PwmForLED_SetLineDutyCycle(int line, double percentage)
{
	if (percentage < 0.0) percentage = 0.0;
	if (percentage > 100.0) percentage = 100.0;

  // This is choosing power based on linear function but this will not lead to human eye perception of brightness lineary
  // because human eye has logarithmic function of brightness perception based on light power.
  // int dutyCycleValue = (long)(PWM_RANGE * (100.0 - percentage) / 100.0);

  // We need to calculate light power based on requested perception level.
  // To do this we can use function y = base ^ (100 / Log-base(101)) - 1 where we choose base so that we get apropriate function.
  // base is here chosen to 1.3 for best results
  // Log calculator: https://www.symbolab.com/solver/logarithmic-equation-calculator/%5Clog_%7B1.3%7D%5Cleft(101%5Cright)
  // Log of 101 for base 1.3 is 17.59050.
  // 100 / 17.59050 = 5.6848867286319320087547255620932
  // so
  //  double base = 1.3;
  //  double powerPerentage = pow(base, percentage / 5.6848867286) - 1;

  // or 
  // Log of 101 for base 1.2 is 25.31308.
  // 100 / 25.31308 = 3.9505267632386102362889067628278
  // so
  //  double base = 1.2;
  //  double powerPerentage = pow(base, percentage / 3.95052676) - 1;

  // or 
  // Log of 101 for base 1.4 is 13.71620.
  // 100 / 25.31308 = 7.2906490135751884632770009186218
  // so
  double base = 1.4;
  double powerPerentage = pow(base, percentage / 7.290649) - 1;


  Serialprint("Perception %:  ");
  Serialprint(percentage);
  Serialprint(", Power %: ");
  Serialprintln(powerPerentage);
  if (powerPerentage > 100.0)
  {
    powerPerentage = 100.0;
  }


  int dutyCycleValue = (long)(PWM_RANGE * (100.0 - powerPerentage) / 100.0);
	analogWrite((int)line, dutyCycleValue);
  Serialprint("Set line:  ");
  Serialprint(line);
  Serialprint(", PWD: ");
  Serialprintln(PWM_RANGE - dutyCycleValue);
}

inline int GetRandomRGBcomponent()
{
	return random(0, 256);
}

void GetRandomRGB_Normalized(int* red, int* green, int* blue)
{
  for (int i = 0; i < 200; i++)
  {
    *red = GetRandomRGBcomponent();
    *green = GetRandomRGBcomponent();
    *blue = GetRandomRGBcomponent();
    NormalizeRGB(red, green, blue);
    if (IsColorColorful(*red, *green, *blue))
    {
      return;
    }
  }
}

void NormalizeRGB(int* red, int* green, int* blue)
{
	double multiplier = (double)255.0 / (double)*red;
	multiplier = min(multiplier, (double)255.0 / (double)*green);
	multiplier = min(multiplier, (double)255.0 / (double)*blue);
	
	*red = *red * multiplier;
	*green = *green * multiplier;
	*blue = *blue * multiplier;
}

void ConvertNormalizedRGBcolorToPercentageValues(double* red, double* green, double* blue, int intensity)
{
  if (intensity < 0) intensity = 0;
  if (intensity > INTENSITY_RANGE) intensity = INTENSITY_RANGE;
  double multiplier = (double)intensity / 2.55 / (double)INTENSITY_RANGE;
  
  *red = *red * multiplier;
  *green = *green * multiplier;
  *blue = *blue * multiplier;

//  Serialprint("NormalizedRGBcolorToPercentageValues RGB(");
//  Serialprint(*red);
//  Serialprint(", ");
//  Serialprint(*green);
//  Serialprint(", ");
//  Serialprint(*blue);
//  Serialprint("), intensity: ");
//  Serialprintln(intensity);
}

bool IsColorColorful(int red, int green, int blue)
{
	int disatnce = abs(red - green);
	disatnce += abs(red - blue);
	disatnce += abs(green - blue);
	
	if (disatnce > 250)
		return true;
	return false;
}








void ParseLEDSHttpRequest(EthernetClient* client, HttpRequest* request)
{
  //declare name and value to use the request parameters and cookies
  char name[HTTP_REQ_PARAM_NAME_LENGTH], value[HTTP_REQ_PARAM_VALUE_LENGTH]; 

  client->println("HTTP/1.1 200 OK");
  client->println("Content-Type: text/html");
  client->println("Connnection: close");
  client->println();
  client->println("<!DOCTYPE HTML>");
  client->println("<html>");
  client->println("<body>");

  //access object properties
  client->print("Method: ");
  client->print(request->method);
  client->println("<br>");
  client->print("Uri: ");
  client->print(request->uri);
  client->println("<br>");
  client->print("Version: ");
  client->print(request->version);
  client->println("<br>");
  client->print("paramCount: ");
  client->print(request->paramCount);
  client->println("<br>");
  //list received parameters GET and POST
  client->println("Parameters:<br>");
  for(int i=1;i<=request->paramCount;i++){
    request->getParam(i,name,value);
    client->print(name);
    client->print("-");
    client->print(value);
    client->println("<br>");
  }
  //list received cookies
  client->println("Cookies:<br>");
  for(int i=1;i<=request->cookieCount;i++)
  {
    request->getCookie(i,name,value);
    client->print(name);
    client->print(" - ");
    client->print(value);
    client->println("<br>");
  }
  client->println("</body>");
  client->print("</html>");
}
