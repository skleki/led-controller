// *****************************************************************************************
// ************       Use #define directives to enable hardware support      ***************

#define ESP8266

#define UNO_BOARD_LEDS
#define LED_PIN D8
#define LED_ON HIGH
#define LED_OFF LOW

//// TEST values
//#define RGB_POWER_LINE D2
//#define WHITE_POWER_LINE D1
//#define RED_LINE D7
//#define GREEN_LINE D6
//#define BLUE_LINE D5
//#define WHITE_LINE D0

#define WHITE_POWER_LINE D0
#define RGB_POWER_LINE D1
#define WHITE_LINE D2
#define BLUE_LINE D5
#define GREEN_LINE D6
#define RED_LINE D7

#define RS232_COMMS

#define RGB_MODE_RANDOM_SLOW "random_slow"
#define RGB_MODE_RANDOM_FAST "random_fast"
#define RGB_MODE_MODE3 "mode_3"
#define RGB_MODE_MODE4 "mode_4"



#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <Ethernet.h> 
#include "HttpRequest.h" 

// *****************************************************************************************
// **************                 Serial logging control code                ***************
// *****************************************************************************************
#define Serialbegin2A(baud,config) Serial.begin(baud,config)
#define Serialbegin(baud) Serial.begin(baud)

//#define RS232_PROTOCOL

#if defined(RS232_PROTOCOL)
bool isRS232 = true;
  #define Serialprint2A(x,y) yield()
  #define Serialprint(x) yield()
  #define Serialprintln2A(x,y) yield()
  #define Serialprintln(x) yield()
  #define ForceSerialprint2A(x,y) yield()
  #define ForceSerialprint(x) yield()
  #define ForceSerialprintln2A(x,y) yield()
  #define ForceSerialprintln(x) yield()
#else
bool isRS232 = false;
  #define Serialprint2A(x,y) Serial.print(x,y)
  #define Serialprint(x) Serial.print(x)
  #define Serialprintln2A(x,y) Serial.println(x,y)
  #define Serialprintln(x) Serial.println(x)
  #define ForceSerialprint2A(x,y) Serial.print(x,y)
  #define ForceSerialprint(x) Serial.print(x)
  #define ForceSerialprintln2A(x,y) Serial.println(x,y)
  #define ForceSerialprintln(x) Serial.println(x)
#endif
// *****************************************************************************************
// *****************************************************************************************


// *****************************************************************************************
// ************               Globaly defined Comms message codes            ***************
#define CommsRPi2UnoEnum_PingToUno 0
#define CommsRPi2UnoEnum_ResponseToUnoPing 1
#define CommsRPi2UnoEnum_GetEepromData 2
#define CommsRPi2UnoEnum_SetEepromData 3
#define CommsRPi2UnoEnum_ResetEepromData 4
#define CommsRPi2UnoEnum_GetEepromWiFiData 5
#define CommsRPi2UnoEnum_SetEepromWiFiData 6
#define CommsRPi2UnoEnum_ResetUno 7
#define CommsRPi2UnoEnum_GetCommsSignalStrength 0xD2

#define CommsUno2RPi_ResponseToRPiPing 0
#define CommsUno2RPi_PingToRPi 1
#define CommsUno2RPi_GetEepromDataResponse 2
#define CommsUno2RPi_SetEepromDataResponse 3
#define CommsUno2RPi_ResetEepromDataResponse 4
#define CommsUno2RPi_GetEepromWiFiDataResponse 5
#define CommsUno2RPi_SetEepromWiFiDataResponse 6
#define CommsUno2RPi_UnoCameOutOfReset 7
#define CommsUno2RPi_CommsSignalStrength 0xD2

#define RX_BUF_SIZE 600
#define TX_BUF_SIZE 600
#define MaxRxMsgLen 250
#define MaxTxMsgLen 250 
#define MAX_TX_CHUNK_SIZE (500 + 25)    // za Fast UDP Comms, 25 je za header



// *****************************************************************************************
// **************               Main sketch, Setup() and Loop()              ***************
// *****************************************************************************************

extern "C" {
#include "user_interface.h"
} 


void SetSendMessageFunction(int (*funct)(byte *data, int len));

// *****************************************************************************************
// ******************          S E T U P        ********************************************
void setup() 
{
  long MCUSR_BKP;

  RGBW_Initialise();
  
  LoadEEPROMData();
  Signal_LEDs_Initialise();
  
  if (isRS232)
  {
    RxTxBuffer_Initialize();
    RS232Comms_Initialize();
    ServerComms_Initialize();
  }
  else
  {
	  Serialbegin(115200);
	  ForceSerialprintln("Starting Arduino !!!");
    Serialprint("EEData data verification: ");
	  Serialprintln(EEData_IsCRCOK());
	  Serialprintln("");
	  Serialprintln("EEData data on Initialisation: ");
	  PrintEEPROMData();
  }

  MyWiFi_Initialise();

  Timer1_Initialize();
  wdt_enable(WDTO_8S);

  if (isRS232)
  {
    const rst_info* resetInfo = system_get_rst_info();
    //consolePort.print(F("system_get_rst_info() reset reason: "));
    //consolePort.println(RST_REASONS[resetInfo->reason]);   
    UnoCameOutOfReset(resetInfo->reason);
  }

  Serialprintln("setup() FINISHED.\n");
}
// *****************************************************************************************
// *****************************************************************************************

// *****************************************************************************************
// ****************           M A I N    L O O P         ***********************************
void loop() 
{
	wdt_reset();
	yield();
	Timer1MailLoop();
	yield();

  if (isRS232)
  {
  	RS232Comms_MailLoop();
  	yield();
  	ServerComms_MailLoop();
  	yield();
  	RxTxBuffer_MailLoop();
  	yield();
  }
  
	MyWiFi_Loop();
	yield();
	RGBW_MainLoop();
}
// *****************************************************************************************
// *****************************************************************************************




// *****************************************************************************************
// ****************           T I M E R    H A N D L E R S        **************************
inline void Timer1_1msHandler(void)
{
	RGBW_1msHandler();
}

inline void Timer1_10msHandler(void)
{
  Signal_LEDs_10msHandler();
}

inline void Timer1_100msHandler(void)
{
}

inline void Timer1_1000msHandler(void)
{
	RGBW_1000msHandler();
}

inline void ResetUno(void)
{
  wdt_enable(WDTO_15MS); // turn on the WatchDog and don't stroke it.
  for(;;);
}

// *****************************************************************************************
// *****************************************************************************************
