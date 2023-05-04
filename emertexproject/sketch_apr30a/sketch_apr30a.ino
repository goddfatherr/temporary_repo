#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#define LED 2

//actuators
#define INLET_VALVE  0x00
#define OUTLET_VALVE 0x01

//digital sensor output 
#define HIGH_FLOAT 0x10
#define LOW_FLOAT  0x11

//analog sensors output 
#define VOLUME 0x30

#define ENABLE  0x01
#define DISABLE 0x00

//LiquidCrystal_I2C lcd(0x27,16,2);
void setup() {

  Serial.begin(19200);
 //pinMode(LED,OUTPUT);
 
 lcd.init();
 lccd.backlight();
 lcd.clear();
 lcd.home();
 lcd.setCursor(0,0);
 lcd.print("Hello");

 Serial.Write(0xFF);
 

}
unsigned char Value_H
unsigned int volume;
unsigned volumeL, volumeH;
void filing_start(){
  //Enable inlet valve
  Serial.write(INLET_VALVE);
  Serial.write(ENABLE);

  //diable outlet valve
  Serial.write(OUTLET_VALVE);
  Serial.write(DISABLE);
  lcd.setCursor(0,0);
  lcd.print("Filling");

  //disable valve once full
  do{
    //Reading status of high float
    Serial.Write(HIGH_FLOAT);
    Value_H = Serial.read();
    while(!Serial.available());
    Value_H = Serial.read();
    }while(Value_H == 0);
  }

void volume(){
  //read volume
  Serial.write(VOLUME);
  
  }
void loop() {
  //digitalWrite(LED,HIGH);
  //delay(100);
  //digitalWrite(LED,LOW);
  //delay(100);

}
