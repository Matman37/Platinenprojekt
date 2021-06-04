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

//******** WLAN ********//
const char* ssid = "Martin Router King";
const char* password = "7896321426123";

//******** ESP8266 als Client starten ********//

WiFiClient WifiClient;
//******************* MQTT *********************//

const char* mqttServer = "192.168.188.20";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

PubSubClient broker(WifiClient);

//******************************* JSON ***********************//

StaticJsonDocument<200> jsonDaten;      // Platz schaffen für JSON-Daten

//******************************* Variable ***********************//



OneButton button1(D2, false);

TM1637Display display(CLK, DIO); //set up the 4-Digit Display.

void setup()
{
  display.setBrightness(0x0a); //set the diplay to maximum brightness

  Serial.begin(9600);
  pinMode(pinTrigger, OUTPUT);
  pinMode(pinEcho, INPUT);
  pinMode(Rot, OUTPUT);
  pinMode(Gelb, OUTPUT);
  pinMode(Gruen, OUTPUT);
  pinMode(Taster, INPUT);

  //******** WLAN verbinden ********//
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay (500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wifi connected");

  //******** MQTT Verbindung mit Broker ********//
  broker.setServer(mqttServer, mqttPort);

  while (!broker.connected())
  {
    Serial.println("Verbinden mit MQTT...");
    if (broker.connect("Mattis", mqttUser, mqttPassword ))
    {
      Serial.println("Verbunden");
    }
    else
    {
      Serial.println("Fehlermeldung: ");
      Serial.print (broker.state());
      delay (2000);
    }
  }
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
  if (entfernungCM >= 500 || entfernungCM <= 0)
  {
    Serial.println("Kein Messwert");
  }
  else
  {
    display.showNumberDec(Wert);
  }

  if ((entfernungCM >= 3) & (entfernungCM <= 6))
  {
    Wert = 1;
  }
  if ((entfernungCM >= 7) & (entfernungCM <= 9))
  {
    Wert = 2;
  }
  if ((entfernungCM >= 10) & (entfernungCM <= 12))
  {
    Wert = 3;
  }
  if (entfernungCM >= 13)
  {
    Wert = 4;
  }
  delay(200);


  //******************************* JSON Daten erzeugen ***********************//

  if (millis() - vorher >= messIntervall)
  {

    jsonDaten.clear();
    jsonDaten["Standort"] = "Halle1";
    jsonDaten["Messstation"] = "1";

    JsonArray Menge = jsonDaten.createNestedArray ("Anzahl");
    Menge[0] = Wert;



    char puffer[512];
    serializeJson(jsonDaten, puffer);
    Serial.println(puffer);
    broker.publish("Standort/Messstation", puffer);

    vorher = millis();
  }
  broker.loop();    //Aktualisierung, Vorgabe der Bibliothek
}
