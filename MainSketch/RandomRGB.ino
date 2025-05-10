// *****************************************************************************************
// *******************              RGB Controller Code              ***********************
// *****************************************************************************************

bool RandomRGB_IsRunning = false;
int RandomRGB_Intensity;
int RandomRGB_StartingRed;
int RandomRGB_StartingGreen;
int RandomRGB_StartingBlue;
int RandomRGB_EndingRed;
int RandomRGB_EndingGreen;
int RandomRGB_EndingBlue;
int RandomRGB_StepCounter;
int RandomRGB_StepChangeTimer;
bool RandomRGB_StepChangeFlag = false;

#define STEPS_NUMBER 100
#define SLOW_STEP_INTERVAL 15
#define FAST_STEP_INTERVAL 1
int RandomRGB_StepChangeInterval;

void RandomRGB_Start(bool slow, int intensity)
{
  RandomRGB_IsRunning = true;
  if (slow)
    RandomRGB_StepChangeInterval = SLOW_STEP_INTERVAL;
  else
    RandomRGB_StepChangeInterval = FAST_STEP_INTERVAL;

  RandomRGB_Intensity = intensity;
  RandomRGB_StartingRed = 184;
  RandomRGB_StartingGreen = 179;
  RandomRGB_StartingBlue = 255;
  GetRandomRGB_Normalized(&RandomRGB_EndingRed, &RandomRGB_EndingGreen, &RandomRGB_EndingBlue);
  RandomRGB_StepCounter = 0;
  RandomRGB_StepChangeTimer = 0;
  RandomRGB_StepChangeFlag = false;

  SetRGB(RandomRGB_StartingRed, RandomRGB_StartingGreen, RandomRGB_StartingBlue, RandomRGB_Intensity, false, CurrentRGBIsOn);
}

void RandomRGB_Stop(void)
{
  RandomRGB_IsRunning = false;
}

inline void RandomRGB_Initialise(void)
{
}

inline void RandomRGB_1000msHandler()
{
  if (RandomRGB_IsRunning)
  {
  }
}

inline void RandomRGB_1msHandler()
{
  if (RandomRGB_IsRunning)
  {
    RandomRGB_StepChangeTimer++;
    if (RandomRGB_StepChangeTimer > RandomRGB_StepChangeInterval)
    {
      RandomRGB_StepChangeTimer = 0;
      RandomRGB_StepChangeFlag = true;
    }
  }
}

inline void RandomRGB_MainLoop()
{
  if (RandomRGB_IsRunning)
  {
    if (RandomRGB_StepChangeFlag)
    {
      RandomRGB_StepChangeFlag = false;
      RandomRGB_StepCounter++;

      int newRed = RqndomRGB_GetColorValueForStep(RandomRGB_StartingRed, RandomRGB_EndingRed, RandomRGB_StepCounter);
      int newGreen = RqndomRGB_GetColorValueForStep(RandomRGB_StartingGreen, RandomRGB_EndingGreen, RandomRGB_StepCounter);
      int newBlue = RqndomRGB_GetColorValueForStep(RandomRGB_StartingBlue, RandomRGB_EndingBlue, RandomRGB_StepCounter);
      SetRGB(newRed, newGreen, newBlue, RandomRGB_Intensity, false, CurrentRGBIsOn);

      if (RandomRGB_StepCounter >= STEPS_NUMBER)
      {
        RandomRGB_StepCounter = 0;
        RandomRGB_StartingRed = RandomRGB_EndingRed;
        RandomRGB_StartingGreen = RandomRGB_EndingGreen;
        RandomRGB_StartingBlue = RandomRGB_EndingBlue;
        GetRandomRGB_Normalized(&RandomRGB_EndingRed, &RandomRGB_EndingGreen, &RandomRGB_EndingBlue);
      }
    }
  }
}

int RqndomRGB_GetColorValueForStep(int startColor, int endColor, int step)
{
  double stepIncrement = ((double)endColor - (double)startColor) / STEPS_NUMBER;
  return (int)(startColor + stepIncrement * step);  
}
