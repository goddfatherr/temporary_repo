/*************************************************************
Title         :   Home automation using blynk
Description   :   To control light's brigntness with brightness,monitor temperature , monitor water level in the tank through blynk app
Pheripherals  :   Arduino UNO , Temperature system, LED, LDR module, Serial Tank, Blynk cloud, Blynk App.
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
//#define BLYNK_TEMPLATE_ID "TMPL3FOVLBhXn"
//#define BLYNK_DEVICE_NAME "Home Automation"
//#define BLYNK_AUTH_TOKEN "vhs7C5Sy2PQlUMiMOm9X9m1pcSv4kAuU"

#define BLYNK_TEMPLATE_ID "TMPL3Ah469yDM"
#define BLYNK_TEMPLATE_NAME "Home Automation"
#define BLYNK_AUTH_TOKEN "B-WRhO60vi_gHYKbXkcgDDoeyO9wVEja"

// Comment this out to disable prints 
//#define BLYNK_PRINT Serial

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
    lcd.print("CO_LR ON ");
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
    lcd.print("HT_R ON  ");
  }
  else{
    heater_control(OFF);
    lcd.setCursor(7,0);
    lcd.print("HT_R OFF ");
  }  
}
/*To turn ON and OFF inlet vale based virtual PIN value*/
BLYNK_WRITE(INLET_V_PIN)
{
  inlet_sw = param.asInt();
  if(inlet_sw){
    enable_inlet();
    lcd.setCursor(7,1);
    lcd.print("IN_FL_ON ");
  }
  else{
    disable_inlet();
    lcd.setCursor(7,1);
    lcd.print("IN_FL_OFF");
  }
}
/*To turn ON and OFF outlet value based virtual switch value*/
BLYNK_WRITE(OUTLET_V_PIN)
{
  outlet_sw = param.asInt();
  if(outlet_sw){
    enable_outlet();
    lcd.setCursor(7,1);
    lcd.print("OT_FL_ON ");
  }
  else{
    disable_outlet();
    lcd.setCursor(7,1);
    lcd.print("OT_FL_OFF");
  }
}
/* To display temperature and water volume as gauge on the Blynk App*/  
void update_temperature_reading()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(TEMPERATURE_SENSOR,read_temperature());
  Blynk.virtualWrite(WATER_VOL_GAUGE,volume());

  
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
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Turing off the heater...\n");

    //update pin status on cloud
    Blynk.virtualWrite(HEATER_V_PIN, 0);

  }
}

/*To control water volume above 2000ltrs*/
void handle_tank(void)
{
  if((tank_volume < 2000) && (inlet_sw == 0)){
    enable_inlet();
    inlet_sw = 1;
    //outlet_sw = 0;

    //update dashboard
    lcd.setCursor(7,1);
    lcd.print("IN_FL_ON ");

    //send notification to cloud
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Volume is above 2000.\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Turning on inlet valve...\n");

    //update blynk cloud
    Blynk.virtualWrite(INLET_V_PIN, 1);
  }

  //if tank is full and inlet valve is on
  if((tank_volume ==3000) && (inlet_sw == 0)){
    disable_inlet();
    inlet_sw = 0;
    //outlet_sw = 0;

    //update dashboard
    lcd.setCursor(7,1);
    lcd.print("IN_FL_ON ");

    //send notification to cloud
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Tanks is full.\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Turning off inlet valve...\n");

    //update blynk cloud
    Blynk.virtualWrite(INLET_V_PIN, 0);
  }


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
  //dispaly temp
  lcd.setCursor(0, 0);
  lcd.print("T=");
  //display volume
  lcd.setCursor(0, 1);
  lcd.print("V=");

  //Connect to blynk cloud
  Blynk.begin(auth);

  //init temp system
  init_temperature_system();

  //init serial tank
  init_serial_tank();

  //update temperature every 0.5s to blynk cloud
  timer.setInterval(500L,update_temperature_reading);
  pinMode(0,OUTPUT);
}

String temperature;
void loop(void) 
{
  Blynk.run();

  //enable timer
  timer.run();

  //control LED brightness
  brightness_control();
  temperature = String(read_temperature(),2);

  //display temp
  lcd.setCursor(2,0);
  lcd.print(temperature);

  //display volume
  tank_volume = volume();
  lcd.setCursor(2,1);
  lcd.print(tank_volume);

  //threshold requirements
  handle_temp();
  handle_tank();
}
