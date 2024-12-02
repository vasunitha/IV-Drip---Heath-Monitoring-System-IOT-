/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on NodeMCU.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!

  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
static const int RXPin = D4, 








TXPin = D5;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPL3guH0Z-AZ"
#define BLYNK_TEMPLATE_NAME "iv drip monitoring and health monitoring"
#define BLYNK_AUTH_TOKEN "j2r4y7riYZ817839LI3o3WdTY3TFVHmq"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Redmi Note 10 Pro Max";
char pass[] = "11110000";

void setup()
{
  // Debug console
  Serial.begin(9600);
  ss.begin(GPSBaud);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();     
  if (Serial.available()) {
    //Serial.write(Serial.read());
    String myString = Serial.readStringUntil('\n');
    String L = getValue(myString, ',', 0);
    String S = getValue(myString, ',', 1);
    String TC = getValue(myString, ',', 2);
    String TF = getValue(myString, ',', 3);
    String p = getValue(myString, ',', 4);
    
    
    Blynk.virtualWrite(V1, L);
    Blynk.virtualWrite(V2, S);
    Blynk.virtualWrite(V3, TC);
    Blynk.virtualWrite(V4, TF);
    Blynk.virtualWrite(V5, p);
    
  }
}

void displayInfo()
{
  Blynk.virtualWrite(V0,F("Location: ")); 
  if (gps.location.isValid())
  {
    //Serial.print("LATITUDE:");
    //Serial.println(gps.location.lat(), 6);
    Blynk.virtualWrite(V8,gps.location.lat(),6);
    //Serial.print(F(","));
    //Serial.print("LONGITUDE:");
    //Serial.println(gps.location.lng(), 6);
    Blynk.virtualWrite(V9,gps.location.lng(),6);

  }
  else
  {
    Blynk.virtualWrite(V0,F("INVALID"));
  }
  delay(500);
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
