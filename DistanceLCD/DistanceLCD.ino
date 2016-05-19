// Here we included our libraries
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
// this project show only the temperature and humidity on i2c 20x4 LCD
// PIN OUT
// A4 --SCL
// A5 ---SDA
// Trig -- D8
// Echo -- D7

#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin

// This is important
// I am using a 20x4 LCD and the memory is 0x27 for this device

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// For the LCD we use:
// lcd.begin()
// lcd.clear()
// lcd.print()
// lcd.setCursor()


int maximumRange = 250; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance
void setup(void)
{
 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.begin(20,4); 
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Twitter: @daniporras");       
  lcd.setCursor(3, 1);
  lcd.print("daniporras.xyz"); 
}
 
 
void loop(void)
{
/* Then we get the data from the sensor HC-SR04 */
/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 /* Calculate the distance (in cm) based on the speed of sound. */
 distance = duration/58.2;
  
/* Lets put the data to the display */
lcd.setCursor(0,2);
lcd.print("We are using HC-SR04");

/* And finally we put the humidity */
lcd.setCursor(0,3);
lcd.print("Distance is:");
lcd.setCursor(13,3);
lcd.print(distance);
lcd.setCursor(18, 3);
lcd.print("cm");

/* We have a delay for 500ms */
  delay(500);
    
}

