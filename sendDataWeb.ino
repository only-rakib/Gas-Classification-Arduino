#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"
#include <MQ2.h>
#include <ShiftRegister74HC595.h>


const int DATA_pin = D1;
const int LATCH_pin = D0;
const int SHIFTCLOCK_pin = D6;

// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

/*Put your SSID & Password*/
const char* ssid = "who_cares";  // Enter SSID here
const char* password = "#definepi3.1416";  //Enter Password here

ESP8266WebServer server(80);

// DHT Sensor
uint8_t DHTPin = D8;

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);
int buzzer = D7;
float Temperature;
float Humidity;
int Analog_Input = A0;
int lpg, co, smoke;
MQ2 mq2(Analog_Input);
ShiftRegister74HC595 sr (2, DATA_pin, SHIFTCLOCK_pin, LATCH_pin);
void setup() {

  Serial.begin(115200);
  delay(100);
  Serial.println("Paisi");
  pinMode(DHTPin, INPUT);
  pinMode(buzzer, OUTPUT);
  //pinMode(Analog_Input, INPUT);
  dht.begin();
  mq2.begin();

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

}
void loop() {

  server.handleClient();

}

void handle_OnConnect() {

  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity
  //analogSensor = analogRead(smokeA0);
  float* values = mq2.read(true); //set it false if you don't want to print the values in the Serial
  //lpg = values[0];
  lpg = mq2.readLPG();
  //co = values[1];
  co = mq2.readCO();
  //smoke = values[2];
  smoke = mq2.readSmoke();
  if(smoke>400.00)
  {
    digitalWrite(buzzer,HIGH);
  }
  else
  {
    digitalWrite(buzzer,LOW);
  }
  server.send(200, "text/html", SendHTML(Temperature, Humidity, lpg, co, smoke));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperaturestat, float Humiditystat, int lpg, int co, int smoke) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Report Via Wifi</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>ESP8266 NodeMCU Weather Report</h1>\n";

  ptr += "<p>Temperature: ";
  ptr += (int)Temperaturestat;
  ptr += " C</p>";
  ptr += "<p>Humidity: ";
  ptr += (int)Humiditystat;
  ptr += "%</p>";
  ptr += "<p>LPG: ";
  ptr += (int)lpg;
  ptr += " </p>";

  ptr += "<p>CO: ";
  ptr += (int)co;
  ptr += " </p>";

  ptr += "<p>Smoke : ";
  ptr += (int)smoke;
  ptr += " </p>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
void shiftRegisterControl(String inputData)
{
  int i;
  for (i = 0; i < inputData.length(); i++)
  {
    if (inputData.charAt(i) == 'A')
    {
      sr.set(1, HIGH);
    }
    else if (inputData.charAt(i) == 'B')
    {
      sr.set(2, HIGH);
    }
    else if (inputData.charAt(i) == 'C')
    {
      sr.set(3, HIGH);
    }
    else if (inputData.charAt(i) == 'D')
    {
      sr.set(4, HIGH);
    }
    else if (inputData.charAt(i) == 'E')
    {
      sr.set(5, HIGH);
    }
    else if (inputData.charAt(i) == 'F')
    {
      sr.set(6, HIGH);
    }
    else if (inputData.charAt(i) == 'G')
    {
      sr.set(7, HIGH);
    }
    else if (inputData.charAt(i) == 'H')
    {
      sr.set(9, HIGH);
    }
    else if (inputData.charAt(i) == 'I')
    {
      sr.set(10, HIGH);
    }
    else if (inputData.charAt(i) == 'J')
    {
      sr.set(11, HIGH);
    }
    else if (inputData.charAt(i) == 'K')
    {
      sr.set(12, HIGH);
    }
    else if (inputData.charAt(i) == 'L')
    {
      sr.set(13, HIGH);
    }

  }
  delay(5000);

  sr.setAllLow();
}
