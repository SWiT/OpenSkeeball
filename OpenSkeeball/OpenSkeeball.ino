#include "pitches.h"
#include <Servo.h> 

// Arduino pin usage
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
#define SPEAKER 5

// Constants for the OpenSkeeball shield hardware
#define CLOSEDPOS 154
#define OPENPOS 130
#define DIGITDELAY 2

Servo BallRelease;

int shaveandahaircut_notes[] = {NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4}; // notes in the melody:
int shaveandahaircut_durations[] = {4,8,8,4,4,4,4,4 }; // note durations: 4 = quarter note, 8 = eighth note, etc.:
int charge_notes[] = {NOTE_C3, NOTE_E3, NOTE_G3, NOTE_G3, NOTE_E3, NOTE_G3}; // notes in the melody:
int charge_durations[] = {8, 8, 8, 4, 8, 4}; // note durations: 4 = quarter note, 8 = eighth note, etc.:

int score;
byte digitindex;
unsigned long now;
unsigned long digittime;
byte digits[4];

unsigned long h10time = 0;
unsigned long h20time = 0;
unsigned long h30time = 0;
unsigned long h40time = 0;
unsigned long h50time = 0;
unsigned long h100time = 0;

bool h10ready = false;
bool h20ready = false;
bool h30ready = false;
bool h40ready = false;
bool h50ready = false;
bool h100ready = false;

void setup() {                
  pinMode(LED, OUTPUT); //Status LED 
  
  pinMode(DS1, OUTPUT); //7 segment digits
  pinMode(DS0, OUTPUT);
  
  pinMode(DA3, OUTPUT); //7 segment values
  pinMode(DA2, OUTPUT);
  pinMode(DA1, OUTPUT);
  pinMode(DA0, OUTPUT);  
  
  pinMode(BALLRELEASE, OUTPUT);  //Ball Release Servo  
  pinMode(SPEAKER, OUTPUT);  //Speaker

  BallRelease.attach(BALLRELEASE);
  BallRelease.write(CLOSEDPOS);
  delay(15);
  
  score = 0;
  digittime = 0;
  digitindex = 0;
  
  // Blink Status LED 3 times
  for (byte i=0; i<3; i++) {
    digitalWrite(LED, HIGH);
    delay(150);
    digitalWrite(LED, LOW);
    delay(150);
  }
  
  // Open ball release servo
  BallRelease.write(OPENPOS);
  
  // Play start song
  playShaveHaircut();
  
  // Close Ball Relase
  BallRelease.write(CLOSEDPOS);
  delay(15);
  
  
}

void loop() {
  now = millis();  
  
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
  if((now - digittime) > DIGITDELAY){
    digittime = now;
    byte d = digits[digitindex];
    digitalWrite(DA0, bitRead(d,0));
    digitalWrite(DA1, bitRead(d,1));
    digitalWrite(DA2, bitRead(d,2));
    digitalWrite(DA3, bitRead(d,3));
    
    digitalWrite(DS0, bitRead(digitindex,0));
    digitalWrite(DS1, bitRead(digitindex,1));
    
    digitindex++;
    if(digitindex>=4){
      digitindex = 0;
    }
  }

  // Check each hole if it is blocked
  checkhole(H10, h10ready, h10time, 10, NOTE_C3);     // 10 point hole
  checkhole(H20, h20ready, h20time, 20, NOTE_E3);     // 20 point hole
  checkhole(H30, h30ready, h30time, 30, NOTE_G3);     // 30 point hole
  checkhole(H40, h40ready, h40time, 40, NOTE_B3);     // 40 point hole
  checkhole(H50, h50ready, h50time, 50, NOTE_C4);     // 50 point hole
  checkhole(H100, h100ready, h100time, 100, CHARGE);  // 100 point holes
}

void checkhole(byte holepin, boolean &holeready, unsigned long &holetimer, byte holeval, unsigned int holesound) {
  // Read analog pin and if it's less than 500 (out of 1023)
  if(analogRead(holepin) < 500){
    // If the hole's timer isn't set, set it to now
    if(holetimer == 0){
      holetimer = now;
    }

    // If the hole is ready and has been blocked for 30 ms
    if(holeready && now - holetimer > 30){
      // Add the holes value to the score.
      score = score + holeval;

      // Play the sound for the hole.
      if (holesound == CHARGE){
        playCharge();
      } else {
        tone(SPEAKER, holesound, 500);
      }

      holetimer = 0;      // Reset the hole's timer
      holeready = false;  // Set the hole to not ready to prevent multiple scores from one ball.
    }
  }else{    
    holetimer = 0;    // Reset the hole's timer
    holeready = true; // The hole needs to be unblocked between scores to be set ready
  }
}
  
void playShaveHaircut() {
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

void playCharge() {
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


