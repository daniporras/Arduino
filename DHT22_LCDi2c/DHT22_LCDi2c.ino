// Here we included our libraries
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Here we defined our variables
#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN,DHTTYPE);
// This is important
// I am using a 20x4 LCD and the memory is 0x27 for this device

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// For the LCD we use:
// lcd.begin()
// lcd.clear()
// lcd.print()
// lcd.setCursor()

// We make global variable
float hum;
float temp;


void setup(void)
{
 // Start the Sensor DHT22 and lcd
  dht.begin();
  lcd.begin(20, 4); 
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Twitter: @daniporras.xyz");       
  lcd.setCursor(3, 1);
  lcd.print("daniporras.xyz"); 
}
 
 
void loop(void)
{
// Then we get the data from the sensor
  hum=dht.readHumidity();
    temp=dht.readTemperature();
  
// Lets put the data to the display
lcd.setCursor(0,2);
lcd.print("Temperature:");
lcd.setCursor(13,2);
lcd.print(temp);
lcd.setCursor(18, 2);
lcd.print((char)223);
lcd.print("C");

// And finally we put the humidity
lcd.setCursor(0,4);
lcd.print("Humidity:");
lcd.setCursor(13,3);
lcd.print(hum);
lcd.setCursor(19, 3);
lcd.print("%");

// We have a delay for 500ms
  delay(500);
    
}
