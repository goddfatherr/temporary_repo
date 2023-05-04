/*************************************************************
Title         :   Home automation using blynk
Description   :   To control light's brigntness with brightness,monitor temperature , monitor water level in the tank through blynk app
Pheripherals  :   Arduino UNO , Temperature system, LED, LDR module, Serial Tank, Blynk cloud, Blynk App.
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPL3FOVLBhXn"
#define BLYNK_DEVICE_NAME "Home Automation"
#define BLYNK_AUTH_TOKEN "vhs7C5Sy2PQlUMiMOm9X9m1pcSv4kAuU"

// Comment this out to disable prints 
#define BLYNK_PRINT Serial

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "main.h"
#include "temperature_system.h"
#include "ldr.h"
#include "serial_tank.h"

char auth[] = BLYNK_AUTH_TOKEN;
bool heater_sw,cooler_sw,inlet_sw,outlet_sw;
unsigned int tank_volume;

BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

// This function is called every time the Virtual Pin 0 state changes
/*To turn ON and OFF cooler based virtual PIN value and print notification to clcd*/
BLYNK_WRITE(COOLER_V_PIN)
{
  //read cooler value
  cooler_sw = param.asInt();
  if(cooler_sw){
    cooler_control(ON);
    lcd.setCursor(7,0);
    lcd.print("CO_LR ON");
  }
  else{
    cooler_control(OFF);
    lcd.setCursor(7,0);
    lcd.print("CO_LR OFF");
  }  
}
/*To turn ON and OFF heater based virtual PIN value and print notification to clcd*/
BLYNK_WRITE(HEATER_V_PIN )
{
  //read heater value
  heater_sw = param.asInt();
  if(heater_sw){
    heater_control(ON);
    lcd.setCursor(7,0);
    lcd.print("HT_R ON");
  }
  else{
    heater_control(OFF);
    lcd.setCursor(7,0);
    lcd.print("HT_R OFF");
  }  
}
/*To turn ON and OFF inlet vale based virtual PIN value*/
BLYNK_WRITE(INLET_V_PIN)
{
  
}
/*To turn ON and OFF outlet value based virtual switch value*/
BLYNK_WRITE(OUTLET_V_PIN)
{
  
}
/* To display temperature and water volume as gauge on the Blynk App*/  
void update_temperature_reading()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(TEMPERATURE_SENSOR,read_temperature());

  
}

/*To turn off the heater if the temperature raises above 35 deg C*/
void handle_temp(void)
{
  if(read_temperature() > float(35) && heater_sw){
    heater_control(OFF);
    //print heater status on dashboard
    lcd.setCursor(7, 0);
    lcd.print("HT_R OFF");

    //print notification on blynk clound 
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Temperature is above 350 C\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Turing off the heater\n");

    //update pin status on cloud
    Blynk.virtualWrite(HEATER_V_PIN, 0);

  }
}

/*To control water volume above 2000ltrs*/
void handle_tank(void)
{
  

}


void setup(void)
{
  //Configure Garden Light
  init_ldr();

  //init lcd
  lcd.init();                     
  lcd.backlight();
  lcd.clear();
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print("T=");

  //Connect to blynk cloud
  Blynk.begin(auth);

  //init temp system
  init_temperature_system();

  //update temperature every 0.5s to blynk cloud
  timer.setInterval(500L,update_temperature_reading);
  pinMode(0,OUTPUT);
}

String temperature;
void loop(void) 
{
  Blynk.run();
  //control LED brightness
  brightness_control();
  temperature = String(read_temperature(),2);
  lcd.setCursor(2,0);
  lcd.print(temperature);
}
