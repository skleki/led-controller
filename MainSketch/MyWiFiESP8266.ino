unsigned char LAN_MAC_Address[6];
HttpRequest httpReq;

#define SERVER_PORT 80
WiFiServer server(SERVER_PORT); 

#define PARAM_RGB_COLOR "rgbColor"
#define PARAM_RGB_INTENSITY "rgbIntensity"
#define PARAM_WHITE_INTENSITY "whiteIntensity"
#define PARAM_RGB_OFF "rgbOff"
#define PARAM_WHITE_OFF "whiteOff"
#define PARAM_RGB_MODE "rgbMode"

extern int CurrentWhiteIntensity;
extern bool CurrentWhiteIsOn;
extern int CurrentRed;
extern int CurrentGreen;
extern int CurrentBlue;
extern int CurrentRGBIntensity;
extern bool CurrentRGBIsOn;
extern String CurrentRGBMode;

inline void MyWiFi_Initialise(void) 
{  
  unsigned char MyWiFi_MAC_Address[6];
  Serialprintln("WiFI SCAN start...");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serialprintln("WiFI SCAN done!");
  if (n == 0)
  {
    Serialprintln("no networks found");
  }
  else
  {
    Serialprint(n);
    Serialprintln(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serialprint(i + 1);
      Serialprint(": ");
      Serialprint(WiFi.SSID(i));
      Serialprint(" (");
      Serialprint(WiFi.RSSI(i));
      Serialprint(")");
      Serialprintln((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serialprintln("");

  
  Serialprint("Configured WLAN_SSID: ");
  Serialprintln((const char *)EEPROMDataWiFiAP.WLAN_SSID);
  Serialprint("WLAN_PASS: ");
  Serialprintln((const char *)EEPROMDataWiFiAP.WLAN_PASS);
  Serialprint("WLAN_SECURITY: ");
  Serialprintln(EEPROMDataWiFiAP.WLAN_SECURITY);
  String clientMac = "";
  WiFi.macAddress(MyWiFi_MAC_Address);	// gets MAC address into buffer
  memcpy(LAN_MAC_Address, MyWiFi_MAC_Address, 6);
  clientMac += macToStr(MyWiFi_MAC_Address);
  clientMac.toUpperCase();
  Serialprint("MAC Address: ");
  Serialprintln(clientMac);

  WIFI_Connect();

  server.begin();
  Serialprint("Server listening is at port ");
  Serialprintln(SERVER_PORT);
}


inline void MyWiFi_Loop(void) 
{
  const long timeoutTime = 500;
  unsigned long currentTime = millis();
  unsigned long previousTime = 0; 

  if (WiFi.status() != WL_CONNECTED)
  {
    Serialprintln("Wifi not connected. reconnecting...");
    WIFI_Connect();
    return;
  }

  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) 
  {
    Serialprintln("New HTTP client!");
    httpReq.resetRequest();
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) // loop while the client's connected
    {
      currentTime = millis();         
      if (client.available()) 
      {
        char c = client.read();
        
        //parse the received caracter
        httpReq.parseRequest(c);
        
        Serialprint(c);
        
        //IF request has ended -> handle response
        if (httpReq.endOfRequest()) 
        {
          // This is a utility function to list HTTP request details
          // UtilityParseHttpRequest(&client, &httpReq);

          PrintHttpRequestDetails(&client, &httpReq);
          ParseHttpRequestAndDoActiona(&httpReq);
//          String myPage = generate_html_page(GetPowerLine(WHITE_POWER_LINE), GetPowerLine(RGB_POWER_LINE), 
//                                              CurrentWhiteIntensity, CurrentRGBIntensity, 
//                                              CurrentRed, CurrentGreen, CurrentBlue, CurrentRGBMode);
          String myPage = generate_html_page(true, false, 
                                              CurrentWhiteIntensity, CurrentRGBIntensity, 
                                              CurrentRed, CurrentGreen, CurrentBlue, CurrentRGBMode);

          client.println(String(http_response_200_ok).c_str());
          delay(50);
          client.println(myPage.c_str());

          //Reset object and free dynamic allocated memory
          httpReq.resetRequest();
        }
      }
    }

    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serialprint("Client disonnected after ");
    Serialprint(currentTime - previousTime);
    Serialprint(" ms");
    Serialprintln("");
  }   
//  else
//  {
//    Serialprintln("No new HTTP client!");
//  }
}


inline void WIFI_Connect()
{
  WiFi.disconnect();
  // setting up Station IP statically

  IPAddress ip(192, 168, 88, 222);
  IPAddress gw(192, 168, 88, 1);
  IPAddress dns(192, 168, 88, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, dns, gw, subnet);

  // setting up Station mode
  WiFi.mode(WIFI_STA);
  WiFi.begin((const char *)EEPROMDataWiFiAP.WLAN_SSID, (const char *)EEPROMDataWiFiAP.WLAN_PASS);

  // Wait for connect to AP
  Serialprint("[Connecting]");
  Serialprint((const char *)EEPROMDataWiFiAP.WLAN_SSID);
  int tries=0;
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serialprint(".");
    tries++;
    if (tries > 30)
    {
      break;
    }
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Serialprintln("Unable to connest to WiFi network");
    Serialprintln();
    return;
  }
  
  Serialprintln("Connected to wifi");
  Serialprintln();
  printWifiStatus();
}
inline long SignalStrength()
{
  if (WiFi.status() != WL_CONNECTED) 
  {
    Serialprintln("Couldn't get a wifi connection");
    return 0;
  }
  else
  {
    long rssi = WiFi.RSSI();
    Serialprint("RSSI:");
    Serialprintln(rssi);
    return rssi;
  }
}

inline void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serialprint("SSID: ");
  Serialprintln(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serialprint("Module IP Address: ");
  Serialprintln(ip);
}

inline String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) 
  {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

void ParseHttpRequestAndDoActiona(HttpRequest* request)
{
  //declare name and value to use the request parameters and cookies
  char name[HTTP_REQ_PARAM_NAME_LENGTH], value[HTTP_REQ_PARAM_VALUE_LENGTH]; 
  char name2[HTTP_REQ_PARAM_NAME_LENGTH], value2[HTTP_REQ_PARAM_VALUE_LENGTH]; 
  
  for(int i=1; i <= request->paramCount; i++)
  {
    request->getParam(i,name,value);
    String paramName(name);
    String paramValue(value);
    Serialprint("Processing parameter [");
    Serialprintln(paramName);
    Serialprint("]: ");
    Serialprintln(paramValue);
    
    if (paramName == PARAM_RGB_COLOR)
    {
      CurrentRGBIsOn = true;
      RandomRGB_Stop();
      String newRedAsString = paramValue.substring(3, 5);
      String newGreenAsString = paramValue.substring(5, 7);
      String newBlueAsString = paramValue.substring(7, 9);
      int newRed = strtol(newRedAsString.c_str(), 0, 16);
      int newGreen = strtol(newGreenAsString.c_str(), 0, 16);
      int newBlue = strtol(newBlueAsString.c_str(), 0, 16);
      SetRGB(newRed, newGreen, newBlue, CurrentRGBIntensity, true, CurrentRGBIsOn);
    }
    else if (paramName == PARAM_RGB_INTENSITY)
    {
      int newRgbIntensity = paramValue.toInt();
      if (newRgbIntensity >= 0)
      {
        SetRGB(CurrentRed, CurrentGreen, CurrentBlue, newRgbIntensity, true, CurrentRGBIsOn);
      }
      else
      {
        Serialprint("Received BAD value for parameter: ");
        Serialprintln(paramName);
        Serialprint("Value received is: ");
        Serialprintln(paramValue);
      }
    }
    else if (paramName == PARAM_WHITE_INTENSITY)
    {
      CurrentWhiteIsOn = true;
      int newWhiteIntensity = paramValue.toInt();
      if (newWhiteIntensity >= 0)
      {
          SetWhite(newWhiteIntensity, CurrentRGBIsOn);
      }
      else
      {
        Serialprint("Received BAD value for parameter: ");
        Serialprintln(paramName);
        Serialprint("Value received is: ");
        Serialprintln(paramValue);
      }
    }
    else if (paramName == PARAM_RGB_OFF)
    {
      RandomRGB_Stop();
      CurrentWhiteIsOn = false;
      SetRGB(CurrentRed, CurrentGreen, CurrentBlue, CurrentRGBIntensity, true, CurrentWhiteIsOn);
    }
    else if (paramName == PARAM_WHITE_OFF)
    {
      CurrentWhiteIsOn = false;
      SetWhite(CurrentWhiteIntensity, CurrentWhiteIsOn);
    }
    else if (paramName == PARAM_RGB_MODE)
    {
      CurrentRGBIsOn = true;
      int newRgbIntensity = -1; 
      for(int k=1; k <= request->paramCount; k++)
      {
        request->getParam(k,name2,value2);
        String paramName2(name2);
        String paramValue2(value2);
        if (paramName2 == PARAM_RGB_INTENSITY)
        {
          newRgbIntensity = paramValue2.toInt();
          if (newRgbIntensity < 0)
          {
            Serialprint("Received BAD value for parameter: ");
            Serialprintln(paramName2);
            Serialprint("Value received is: ");
            Serialprintln(paramValue2);
          }
          break;
        }
      }

      if (newRgbIntensity < 0)
      {
        Serialprint("Received BAD intensity param for rgbMode. ");
        Serialprint("Value received is: ");
        Serialprintln(newRgbIntensity);
      }
      else
      {
        if (paramValue == RGB_MODE_RANDOM_SLOW)
        {
          RandomRGB_Start(true, newRgbIntensity);
        }
        else if (paramValue == RGB_MODE_RANDOM_FAST)
        {
          RandomRGB_Start(false, newRgbIntensity);
        }
        else
        {
          RandomRGB_Stop();
        }

        CurrentRGBMode = paramValue;
      }
    }
  }
}






// This is a utility function to list HTTP request details
void UtilityParseHttpRequest(WiFiClient* client, HttpRequest* request)
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

void PrintHttpRequestDetails(WiFiClient* client, HttpRequest* request)
{
  //declare name and value to use the request parameters and cookies
  char name[HTTP_REQ_PARAM_NAME_LENGTH], value[HTTP_REQ_PARAM_VALUE_LENGTH]; 

  Serialprint("Method: ");
  Serialprintln(request->method);
  Serialprint("Uri: ");
  Serialprintln(request->uri);
  Serialprint("Version: ");
  Serialprintln(request->version);
  Serialprint("paramCount: ");
  Serialprintln(request->paramCount);
  //list received parameters GET and POST
  Serialprintln("Parameters:");
  for(int i=1;i<=request->paramCount;i++){
    request->getParam(i,name,value);
    Serialprint("  ");
    Serialprint(name);
    Serialprint("-");
    Serialprint(value);
    Serialprintln("");
  }
  //list received cookies
  Serialprintln("Cookies:");
  for(int i=1;i<=request->cookieCount;i++)
  {
    request->getCookie(i,name,value);
    Serialprint("  ");
    Serialprint(name);
    Serialprint(" - ");
    Serialprint(value);
    Serialprintln("");
  }
  
}
