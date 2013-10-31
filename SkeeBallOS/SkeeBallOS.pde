// Skee Ball OS
// By: Matthew G. Switlik
// 2010-05-25
const char sVersion[] = "{\"version\":\"SkeeBallOS v0.01\"}";

#include <Servo.h> 
int MaxThreshold = 2000;   //ms the switch needs to be disconnected to trigger a hit (decreases false positives)

const int nSwitches = 5; //Total Number of Switch
int Curr = 0;  //Current Switch

Servo Servo0;  //Servo for Ball Release
int ServoPosUp   = 0;     //degrees for up position
int ServoPosDown = 90;    //degrees for down position
int BallReleaseServoPin = 9; //Servo pin #
int ServoPos = ServoPosUp;   //degrees for up position

unsigned int aSwitchPins[] = {6, 5, 4, 3, 2};
boolean aSwitchHit[]    = {false,  false, false,  false, false};  //var for hit detection

unsigned int LEDPin = 13;

unsigned long time = 0; //Time of loops beginning
unsigned long timeLEDOn = 0; //Time the LED has been on
unsigned long timeReceived = 0; //Time of last Serial Input
unsigned long SwitchThreshold[]  = {0,  0,  0,  0, 0};    //var for tracking the length of a hit (decreases false positives)
                                                       //, 0 means timer not started
unsigned long timeGameStarted = 0;
unsigned int TotalScore = 0;
unsigned int nBalls = 0;

const int maxCommand = 15;
char command[maxCommand];         // incoming command
int commandIndex = 0;

void setup() 
{ 
  Serial.begin(9600);
  
  Servo0.attach(BallReleaseServoPin);
  
  for(Curr=0; Curr < nSwitches; Curr++)
  {
    pinMode(aSwitchPins[Curr], INPUT);
  }
  
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, HIGH); delay(200);
  digitalWrite(LEDPin, LOW);  delay(200);
  digitalWrite(LEDPin, HIGH); delay(200);
  digitalWrite(LEDPin, LOW);  delay(200);
  digitalWrite(LEDPin, HIGH); delay(200);
  digitalWrite(LEDPin, LOW);  delay(200);
  timeGameStarted = millis();
} 
 
 
void loop() 
{  
    time = millis();
    
    if (Serial.available() > 0)
    {
      command[commandIndex++] = Serial.read();
      if(command[(commandIndex-1)]==';')
        runCommand();
      else if(commandIndex>=maxCommand)
        unknownCommand();
      else if(timeReceived > 0 && (time-timeReceived) > 1000)
        clearCommand();
      else
        timeReceived = time;
    }
    
    for(Curr=0; Curr < nSwitches; Curr++)
    {
      aSwitchHit[Curr] = !bool(digitalRead(aSwitchPins[Curr]));
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

void runCommand()
{
  char param1[maxCommand-2];
  char param2[maxCommand-2];
  
  boolean bSuccess;
  int i;

  parseString(command, param1);
  parseString(param1, param2);
  
  if(strcmp(command,"getVersion")==0)
  {
      sendVersion();
  }
  else if(strcmp(command,"getGameStatus")==0)
  {
      sendGameStatus();
  }
  else
  {
      unknownCommand();
  }
  clearCommand();
}

void sendVersion()
{
  Serial.print(sVersion);
}

void sendGameStatus()
{
  Serial.print("{\"timeGameStarted\":");
  Serial.print(timeGameStarted);
  Serial.print(",\"currentTime\":");
  Serial.print(time);
  Serial.print(",\"nBalls\":");
  Serial.print(nBalls);
  Serial.print(",\"TotalScore\":");
  Serial.print(TotalScore);
  Serial.print("}");
  Serial.print("\n"); //for terminal testing
}



void unknownCommand()
{
  Serial.print("{\"error\":\"Unknown command:'");
  Serial.print(command);
  Serial.print("\"}");
}

void clearCommand()
{
  for(int i=0; i<maxCommand; i++)
  {
    command[i] = 0;
  }
  commandIndex = 0;
  timeReceived = 0;
}

void parseString(char str1[], char str2[])
{
  int paramStart = -1;
  for(int i=0; i<maxCommand; i++)
  {
    if(str1[i] == 0)
      break;
    
    if(str1[i] == ';')
    {
      str1[i] = 0;
      break;
    }
    
    if(str1[i] == ',' && paramStart == -1)
      paramStart = i;
     
    if(i > paramStart)
    {
      str2[i-paramStart-1] = str1[i];
      str2[i-paramStart] = 0;
    }
    
    if(paramStart > 0)
      str1[i] = 0;
  }
}

