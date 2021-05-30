#include <OneButton.h>
#include <TM1637Display.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const int CLK = D4; //Set the CLK pin connection to the display
const int DIO = D3; //Set the DIO pin connection to the display
int numCounter = 0;
long Wert;

unsigned long vorher;
unsigned int messIntervall = 5000;

const int pinTrigger = D7;
const int pinEcho = D6;
long dauer = 0;
long entfernungCM = 0;

const int Rot = D8;
const int Gelb = D5;
const int Gruen = D0;
int Taster = D2;
boolean merker;


TM1637Display display(CLK, DIO); //set up the 4-Digit Display.

void setup()
{

  digitalWrite (Rot, HIGH);
  digitalWrite (Gelb, HIGH);
  digitalWrite (Gruen, HIGH);

  display.setBrightness(0x0a); //set the diplay to maximum brightness

  Serial.begin(9600);
  pinMode(pinTrigger, OUTPUT);
  pinMode(pinEcho, INPUT);
  pinMode(Rot, OUTPUT);
  pinMode(Gelb, OUTPUT);
  pinMode(Gruen, OUTPUT);
  pinMode(Taster, INPUT);


}
void loop()
{


  digitalWrite(pinTrigger, LOW);
  delay(5);
  digitalWrite(pinTrigger, HIGH);
  delay(10);
  digitalWrite(pinTrigger, LOW);

  dauer = pulseIn(pinEcho, HIGH);
  entfernungCM = (dauer / 2) * 0.03432;

  Serial.println(entfernungCM);

  delay(200);

}
