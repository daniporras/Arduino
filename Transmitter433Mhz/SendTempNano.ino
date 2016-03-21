//Codigo para arduino nano que envia datos

#include <OneWire.h>
#include <DallasTemperature.h>
#include <VirtualWire.h>
 
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 5
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
// variable para enviar datos
char msg[6];


 
void setup(void)
{
  // start serial port
  //Serial.begin(9600);
  //Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();

  // start 433mhz
  //vw_set_ptt_inverted(true); //
  
  vw_setup(2000);// speed of data transfer Kbps
  vw_set_tx_pin(8);
}
 
 
void loop(void)
{
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  //Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  //Serial.println("DONE");

  //Serial.print("Temperature for Device 1 is: ");
  //Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"? 
    // You can have more than one IC on the same bus. 
    // 0 refers to the first IC on the wire

    //here is the new code

  float temperature =sensors.getTempCByIndex(0);
  dtostrf(temperature, 6, 2, msg);
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  
  delay(200);
  
    
 
}
