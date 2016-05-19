Arduino NANO with DHT22 and I2C LCD 

In this proyect we will leart who conected a LCD and DHT22 with Arduino NANO.

##First Step
We need the following libraries:
 `DHT.h` 
 `Wire.h`
 `LiquidCrystal_I2C.h`

##Second Step
We made the connections with the wires:

LCD: 

    A4 <--> SCL
    A5 <--> SDA
    
DHT22

    D5 <--> DataSensor

##Thrid Step
Configuring the LCD memory.
In this project, i used a 20x4 Display and the memory i2c is 0x27. In other LCD, you might need a sketch to get you LCD memory.

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

##Fourth Step
Using my code, you will see the data on your LCD
