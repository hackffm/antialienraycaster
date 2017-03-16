//#include <FastLED.h>
#include <Servo.h>

/*#define LED_PIN     6
#define COLOR_ORDER GRB
#define CHIPSET     WS2812
#define NUM_LEDS    16

#define BRIGHTNESS  200
#define FRAMES_PER_SECOND 60
*/
bool gReverseDirection = false;

//CRGB leds[NUM_LEDS];

Servo servoX;
Servo servoY;
int valX = 90;
int valXOld;
int valY = 90;
int valYOld;

int Energy = 100;
int OldEnergy = 0;

void setup() {
  Serial.begin(2400);
  
  delay(3000); // sanity delay
  //FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  //FastLED.setBrightness( BRIGHTNESS );

  servoX.attach(9);
  servoY.attach(10); 
}

void loop()
{
  receive_serial_cmd();

  // test if energy has changed
  if(Energy != OldEnergy) {
    OldEnergy = Energy;
    // Energy level has been changed - react here
    Serial.println("Energy changed!");
  }
  if(valX != valXOld) {
    valXOld = valX;
    servoX.write(valX);
  }
  if(valY != valYOld) {
    valYOld = valY;
    servoY.write(valY);
  }

  /*valX = analogRead(A0);            
  valX = map(valX, 0, 1023, 0, 180);     
  servoX.write(valX);

  valY = analogRead(A1);            
  valY = map(valY, 0, 1023, 0, 180);     
  servoY.write(valY);*/
  
  //FastLED.show(); // display this frame
  //FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void receive_serial_cmd(void) {
  static uint8_t cmd[18];
  static uint8_t cmdcount = 0;
  uint8_t c;
  while(Serial.available()) {
    c = Serial.read();
    if(c > ' ') cmd[cmdcount++] = c;
    if((c == 8) && (cmdcount > 0)) cmdcount--;
    if((c == 0x0d) || (c == 0x0a) || (cmdcount > 16)) {
      cmd[cmdcount] = 0;
      if(cmdcount > 0) {
       switch(cmd[0]) {
        case 'e': 
          if((cmdcount > 1) && (cmdcount < 7)) {
            int temp = atoi((const char *)&cmd[1]);           
            Energy = constrain(temp, 0, 100);
            Serial.print("Value received:");
            Serial.print(temp); 
            Serial.print(", Energy set to:");
            Serial.println(Energy);                       
          }                    
          break; 
          case 'x':
           if((cmdcount > 1) && (cmdcount < 7)) {
            int temp = atoi((const char *)&cmd[1]);           
            valX = constrain(temp, 0, 180);
            Serial.print("x:");
            Serial.print(temp);                        
          }  
          break;
          case 'y':
           if((cmdcount > 1) && (cmdcount < 7)) {
            int temp = atoi((const char *)&cmd[1]);           
            valY = constrain(temp, 0, 180);
            Serial.print("y:");
            Serial.print(temp);                        
          }  
          break;                   
       }    
      }
      cmdcount = 0;
    } 
  }
}