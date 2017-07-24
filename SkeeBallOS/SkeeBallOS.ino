// Open Skeeball v 1.1
// By: Matthew G. Switlik
// 2017-07-24

#include <Servo.h> 

#define BallReleaseOpen 0
#define BallReleaseClosed 90

#define BallReleasePin 6

#define DA0Pin 7
#define DA1Pin 8
#define DA2Pin 9
#define DA3Pin 10

#define DS0Pin 11
#define DS1Pin 12

#define LEDPin 13

#define Hole10Pin 14
#define Hole20Pin 15
#define Hole30Pin 16
#define Hole40Pin 17
#define Hole50Pin 18
#define Hole100Pin 19

unsigned long MaxThreshold = 200

Servo BallRelease;  //Servo for Ball Release
byte ServoPos = ServoPosUp;   //degrees for up position

byte nSwitches = 6; //Total Number of Switches
byte aSwitchPins[]  = {14, 15, 16, 17, 18, 19};
boolean aSwitchHit[]    = {false,  false, false,  false, false, false};  //var for hit detection
unsigned long SwitchThreshold[]  = {0,  0,  0,  0, 0, 0}; //var for tracking the length of a hit (decreases false positives)
                                                          //, 0 means timer not started


unsigned long now = 0; //Time of loops beginning
unsigned long timeLEDOn = 0; //Time the LED has been on
unsigned int gamescore = 0;
unsigned int ballCount = 0;

void setup() 
{ 
  // Start the serial connection.
  Serial.begin(9600);

  // Attach the ball release servo.
  BallRelease.attach(BallReleasePin);

  // Set all the switch pins to inputs
  for(i=0; i < nSwitches; i++)
  {
    pinMode(aSwitchPins[i], INPUT);
  }

  // Set all the 7 segment display pins to outputs.
  pinMode(DA0Pin, OUTPUT);
  pinMode(DA1Pin, OUTPUT);
  pinMode(DA2Pin, OUTPUT);
  pinMode(DA3Pin, OUTPUT);
  pinMode(DS0Pin, OUTPUT);
  pinMode(DS1Pin, OUTPUT);

  // Set the status LED as an output.
  pinMode(LEDPin, OUTPUT);

  // Blink 5 times at startup.
  for (byte i = 0; i < 5; i++)
  {
    digitalWrite(LEDPin, HIGH);
    delay(200);
    digitalWrite(LEDPin, LOW);
    delay(200);
  }
  
  Serial.print("OpenSkeeball v1.1\n");
} 
 
 
void loop() 
{  
    now = millis();
    
    for(i=0; i < nSwitches; i++)
    {
      aSwitchHit[i] = bool(digitalRead(aSwitchPins[i]));
      Serial.print("Switch:");
      Serial.print(i);
      Serial.print(", ");
      Serial.print(aSwitchHit[i]);
      Serial.print("\n");
      
      if(aSwitchHit[Curr])
      {
        if(SwitchThreshold[Curr] == 0)
        {
          SwitchThreshold[Curr] = time;
        }
        
        if( (time-SwitchThreshold[Curr]) >= MaxThreshold)
        {
          //Hit!!!
          TotalScore += (10 + Curr*10);
          digitalWrite(LEDPin, HIGH);
          timeLEDOn = time;
          nBalls++;
          SwitchThreshold[Curr] = 0;
        }
      }
      else
      {
        SwitchThreshold[Curr] = 0;
      }
    }
    
    //if status LED has been on for proper amount turn it off
    if(time - timeLEDOn > 1000)
    {
      digitalWrite(LEDPin, LOW);
      timeLEDOn = 0;
    }
    
    Servo0.write(ServoPos); 
    delay(1);
} 

