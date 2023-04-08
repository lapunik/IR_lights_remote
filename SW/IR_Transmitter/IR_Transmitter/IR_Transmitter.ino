#include <Arduino.h>
#include <IRremote.hpp>

#define BUTTON_PIN 4
#define IR_SEND_PIN 3

#define LIGHTS_ADDRESS 0x0
#define LIGHTS_POWER 0xBE41FF00
#define LIGHTS_EVENING 0xBF40FF00
#define LIGHTS_SHINE_UP 0xA35CFF00
#define LIGHTS_SHINE_DOWN 0xA25DFF00a
#define LIGHTS_COLOR_UP 0xBB44FF00
#define LIGHTS_COLOR_DOWN 0xB748FF00
#define LIGHTS_MID_BUTTON 0xF906FF00
#define LIGHTS_THREE_STATES 0xAF50FF00
#define LIGHTS_SLEEP 0xB24DFF00
#define LIGHTS_0_5_HOURS 0xAE51FF00 

int buttonState = 0;

void setup() 
{
  Serial.begin(115200);
  //IrReceiver.begin(IR_PIN_PRIJIMACE);
  IrSender.begin(IR_SEND_PIN);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
}

void loop() 
{
  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH) 
  {
    while(buttonState)
    {
      buttonState = digitalRead(BUTTON_PIN);
    }   
    IrSender.sendNEC(0x0000, 0x41, 0); 
  } 
}
