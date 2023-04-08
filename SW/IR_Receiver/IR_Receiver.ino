#define IR_PIN_PRIJIMACE 2
#define BUTTON_PIN 4
#define LED_PIN 5
#define IR_SEND_PIN 3

#include <Arduino.h>
#include <IRremote.hpp>

#define DECODE_DENON
#define DECODE_JVC
#define DECODE_KASEIKYO
#define DECODE_PANASONIC
#define DECODE_LG
#define DECODE_NEC
#define DECODE_SAMSUNG
#define DECODE_SONY
#define DECODE_RC5
#define DECODE_RC6

#define DECODE_BOSEWAVE
#define DECODE_LEGO_PF
#define DECODE_MAGIQUEST
#define DECODE_WHYNTER 


void setup() 
{
  Serial.begin(115200);
  IrReceiver.begin(IR_PIN_PRIJIMACE);  
}

void loop() 
{
  
 if(IrReceiver.decode())
  {    
    
    Serial.print("Data: ");
    Serial.println(IrReceiver.decodedIRData.decodedRawData,HEX);
          switch (IrReceiver.decodedIRData.protocol){
            case NEC: Serial.println("NEC"); break ;
            case SONY: Serial.println("SONY"); break ;
            case RC5: Serial.println("RC5"); break ;
            case RC6: Serial.println("RC6"); break ;
            case SHARP: Serial.println("SHARP"); break ;
            case JVC: Serial.println("JVC"); break ;
            case SAMSUNG: Serial.println("SAMSUNG"); break ;
            case LG: Serial.println("LG"); break ;
            case WHYNTER: Serial.println("WHYNTER"); break ;
            case PANASONIC: Serial.println("PANASONIC"); break ;
            case DENON: Serial.println("DENON"); break ;
          default:
            case UNKNOWN: Serial.println("UNKNOWN"); break ;
          }
  Serial.print("Adresa: ");
  Serial.println(IrReceiver.decodedIRData.address);
  Serial.println(IrReceiver.decodedIRData.command); // Decoded command
  Serial.println(IrReceiver.decodedIRData.extra); // Used for Kaseikyo unknown vendor ID. Ticks used for decoding Distance protocol.
  Serial.println(IrReceiver.decodedIRData.numberOfBits);// Number of bits received for data (address + command + parity) - to determine protocol length if different length are possible.
  Serial.println(IrReceiver.decodedIRData.flags); //See IRDATA_FLAGS_* definitions 
  Serial.println("------------------------------------");
  IrReceiver.resume();
  
  }
}
