
int score, digit_index;
long current_time, digit_time, digit_delay;
int digits[4];
int d;


#include "pitches.h"
int shaveandahaircut_notes[] = {NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4}; // notes in the melody:
int shaveandahaircut_durations[] = {4,8,8,4,4,4,4,4 }; // note durations: 4 = quarter note, 8 = eighth note, etc.:
int charge_notes[] = {NOTE_C3, NOTE_E3, NOTE_G3, NOTE_G3, NOTE_E3, NOTE_G3}; // notes in the melody:
int charge_durations[] = {8, 8, 8, 4, 8, 4}; // note durations: 4 = quarter note, 8 = eighth note, etc.:


#include <Servo.h> 
Servo BallRelease;
int ClosedPos = 154;
int OpenPos = 130;

int hole10 = 0;
int hole20 = 1;
int hole30 = 2;
int hole40 = 3;
int hole50 = 4;
int hole100 = 5;
int hole10timer = 0;
int hole20timer = 0;
int hole30timer = 0;
int hole40timer = 0;
int hole50timer = 0;
int hole100timer = 0;

  
void setup() {                
  pinMode(13, OUTPUT); //Status LED 
  
  pinMode(12, OUTPUT); //7 segment digit
  pinMode(11, OUTPUT);
  
  pinMode(10, OUTPUT); //7 segment value
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);  
  
  pinMode(6, OUTPUT);  //Ball Release Servo  
  pinMode(5, OUTPUT);  //Speaker

  BallRelease.attach(6);
  BallRelease.write(ClosedPos);
  delay(15);
  
  score = 0;
  digit_time = 0;
  digit_delay = 2; //ms
  digit_index = 0;
  
  //Blink Status LED 3 times
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  
  //open ball release servo
  BallRelease.write(OpenPos);
  //delay(1000);
  
  //play start song
  shaveandahaircut();
  
  //Close Ball Relase
  BallRelease.write(ClosedPos);
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
    digitalWrite(7, bitRead(d,0));
    digitalWrite(8, bitRead(d,1));
    digitalWrite(9, bitRead(d,2));
    digitalWrite(10,bitRead(d,3));
    
    digitalWrite(11, bitRead(digit_index,0));
    digitalWrite(12, bitRead(digit_index,1));
    
    digit_index++;
    if(digit_index>=4){
      digit_index = 0;
    }
  }

  //read analog pins
  //10 point hole
  if(analogRead(hole10)<500){
    if(hole10timer == 0){
      hole10timer = current_time;
    }
    if(current_time - hole10timer > 30){
      //score!!!
      score = score + 10;
      tone(5, NOTE_C3 , 500);
      hole10timer = 0;
    }
  }else{
    hole10timer = 0;
  }
  
  //20 point hole
  if(analogRead(hole20)<500){
    if(hole20timer == 0){
      hole20timer = current_time;
    }
    if(current_time - hole20timer > 30){
      //score!!!
      score = score + 20;
      tone(5, NOTE_E3 , 500);
      hole20timer = 0;
    }
  }else{
    hole20timer = 0;
  }
  
  //30 point hole
  if(analogRead(hole30)<500){
    if(hole30timer == 0){
      hole30timer = current_time;
    }
    if(current_time - hole30timer > 30){
      //score!!!
      score = score + 30;
      tone(5, NOTE_G3 , 500);
    }
  }else{
    hole30timer = 0;
  }
  
  //40 point hole
  if(analogRead(hole40)<500){
    if(hole40timer == 0){
      hole40timer = current_time;
    }
    if(current_time - hole40timer > 30){
      //score!!!
      score = score + 40;
      tone(5, NOTE_B3 , 500);
    }
  }else{
    hole40timer = 0;
  }
  
  //50 point hole
  if(analogRead(hole50)<500){
    if(hole50timer == 0){
      hole50timer = current_time;
    }
    if(current_time - hole50timer > 30){
      //score!!!
      score = score + 50;
      tone(5, NOTE_C4 , 500);
    }
  }else{
    hole50timer = 0;
  }
  
  //100 point hole
  if(analogRead(hole100)<500){
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
    tone(5, shaveandahaircut_notes[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(5);
  }
}

void charge() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/charge_durations[thisNote];
    tone(5, charge_notes[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(5);
  }
}


