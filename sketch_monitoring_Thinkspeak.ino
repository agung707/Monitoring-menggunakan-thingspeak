#include <DHT.h>
#include <ESP8266WiFi.h>
 
// replace with your channel’s thingspeak API key and your SSID and password
String apiKey = "D10W8RBE3ZZRFC9T";
const char* ssid = "VIRUZ";
const char* password = "08121996";
const char* server = "api.thingspeak.com";
const char* resource = "/update?api_key=";

const int ldr = A0;
const int trigPin = D2; 
const int echoPin = D1;
 
long duration;
float distance;

static char temperatureTemp[7];
static char humidityTemp[7];
static char cahaya[7];
static char jarak[7];

#define DHTPIN D7
#define DHTTYPE DHT11 
 
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;
 
void setup() 
{
Serial.begin(115200);
delay(10);
dht.begin();
 
pinMode(trigPin, OUTPUT); 
pinMode(echoPin, INPUT); 
  
WiFi.begin(ssid, password);
 
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
 
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) 
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
 
}
 
void loop() 
{
int nilai = analogRead(ldr); 
float h = dht.readHumidity();
float t = dht.readTemperature();
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);  
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance= duration*0.034/2;

if (isnan(h) || isnan(t)|| isnan(nilai)|| isnan(distance)) 
{
Serial.println("Failed to read from ALL sensor!");
return;
}
    dtostrf(t, 6, 2, temperatureTemp);      
    dtostrf(h, 6, 2, humidityTemp);
    dtostrf(nilai, 6, 2, cahaya);
    dtostrf(distance, 6, 2, jarak); 
if (client.connect(server,80)) {

client.print(String("GET ") + resource + apiKey + "&field1=" + temperatureTemp + "&field2=" + humidityTemp +"&field3=" + cahaya +"&field4=" + jarak +
                  " HTTP/1.1\r\n" +
                  "Host: " + server + "\r\n" + 
                  "Connection: close\r\n\r\n");

 
Serial.print("Suhu: ");
Serial.print(t);
Serial.print("Kelembaban: ");
Serial.print(h);
Serial.print(" Cahaya: ");
Serial.print(nilai);
Serial.print("Jarak: ");
Serial.print(distance);
Serial.println("Sending data to Thingspeak");
}
client.stop();
 
Serial.println("Waiting 20 secs");

delay(2000);
}
