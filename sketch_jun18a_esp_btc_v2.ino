#include <Arduino_JSON.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <tcMenu.h>
#include "iot.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const PROGMEM char *ssid = "FindMyGov.in";          ///"FindMyGov.in";      ///"Dinesh"    ///"jfldsjflsf" ///"Redme 6A";
const PROGMEM char *password = "8ca557whe82hs8ga8"; ///"8ca557whe82hs8ga8"; ///"dinesh123" ///"kl19m51A"   ///"12345678";

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
PROGMEM int debug=0;
int secs = 0;
int y=0;
unsigned long int price=0;
byte connectedToWifi = 0;
void iotsetup();
void iotloop();
#define X_ONLINE 0
#define X_FRESH 27
void setup(void)
{ 
  Serial.begin(9600);
  iotsetup();
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
    }

    
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, y=10);
  display.println(__DATE__);
  display.setCursor(0, y+=10);
  display.println(__TIME__);
  display.display(); 


    display.setCursor(X_ONLINE, 55);
    display.setTextSize(1);
    display.print("CON");
    display.display();          


  /*  while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        display.print(".");
        display.display(); 
    }*/

    taskManager.scheduleFixedRate(1000, []
      {
        String status;
        if(WiFi.status() == WL_CONNECTED) {
            if(!connectedToWifi) {
                //IPAddress localIp = WiFi.localIP();
                //menuIP.setIpAddress(localIp[0], localIp[1], localIp[2], localIp[3]);
                connectedToWifi = true;
                status = "ONL";           
            }
        }
        else if(connectedToWifi) {
            connectedToWifi = false;
            status = "OFF";          
        }

        if(status != "")
        {
            display.setCursor(X_ONLINE, 55);
            display.setTextSize(1);
            display.print(status.c_str());
            display.display();          
        }
    }
    );
        
    
    // now keep updating the lock in lcd menu every 10 sec
    taskManager.scheduleFixedRate(10000, []
                                  { ticker(); });
}

void loop() {
    taskManager.runLoop();
    iotloop() ;
}

void ticker()
{
    display.setTextSize(1);
    display.setCursor(X_FRESH, 55);
    display.print("   ");
    display.display();          
    display.setCursor(X_FRESH, 55);
    display.print("OLD");
    display.display();          
        
    if(!connectedToWifi)
      return;

    display.setCursor(0, 20);
    WiFiClientSecure client;
    client.setInsecure();
    int rt1 = client.connect("api-pub.bitfinex.com", 443);
    
    HTTPClient http;
    String serverPath = "https://api-pub.bitfinex.com/v2/ticker/tBTCUSD"; // + "?data=" + req;
    int rt2 = http.begin(client, serverPath.c_str());
    int httpResponseCode = http.GET();
    if (http.GET() != HTTP_CODE_OK) 
    {
      display.print(" NOT OK ");
      yield();
      return;
    }
    
    if (httpResponseCode > 0)
    {
        String payload = http.getString();
        JSONVar myArray = JSON.parse(payload);
        
        // JSON.typeof(jsonVar) can be used to get the type of the var
        if (JSON.typeof(myArray) == "undefined") {
          display.print(";105,");
          display.display(); 
          return;
        }        
        
        price = (int)myArray[6];
        display.clearDisplay();
        display.setTextSize(3);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        // Display static text
        display.print("$");
        display.print(price);
        display.display();

        /* no need to "new" it everytime
        display.setTextSize(1);
        display.setCursor(X_FRESH, 55);
        display.print("NEW");
        display.display();          
        */

      
    }
    else if (debug)
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    http.end();
}
