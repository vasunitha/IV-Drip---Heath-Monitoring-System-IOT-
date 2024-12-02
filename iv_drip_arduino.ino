#include <Arduino.h>
#include "HX711.h"
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
// Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
// Otherwise leave the default "550" value.

PulseSensorPlayground pulseSensor;
const int LOADCELL_DOUT_PIN = 8;
const int LOADCELL_SCK_PIN = 9;

HX711 scale;
int relay1 = 10;
int relay2 = 11;
int buzzer = 4;
int p;
float L,TF,TC;
String str,S;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sensors.begin();
  digitalWrite(buzzer, LOW);
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);

  // Double-check the "pulseSensor" object was created and "began" seeing a signal.
  if (pulseSensor.begin()) {
    //Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.
  }
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(-471.497);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0
}

void loop() {
  // put your main code here, to run repeatedly:
  weight_status();
  temperature();
  pulse();
  str = String(L) + "," + String(S) + "," + String(TC) + "," + String(TF) + "," + String(p);
  Serial.println(str);
  delay(1000);
}

void weight_status() {
  float load = scale.get_units();
  L = load;
  scale.power_down();             // put the ADC in sleep mode
  delay(1000);
  scale.power_up();

  if (load < 100) {
    S = "ALERT!!";
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
    delay(1000);
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, LOW);
    
  }

else if(load > 100){
 S = "FINE!!";
    digitalWrite(buzzer, LOW);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay1, LOW); 
}

}

void temperature() {
  sensors.requestTemperatures();
  //print the temperature in Celsius
  
  TC = sensors.getTempCByIndex(0);
  

  //print the temperature in Fahrenheit
  TF = (sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0;
  
}

void pulse() {
  int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".                                              // "myBPM" hold this BPM value now.
  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
   
    p = myBPM;                        // Print the value inside of myBPM.
  }

  //delay(100);
}
