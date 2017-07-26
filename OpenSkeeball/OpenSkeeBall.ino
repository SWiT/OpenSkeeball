#include "pitches.h"
#include <Servo.h> 

#define H10 A0
#define H20 A1
#define H30 A2
#define H40 A3
#define H50 A4
#define H100 A5

#define LED 13
#define DS1 12
#define DS0 11
#define DA3 10
#define DA2 9
#define DA1 8
#define DA0 7

#define BALLRELEASE 6
#define CLOSEDPOS 154
#define OPENPOS 130

#define SPEAKER 5

Servo BallRelease;

int shaveandahaircut_notes[] = {NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4}; // notes in the melody:
int shaveandahaircut_durations[] = {4,8,8,4,4,4,4,4 }; // note durations: 4 = quarter note, 8 = eighth note, etc.:
int charge_notes[] = {NOTE_C3, NOTE_E3, NOTE_G3, NOTE_G3, NOTE_E3, NOTE_G3}; // notes in the melody:
int charge_durations[] = {8, 8, 8, 4, 8, 4}; // note durations: 4 = quarter note, 8 = eighth note, etc.:

int score;
byte digit_index;
unsigned long current_time, digit_time, digit_delay;
int digits[4];
int d;

int hole10timer = 0;
int hole20timer = 0;
int hole30timer = 0;
int hole40timer = 0;
int hole50timer = 0;
int hole100timer = 0;

  
void setup() {                
  pinMode(LED, OUTPUT); //Status LED 
  
  pinMode(DS1, OUTPUT); //7 segment digit
  pinMode(DS0, OUTPUT);
  
  pinMode(DA3, OUTPUT); //7 segment value
  pinMode(DA2, OUTPUT);
  pinMode(DA1, OUTPUT);
  pinMode(DA0, OUTPUT);  
  
  pinMode(BALLRELEASE, OUTPUT);  //Ball Release Servo  
  pinMode(SPEAKER, OUTPUT);  //Speaker

  BallRelease.attach(BALLRELEASE);
  BallRelease.write(CLOSEDPOS);
  delay(15);
  
  score = 0;
  digit_time = 0;
  digit_delay = 2; //ms
  digit_index = 0;
  
  //Blink Status LED 3 times
  for (byte i=0; i<3; i++) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }
  
  //open ball release servo
  BallRelease.write(OPENPOS);
  //delay(1000);
  
  //play start song
  shaveandahaircut();
  
  //Close Ball Relase
  BallRelease.write(CLOSEDPOS);
  delay(15);
  
  
}

void loop() {
  current_time = millis();  
  //score = current_time/1000;
  
  //figure out the digits to display
  int tmpscore = score;
  digits[0] = tmpscore % 10;
  tmpscore = tmpscore / 10;
  digits[1] = tmpscore % 10;
  tmpscore = tmpscore / 10;
  digits[2] = tmpscore % 10;
  tmpscore = tmpscore / 10;
  digits[3] = tmpscore % 10;
  
  //Draw the digit to the 7 segment display
  if((current_time-digit_time) > digit_delay){
    digit_time = current_time;
    d = digits[digit_index];
    digitalWrite(DA0, bitRead(d,0));
    digitalWrite(DA1, bitRead(d,1));
    digitalWrite(DA2, bitRead(d,2));
    digitalWrite(DA3, bitRead(d,3));
    
    digitalWrite(DS0, bitRead(digit_index,0));
    digitalWrite(DS1, bitRead(digit_index,1));
    
    digit_index++;
    if(digit_index>=4){
      digit_index = 0;
    }
  }

  //read analog pins
  //10 point hole
  if(analogRead(H10)<500){
    if(hole10timer == 0){
      hole10timer = current_time;
    }
    if(current_time - hole10timer > 30){
      //score!!!
      score = score + 10;
      tone(SPEAKER, NOTE_C3 , 500);
      hole10timer = 0;
    }
  }else{
    hole10timer = 0;
  }
  
  //20 point hole
  if(analogRead(H20)<500){
    if(hole20timer == 0){
      hole20timer = current_time;
    }
    if(current_time - hole20timer > 30){
      //score!!!
      score = score + 20;
      tone(SPEAKER, NOTE_E3 , 500);
      hole20timer = 0;
    }
  }else{
    hole20timer = 0;
  }
  
  //30 point hole
  if(analogRead(H30)<500){
    if(hole30timer == 0){
      hole30timer = current_time;
    }
    if(current_time - hole30timer > 30){
      //score!!!
      score = score + 30;
      tone(SPEAKER, NOTE_G3 , 500);
    }
  }else{
    hole30timer = 0;
  }
  
  //40 point hole
  if(analogRead(H40)<500){
    if(hole40timer == 0){
      hole40timer = current_time;
    }
    if(current_time - hole40timer > 30){
      //score!!!
      score = score + 40;
      tone(SPEAKER, NOTE_B3 , 500);
    }
  }else{
    hole40timer = 0;
  }
  
  //50 point hole
  if(analogRead(H50)<500){
    if(hole50timer == 0){
      hole50timer = current_time;
    }
    if(current_time - hole50timer > 30){
      //score!!!
      score = score + 50;
      tone(SPEAKER, NOTE_C4 , 500);
    }
  }else{
    hole50timer = 0;
  }
  
  //100 point hole
  if(analogRead(H100)<500){
    if(hole100timer == 0){
      hole100timer = current_time;
    }
    if(current_time - hole100timer > 30){
      //score!!!
      score = score + 100;
      charge();
    }
  }else{
    hole100timer = 0;
  }

  
}

void shaveandahaircut() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/shaveandahaircut_durations[thisNote];
    tone(SPEAKER, shaveandahaircut_notes[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(SPEAKER);
  }
}

void charge() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 6; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/charge_durations[thisNote];
    tone(SPEAKER, charge_notes[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(SPEAKER);
  }
}


