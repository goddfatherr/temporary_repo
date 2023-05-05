#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

#define BLYNK_TEMPLATE_ID "TMPL3Ah469yDM"
#define BLYNK_TEMPLATE_NAME "Home Automation"
#define BLYNK_AUTH_TOKEN "B-WRhO60vi_gHYKbXkcgDDoeyO9wVEja"

#define LED 2

//Write Virtual Pin
BLYNK_WRITE(V0){
  int value = param.asInt();
  if(value){
    digitalWrite(LED,HIGH);
    }
    else{
      digitalWrite(LED,LOW);
      }
  }
char auth[] = BLYNK_AUTH_TOKEN;
void setup() {
  // put your setup code here, to run once:
  Blynk.begin(auth);
  pinMode(LED,OUTPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();

}
