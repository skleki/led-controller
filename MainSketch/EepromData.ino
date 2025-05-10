// *****************************************************************************************
// **********                           EEPROM data                          ***************
// *****************************************************************************************

#include <EEPROM.h>

#define EEPROM_DATA_REGULAR_OFFSET 0
#define EEPROM_DATA_WIFIAP_OFFSET ((sizeof(struct MyEepromDataRegular) + 3) & (~3))


/*
Data:
  unsigned long MagicNumber;                4 bytes
  unsigned long CommsType;                  4 bytes // Not Used with Fast UDP Comms
  byte RF24_ChAdd0[8];                      8 bytes // Not Used with Fast UDP Comms
  byte RF24_ChAdd1[8];                      8 bytes // Not Used with Fast UDP Comms
  unsigned long RetransmitCount;   4 bytes
  unsigned long PingTimeoutSeconds;         4 bytes
  byte ARDUINO_MACadd[8];                   8 bytes // Not Used with Fast UDP Comms
  byte Server_IP[4];                        4 bytes
  unsigned long Server_UDP_Port;            4 bytes
  byte ARDUINO_SubNetMask[4];               4 bytes // Not Used with Fast UDP Comms
  byte ARDUINO_dfGW[4];                     4 bytes // Not Used with Fast UDP Comms
  byte ARDUINO_DNSServer[4];                4 bytes // Not Used with Fast UDP Comms
  unsigned long LoggingEnabled;             4 bytes
  unsigned long CRC;                        4 bytes
                                            Total: 68 bytes





  unsigned long MagicNumber;                4 bytes
  byte WLAN_SSID[32];  //("NasStancic")     32 bytes
  byte WLAN_PASS[32];  //("lara05102008")   32 bytes
  unsigned long WLAN_SECURITY;              4 bytes
  unsigned long CRC;                        4 bytes
                                            Total: 76 bytes
 */


struct MyEepromDataRegular{
  unsigned long MagicNumber;
  unsigned long CommsType;      // Not Used with Fast UDP Comms
  byte RF24_ChAdd0[8];          // Not Used with Fast UDP Comms
  byte RF24_ChAdd1[8];          // Not Used with Fast UDP Comms
  unsigned long RetransmitCount; 
  unsigned long PingTimeoutSeconds;
  byte ARDUINO_MACadd[8];       // Not Used with Fast UDP Comms //{0x08, 0x00, 0x28, 0x57, 0xAC, 0x96}
  byte Server_IP[4];           //(192.168.88.201)
  unsigned long Server_UDP_Port;   //byte  (30231)
  byte ARDUINO_SubNetMask[4];   // Not Used with Fast UDP Comms //(255, 255, 255, 0)
  byte ARDUINO_dfGW[4];         // Not Used with Fast UDP Comms//(192.168.0.1)
  byte ARDUINO_DNSServer[4];    // Not Used with Fast UDP Comms//(192.168.0.1)
  unsigned long LoggingEnabled;
  unsigned long CRC;
}; 

#if defined(ESP8266) || defined(ESP32)
struct MyEepromDataWiFiAP{
  unsigned long MagicNumber;
  byte WLAN_SSID[32];  //("NasStancic")        // cannot be longer than 32 characters!
  byte WLAN_PASS[32];  //("lara05102008")
  unsigned long WLAN_SECURITY;  /* (#define    WLAN_SEC_UNSEC    (0)
                                  #define    WLAN_SEC_WEP      (1)
                                  #define    WLAN_SEC_WPA      (2)
                                  #define    WLAN_SEC_WPA2     (3) */
  unsigned long CRC;
}; 
#endif


MyEepromDataRegular EEPROMDataRegular;
#if defined(ESP8266) || defined(ESP32)
  MyEepromDataWiFiAP EEPROMDataWiFiAP;
#endif

inline bool LoadEEPROMData()
{
#if defined(ESP8266) || defined(ESP32)
  int i;
  EEPROM.begin(256);
  for (i = 0; i < sizeof(EEPROMDataRegular); i++)
  {
    ((byte*)(&EEPROMDataRegular))[i] = EEPROM.read(EEPROM_DATA_REGULAR_OFFSET + i);
  }
  for (i = 0; i < sizeof(EEPROMDataWiFiAP); i++)
  {
    ((byte*)(&EEPROMDataWiFiAP))[i] = EEPROM.read(EEPROM_DATA_WIFIAP_OFFSET + i);
  }
#else
  EEPROM.get(EEPROM_DATA_REGULAR_OFFSET, EEPROMDataRegular);
#endif


//  EEPROMDataRegular.MagicNumber = 0x55AA;
//  EEPROMDataRegular.CommsType = 0;
//  for (byte i = 0; i < 5; i++)
//  {
//    EEPROMDataRegular.RF24_ChAdd0[i] = 0xA0 + i;
//    EEPROMDataRegular.RF24_ChAdd1[i] = 0xB0 + i;
//  }
//  EEPROMDataRegular.RetransmitCount = 3;
//  EEPROMDataRegular.PingTimeoutSeconds = 2;
//  for (byte i = 0; i < 6; i++)
//  {
//    EEPROMDataRegular.ARDUINO_MACadd[i] = 0xA0 + i;
//  }  
//  EEPROMDataRegular.Server_IP[0] = 192;
//  EEPROMDataRegular.Server_IP[1] = 168;
//  EEPROMDataRegular.Server_IP[2] = 0;
//  EEPROMDataRegular.Server_IP[3] = 202;
//  EEPROMDataRegular.Server_UDP_Port = 30123;
//  EEPROMDataRegular.ARDUINO_SubNetMask[0] = 255;
//  EEPROMDataRegular.ARDUINO_SubNetMask[1] = 255;
//  EEPROMDataRegular.ARDUINO_SubNetMask[2] = 255;
//  EEPROMDataRegular.ARDUINO_SubNetMask[3] = 0;
//  EEPROMDataRegular.ARDUINO_dfGW[0] = 192;
//  EEPROMDataRegular.ARDUINO_dfGW[1] = 168;
//  EEPROMDataRegular.ARDUINO_dfGW[2] = 0;
//  EEPROMDataRegular.ARDUINO_dfGW[3] = 1;
//  EEPROMDataRegular.ARDUINO_DNSServer[0] = 192;
//  EEPROMDataRegular.ARDUINO_DNSServer[1] = 168;
//  EEPROMDataRegular.ARDUINO_DNSServer[2] = 0;
//  EEPROMDataRegular.ARDUINO_DNSServer[3] = 1;
//  EEPROMDataRegular.LoggingEnabled = 0;
//  EEPROMDataRegular.CRC = eeprom_crc((unsigned char*)&EEPROMDataRegular, sizeof(EEPROMDataRegular) - sizeof(EEPROMDataRegular.CRC));
//
//  EEPROMDataWiFiAP.MagicNumber = 0x55AA;
//  memcpy(EEPROMDataWiFiAP.WLAN_SSID, "NasStancic", sizeof("NasStancic") + 1);
//  memcpy(EEPROMDataWiFiAP.WLAN_PASS, "lara05102008", sizeof("lara05102008") + 1);
//  EEPROMDataWiFiAP.WLAN_SECURITY = 2;
//  EEPROMDataWiFiAP.CRC = eeprom_crc((unsigned char*)&EEPROMDataWiFiAP, sizeof(EEPROMDataWiFiAP) - sizeof(EEPROMDataWiFiAP.CRC));



  
  return EEData_IsCRCOK();
}

inline void PrintEEPROMData()
{
#if defined(ESP8266) || defined(ESP32)
  Serialprint("EEPROM WLAN_SSID: ");
  Serialprintln((const char *)EEPROMDataWiFiAP.WLAN_SSID);
  Serialprint("EEPROM WLAN_PASS: ");
  Serialprintln((const char *)EEPROMDataWiFiAP.WLAN_PASS);
  Serialprint("EEPROM WLAN_SECURITY: ");
  Serialprintln(EEPROMDataWiFiAP.WLAN_SECURITY);
  Serialprint("EEPROM Server IP address: ");
  for (int i = 0; i < 4; i++)
  {
    Serialprint(EEPROMDataRegular.Server_IP[i]);
    if (i < 3) Serialprint(".");
  }
  Serialprintln("");
  Serialprint("EEPROM Server UDP Port: ");
  Serialprintln(EEPROMDataRegular.Server_UDP_Port);
  Serialprintln("");
#else
  Serialprint("EEPROM MAC address: ");
  for (int i = 0; i < 6; i++)
  {
    Serialprint2A(EEPROMDataRegular.ARDUINO_MACadd[i], HEX);
    if (i < 5) Serialprint(":");
  }
  Serialprintln("");
  Serialprint("EEPROM Server IP address: ");
  for (int i = 0; i < 4; i++)
  {
    Serialprint(EEPROMDataRegular.Server_IP[i]);
    if (i < 3) Serialprint(".");
  }
  Serialprintln("");
  Serialprint("EEPROM Server UDP Port: ");
  Serialprintln(EEPROMDataRegular.Server_UDP_Port);
  Serialprintln("");
  Serialprintln("");
#endif
}



inline void SaveEEPROMData()
{
  int i;
#if defined(ESP8266) || defined(ESP32)
  EEPROMDataRegular.CRC = eeprom_crc((unsigned char*)&EEPROMDataRegular, sizeof(EEPROMDataRegular) - sizeof(EEPROMDataRegular.CRC));
  for (i = 0; i < sizeof(EEPROMDataRegular); i++)
  {
    EEPROM.write(EEPROM_DATA_REGULAR_OFFSET + i, ((byte*)(&EEPROMDataRegular))[i]);
  }
  EEPROMDataWiFiAP.CRC = eeprom_crc((unsigned char*)&EEPROMDataWiFiAP, sizeof(EEPROMDataWiFiAP) - sizeof(EEPROMDataWiFiAP.CRC));
  for (i = 0; i < sizeof(EEPROMDataWiFiAP); i++)
  {
    EEPROM.write(EEPROM_DATA_WIFIAP_OFFSET + i, ((byte*)(&EEPROMDataWiFiAP))[i]);
  }
  EEPROM.commit();
#else
  EEPROM.put(EEPROM_DATA_REGULAR_OFFSET, EEPROMDataRegular);  
#endif
}

inline bool IsLoggingEnabled()
{
  return EEPROMDataRegular.LoggingEnabled;
}

inline void EEData_Reset()
{
  EEData_Reset_Regular();
  EEData_Reset_WiFy();
}

inline void EEData_Reset_Regular()
{
  EEPROMDataRegular.MagicNumber = 0x55AA;
  EEPROMDataRegular.CommsType = 0;
  for (byte i = 0; i < 5; i++)
  {
    EEPROMDataRegular.RF24_ChAdd0[i] = 0xA0 + i;
    EEPROMDataRegular.RF24_ChAdd1[i] = 0xB0 + i;
  }
  EEPROMDataRegular.RetransmitCount = 3;
  EEPROMDataRegular.PingTimeoutSeconds = 2;
  for (byte i = 0; i < 6; i++)
  {
    EEPROMDataRegular.ARDUINO_MACadd[i] = 0xA0 + i;
  }  
  EEPROMDataRegular.Server_IP[0] = 192;
  EEPROMDataRegular.Server_IP[1] = 168;
  EEPROMDataRegular.Server_IP[2] = 88;
  EEPROMDataRegular.Server_IP[3] = 222;
  EEPROMDataRegular.Server_UDP_Port = 30123;
  EEPROMDataRegular.ARDUINO_SubNetMask[0] = 255;
  EEPROMDataRegular.ARDUINO_SubNetMask[1] = 255;
  EEPROMDataRegular.ARDUINO_SubNetMask[2] = 255;
  EEPROMDataRegular.ARDUINO_SubNetMask[3] = 0;
  EEPROMDataRegular.ARDUINO_dfGW[0] = 192;
  EEPROMDataRegular.ARDUINO_dfGW[1] = 168;
  EEPROMDataRegular.ARDUINO_dfGW[2] = 0;
  EEPROMDataRegular.ARDUINO_dfGW[3] = 1;
  EEPROMDataRegular.ARDUINO_DNSServer[0] = 192;
  EEPROMDataRegular.ARDUINO_DNSServer[1] = 168;
  EEPROMDataRegular.ARDUINO_DNSServer[2] = 0;
  EEPROMDataRegular.ARDUINO_DNSServer[3] = 1;
  EEPROMDataRegular.LoggingEnabled = 0;
  EEPROMDataRegular.CRC = eeprom_crc((unsigned char*)&EEPROMDataRegular, sizeof(EEPROMDataRegular) - sizeof(EEPROMDataRegular.CRC));
}

inline void EEData_Reset_WiFy()
{
#if defined(ESP8266) || defined(ESP32)
  EEPROMDataWiFiAP.MagicNumber = 0x55AA;
  memcpy(EEPROMDataWiFiAP.WLAN_SSID, "NasStancicHA", sizeof("WLAN_SSID") + 1);
  memcpy(EEPROMDataWiFiAP.WLAN_PASS, "lara05102008", sizeof("WLAN_PASS") + 1);
  EEPROMDataWiFiAP.WLAN_SECURITY = 2;
  EEPROMDataWiFiAP.CRC = eeprom_crc((unsigned char*)&EEPROMDataWiFiAP, sizeof(EEPROMDataWiFiAP) - sizeof(EEPROMDataWiFiAP.CRC));
#endif
}

inline void EEData_SetCRC()
{
  EEPROMDataRegular.CRC = eeprom_crc((unsigned char*)&EEPROMDataRegular, sizeof(EEPROMDataRegular) - sizeof(EEPROMDataRegular.CRC));
#if defined(ESP8266) || defined(ESP32)
  EEPROMDataWiFiAP.CRC = eeprom_crc((unsigned char*)&EEPROMDataWiFiAP, sizeof(EEPROMDataWiFiAP) - sizeof(EEPROMDataWiFiAP.CRC));
#endif
}

unsigned long eeprom_crc(unsigned char* data, int len)
{
  const unsigned long crc_table[16] = {
      0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
      0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
      0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
      0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
  };  
  unsigned long crc = ~0L;
  for( int index = 0; index < len; ++index )
  {
    crc = crc_table[( crc ^ data[index] ) & 0x0f] ^ (crc >> 4);
    crc = crc_table[( crc ^ (data[index] >> 4 )) & 0x0f] ^ (crc >> 4);    
    crc = ~crc;
  }
  return crc;
}

inline bool EEData_IsCRCOK()
{
  bool EEPROMDataRegularVerified = (EEPROMDataRegular.CRC == eeprom_crc((unsigned char*)&EEPROMDataRegular, sizeof(EEPROMDataRegular) - sizeof(EEPROMDataRegular.CRC)));
  bool EEPROMDataWiFiAPVerified = true;
#if defined(ESP8266) || defined(ESP32)
  EEPROMDataWiFiAPVerified = (EEPROMDataWiFiAP.CRC == eeprom_crc((unsigned char*)&EEPROMDataWiFiAP, sizeof(EEPROMDataWiFiAP) - sizeof(EEPROMDataWiFiAP.CRC)));
#endif
  return (EEPROMDataRegularVerified && EEPROMDataWiFiAPVerified);
}

inline bool SetEEDataRegular(byte* buf, int len)
{
  int p = 0;
  EEData_Reset_Regular();
  memcpy((byte*)(&(EEPROMDataRegular.MagicNumber)), &(buf[p]), 2);
  p += 2;
  memcpy((byte*)(&(EEPROMDataRegular.CommsType)), &(buf[p]), 1);
  p += 1;
  memcpy((byte*)(&(EEPROMDataRegular.RF24_ChAdd0[0])), &(buf[p]), 5);
  p += 5;
  memcpy((byte*)(&(EEPROMDataRegular.RF24_ChAdd1[0])), &(buf[p]), 5);
  p += 5;
  memcpy((byte*)(&(EEPROMDataRegular.RetransmitCount)), &(buf[p]), 1);
  p += 1;
  memcpy((byte*)(&(EEPROMDataRegular.PingTimeoutSeconds)), &(buf[p]), 1);
  p += 1;
  memcpy((byte*)(&(EEPROMDataRegular.ARDUINO_MACadd[0])), &(buf[p]), 6);
  p += 6;
  memcpy((byte*)(&(EEPROMDataRegular.Server_IP[0])), &(buf[p]), 4);
  p += 4;
  memcpy((byte*)(&(EEPROMDataRegular.Server_UDP_Port)), &(buf[p]), 2);
  p += 2;
  memcpy((byte*)(&(EEPROMDataRegular.ARDUINO_SubNetMask[0])), &(buf[p]), 4);
  p += 4;
  memcpy((byte*)(&(EEPROMDataRegular.ARDUINO_dfGW[0])), &(buf[p]), 4);
  p += 4;
  memcpy((byte*)(&(EEPROMDataRegular.ARDUINO_DNSServer[0])), &(buf[p]), 4);
  p += 4;
  memcpy((byte*)(&(EEPROMDataRegular.LoggingEnabled)), &(buf[p]), 1);
  p += 1;
  SaveEEPROMData();
  return true;
}

inline bool GetEEDataRegular(byte* buf, int* len)
{
  int p = 0;
  memcpy(&(buf[p]), (byte*)(&(EEPROMDataRegular.MagicNumber)), 2);
  p += 2;
  memcpy(&(buf[p]), (byte*)(&(EEPROMDataRegular.CommsType)), 1);
  p += 1;
  memcpy(&(buf[p]), (byte*)(&(EEPROMDataRegular.RF24_ChAdd0[0])), 5);
  p += 5;
  memcpy(&(buf[p]), (byte*)(&(EEPROMDataRegular.RF24_ChAdd1[0])), 5);
  p += 5;
  memcpy(&(buf[p]), (byte*)(&(EEPROMDataRegular.RetransmitCount)), 1);
  p += 1;
  memcpy(&(buf[p]), (byte*)(&(EEPROMDataRegular.PingTimeoutSeconds)), 1);
  p += 1;
  memcpy(&(buf[p]), (byte*)(&(EEPROMDataRegular.ARDUINO_MACadd[0])), 6);
  p += 6;
  memcpy(&(buf[p]), (byte*)(&(EEPROMDataRegular.Server_IP[0])), 4);
  p += 4;
  memcpy(&(buf[p]), (byte*)(&(EEPROMDataRegular.Server_UDP_Port)), 2);
  p += 2;
  memcpy(&(buf[p]), (byte*)(&(EEPROMDataRegular.ARDUINO_SubNetMask[0])), 4);
  p += 4;
  memcpy(&(buf[p]), (byte*)(&(EEPROMDataRegular.ARDUINO_dfGW[0])), 4);
  p += 4;
  memcpy(&(buf[p]), (byte*)(&(EEPROMDataRegular.ARDUINO_DNSServer[0])), 4);
  p += 4;
  memcpy(&(buf[p]), (byte*)(&(EEPROMDataRegular.LoggingEnabled)), 1);
  p += 1;
  *len = p;
  return true;
}

#if defined(ESP8266) || defined(ESP32)
  inline bool SetEEDataWiFi(byte* buf, int len)
  {
    EEData_Reset_WiFy();
    int p = 0;
    memcpy((byte*)(&(EEPROMDataWiFiAP.MagicNumber)), &(buf[p]), 2);
    p += 2;
    memcpy((byte*)(&(EEPROMDataWiFiAP.WLAN_SSID[0])), &(buf[p]), 32);
    p += 32;
    memcpy((byte*)(&(EEPROMDataWiFiAP.WLAN_PASS[0])), &(buf[p]), 32);
    p += 32;
    memcpy((byte*)(&(EEPROMDataWiFiAP.WLAN_SECURITY)), &(buf[p]), 4);
    p += 4;
    SaveEEPROMData();
	return true;
  }
  inline bool GetEEDataWiFi(byte* buf, int* len)
  {
    int p = 0;
    memcpy(&(buf[p]), (byte*)(&(EEPROMDataWiFiAP.MagicNumber)), 2);
    p += 2;
    memcpy(&(buf[p]), (byte*)(&(EEPROMDataWiFiAP.WLAN_SSID[0])), 32);
    p += 32;
    memcpy(&(buf[p]), (byte*)(&(EEPROMDataWiFiAP.WLAN_PASS[0])), 32);
    p += 32;
    memcpy(&(buf[p]), (byte*)(&(EEPROMDataWiFiAP.WLAN_SECURITY)), 4);
    p += 4;
    *len = p;  
	return true;
  }
#endif
