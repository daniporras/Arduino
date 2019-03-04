// Version 1.0  March 2019
// This version does not use the buttons 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PubSubClient.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// Update these with values suitable for your network.
const char* ssid = "";
const char* password = "";
const char* mqtt_server = "daniporras.com";
// Data wire is plugged into pin D1 on the ESP8266 12-E - GPIO 5
//Where is the ds18b20 sensors -> D3
#define ONE_WIRE_BUS D3 

// chars
byte termometru[8] = //icon for termometer
{
  B00100,
  B01010,
  B01010,
  B01010,
  B01110,
  B11111,
  B11111,
  B01110
};
byte vacio[8]=
{
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte subida[8]=
{
  B00100,
  B01110,
  B11111,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100
};

byte bajada[8]=
{
  B00100,
  B00100,
  B00100,
  B00100,
  B11111,
  B11111,
  B01110,
  B00100
};
byte punto[8]={
  B11000,
  B11000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte tempe[8] =
{
 B11011,
  B11010,
  B00010,
  B00011,
  B00000,
  B00000,
  B00000,
  B00000 
};

const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
// Set the LCD address to 0x27 for a 16 chars and 2 line display

LiquidCrystal_I2C lcd(0x27, 20, 4);

// Sets the sensors

DeviceAddress tempSensor1 = {0x28, 0xFF, 0xC1, 0xD3, 0xA4, 0x15, 0x01, 0x0B};
DeviceAddress tempSensor2 = {0x28, 0xFF, 0x27, 0xCE, 0xA4, 0x15, 0x01, 0XE8};

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature DS18B20(&oneWire);

char temperatureCString_UNO[7];
char temperatureCString_DOS[7];
char temperatureFString[7];

float temperatura1;
float temperatura2;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(2, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.createChar(1, termometru);
  lcd.createChar(2, subida);
  lcd.createChar(3, bajada);
  lcd.createChar(4, punto);
  lcd.createChar(5, tempe);
  lcd.createChar(6, vacio);

  DS18B20.setResolution(tempSensor1, 9);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  timeClient.begin();
}

void getTemperature() {
  float tempC;
   DS18B20.requestTemperaturesByAddress(tempSensor1); 
   tempC = DS18B20.getTempC(tempSensor1);
   dtostrf(tempC, 2, 2, temperatureCString_UNO);
   temperatura1 = tempC;
   
}
void getTemperature_DOS() {
  float tempC;
  DS18B20.requestTemperaturesByAddress(tempSensor2); 
  tempC = DS18B20.getTempC(tempSensor2);
  dtostrf(tempC, 2, 2, temperatureCString_DOS);
  temperatura2 = tempC;
  
}
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  lcd.setCursor(0,3);
    for(int i=0; i<20; i++)
    {
    //  lcd.write(6);
    }
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    
    // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
    lcd.print("ACTIVADO");
    digitalWrite(2, LOW);
    
  } else {
    
    lcd.setCursor(0,3);   
    lcd.print("DESACTIVADO");
    digitalWrite(2, HIGH);  // Turn the LED off by making the voltage HIGH
    
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("casa/despacho/temperatura", "Enviando el primer mensaje");
      client.publish("casa/despacho/temperatura1", "Enviando el primer mensaje");
      // ... and resubscribe
      client.subscribe("casa/despacho/luz");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    timeClient.update();
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "Hello World #%ld", value);
    Serial.print("Publish message: ");
    getTemperature();
    // Serial.printf("Sensor 1 ");
    // Serial.println(temperatureCString_UNO);
    
    client.publish("casa/despacho/temperatura", temperatureCString_UNO);
    getTemperature_DOS();
    lcd.setCursor(0, 0);
    lcd.print(String(daysOfTheWeek[timeClient.getDay()]) + ", " + String(timeClient.getHours()) + ":" + String(timeClient.getMinutes()));
    lcd.setCursor(0, 1);
    lcd.write(1);
    lcd.write(4);
    
    lcd.write(2);
    lcd.print(" "+String(temperatura1));
    lcd.write(5);
   
    lcd.print(" ");
    lcd.write(3);

    lcd.print(" "+String(temperatura2));
    lcd.write(5);
    
    client.publish("casa/despacho/temperatura1", temperatureCString_DOS);
    
    // Set a delay time for 10 senconds
    delay(10000);
  
  
  }
}
