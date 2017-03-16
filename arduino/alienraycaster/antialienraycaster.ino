#include <Servo.h>

bool gReverseDirection = false;

Servo servoX;
Servo servoY;
int valX = 90;
int valXOld;
int valY = 90;
int valYOld;

int Energy = 100;
int OldEnergy = 0;

int mode = 1;  // 0=nothing, 1=shootRandomDirection



#define TROUND
#ifdef TSQUARE
uint8_t pathX[] = {50, 140, 90};
uint8_t pathY[] = {20, 70, 50};
#endif
#ifdef TROUND
uint8_t pathX[] = {50, 90, 130, 50};
uint8_t pathY[] = {50, 100, 60, 50};
#endif


void setup() {
  Serial.begin(2400);
  Serial.println(sizeof(pathX));
  
  delay(3000); // sanity delay
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

  switch(mode) {
    case 0:
      nothing();
      break;
    case 1:
      shootRandomDirection();
      break;
  }

  
  //FastLED.show(); // display this frame
  //FastLED.delay(1000 / FRAMES_PER_SECOND);
}
void nothing() {
  if(valX != valXOld) {
    valXOld = valX;
    servoX.write(valX);
  }
  if(valY != valYOld) {
    valYOld = valY;
    servoY.write(valY);
  }
}

uint8_t pathS[] = {10, 20, 5, 8};
int pathIndex = 0;
uint32_t pathTicks = 0;

void shootRandomDirection() {
  static uint16_t timestamp = millis();

  if(((uint16_t)millis()-timestamp) > 1) {
    timestamp += 1;
    int pathIndex2 = pathIndex+1;
    if(pathIndex2>=sizeof(pathX)) pathIndex2 = 0;
    int valX = map(pathTicks, 0,50000, pathX[pathIndex], pathX[pathIndex2]);
    int valY = map(pathTicks, 0,50000, pathY[pathIndex], pathY[pathIndex2]);
    pathTicks += pathS[pathIndex];
    if(pathTicks > 50000) {
      pathTicks = 0;
      pathIndex++;
      if(pathIndex>=sizeof(pathX)) pathIndex = 0;
      Serial.println(pathIndex);
    }
    servoX.write(valX);
    servoY.write(valY);
  }
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