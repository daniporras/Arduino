
// DATE: 7/31/2014
// UPDATE: 3/02/2015
// VERSION 1.0.1
// EMAIL: daniel.porras@gmail.com
// Twitter: @daniporras

// Library neccesary
#include "DHT.h"
#include <SPI.h>
#include <OneWire.h>
// I used 28j60 NOT shield pins 9,10,11,12
#include <UIPEthernet.h> 
#define DHTPIN 4 // I use this pin to DHT22
#define DHTTYPE DHT22   //type of sensor 

//Functions
DHT dht(DHTPIN, DHTTYPE);
OneWire  ds(2);  // I use this pin to conect ds18b20

// Globals

// Definition of MACS and static IP
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x00, 0x23, 0x36 };
IPAddress ip(192,168,1,177);


// Start the server on 80 port
EthernetServer server(80);


void setup() {
 
  
  Serial.begin(9600);
   
  // This is used to star the funcion in library
  dht.begin();

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("The server is at IP:  ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  /*if (isnan(tempC) || isnan(tempC)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(tempC);
    Serial.println(" *C");
  }
  */
  
  // listen for incoming clients
  EthernetClient client = server.available();
  
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !ds.search(addr)) {
    //Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  
  //Serial.print("ROM =");
  //for( i = 0; i < 8; i++) {
    //Serial.write(' ');
    //Serial.print(addr[i], HEX);
  //}

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  //Serial.print("  Data = ");
  //Serial.print(present, HEX);
 // Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
   // Serial.print(data[i], HEX);
   // Serial.print(" ");
  }
  //Serial.print(" CRC=");
  //Serial.print(OneWire::crc8(data, 8), HEX);
  //Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");  


// Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.println("  Sensor =\t DHT 22 ");
    Serial.print("  Temperature: "); 
    Serial.print(t);
    Serial.print(" *C");
    Serial.print(" \t");
    Serial.print("Humidity: "); 
    Serial.println(h);
    
  
  }
if (client) {
    Serial.println("New client");
    // an http request ends with a blank line
    
    boolean currentLineIsBlank = true;
    
    while (client.connected()) {
     
      if (client.available()) {
     
        char c = client.read();
     
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
	  client.println("Refresh: 10");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<title> WebPage of sensors </title>");
          client.println("<body>");
          client.println("<h1> WebPage of sensors, by DaniPorras.es </h1>");
          client.println("<H2>"); 
          client.print("Sensor DHT 22: "); 
          client.println("</H2>"); 
          client.println("<p />"); 
          client.println("<H1>"); 
	  client.print("Temperature: ");
          client.print(t);
          client.print(" Celsius\t");
	  client.print(" Humidity: ");
	  client.print(h);
	  client.println("%");
          client.println("</H1>"); 
          client.println("<p />");  
          client.println("<H2>"); 
          client.print("Sensor DS1820: "); 
          client.println("</H2>"); 
          client.println("<H1>");
	  client.print("Temperature: "); 
          client.print(celsius);
          client.println(" &#176;");
          client.println("C");
	  client.println("\t Fahrenheit: ");
	  client.println(fahrenheit);
	  client.println(" F");
          client.println("</H1>");        
          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // Give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("Client disconnected");
  }
  delay(1000);
}

