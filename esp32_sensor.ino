#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

const char* ssid = "Workshop";
const char* password = "Lesmoine496";

//adresse IP fixe
IPAddress local_IP(192, 168, 1, 184);
IPAddress gateway(192, 168, 1, 252);
IPAddress subnet(255, 255, 255, 0);

// Domain Name with full URL Path for HTTP POST Request
const char* serverName = "http://192.168.1.10:9000";
// Service API Key
String apiKey = "";


#define DHTTYPE DHT11
#define SENSORS_PORT 9000
#define SERVER_IP "192.168.1.10"

unsigned long lastTime = 0;
// Set timer to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Timer set to 10 seconds (10000)
unsigned long timerDelay = 10000;


// DHT Sensor
uint8_t DHTPin = 15; 
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);                

float Temperature;
float Humidity;

void setup() {
    Serial.begin(115200);
    delay(10);

    pinMode(DHTPin, INPUT);
    dht.begin();   

    /* Connection à un réseau Wifi */
    if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    
    Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");

}


void loop() {
    Temperature = dht.readTemperature(); // Gets the values of the temperature
    Humidity = dht.readHumidity();
    //int temperature = 15; // à lire
    //Serial.println(Temperature);
    //Serial.println(Humidity);

    //Send an HTTP POST request every 10 seconds
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
    
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
      
      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      String httpRequestData = "api_key=" + apiKey + "&temp=" + String(Temperature) + "&humi=" + String(Humidity);           
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
    }
    lastTime = millis();
    }
}
