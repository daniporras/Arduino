//Este es el codigo para el arduino uno que recibe los datos
// include the library code:
#include <LiquidCrystal.h>

#include <VirtualWire.h>

#include <OneWire.h>

#include <DallasTemperature.h>

int i;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define ONE_WIRE_BUS 10

OneWire ourWire(ONE_WIRE_BUS);
DallasTemperature sensors(&ourWire);

void setup(){
vw_set_rx_pin(8);
lcd.begin(16, 2);

sensors.begin();

vw_setup(2000);

vw_rx_start();



}

void loop(){

sensors.requestTemperatures();

lcd.setCursor(0, 1);

lcd.print(" In:");

lcd.setCursor(6, 1);

lcd.print(sensors.getTempCByIndex(0));

lcd.setCursor(12, 1);

lcd.print((char)223);

lcd.print("C");

uint8_t buf[VW_MAX_MESSAGE_LEN];
uint8_t buflen = VW_MAX_MESSAGE_LEN;

if( vw_get_message(buf, &buflen) )

{
lcd.setCursor(0, 0);

lcd.print("Out:");
lcd.setCursor(5,0);
for (i = 0; i < buflen; i++)
{

lcd.write(buf[i]);

}

lcd.setCursor(12, 0);

lcd.print((char)223);

lcd.print("C");

}

}

