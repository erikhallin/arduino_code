//version 1.0

#include<Wire.h>
#include "pitches.h"
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,GyX,GyY,GyZ;
float AcXs,AcYs,AcZs,GyXs,GyYs,GyZs;
int redPin = 9;
int greenPin = 10;
int bluePin = 11;
int buzzer_pin = 6;
int ac_start_x,ac_start_y,ac_start_z,gy_start_x,gy_start_y,gy_start_z;
int dead_zone = 7000;
int warning_zone = 3000;
int warning_level_1 = 3500;
int warning_level_2 = 4000;
int warning_level_3 = 4500;
int warning_level_4 = 5000;
int warning_level_5 = 6000;
bool gameover=false;
int smoothing = 10;
int step_counter = smoothing;
int delay_time = 10;

int led_red=0; int led_green=255; int led_blue=0;

int melody_start[] = 
{
  NOTE_G4, NOTE_G4, NOTE_G4
};
int noteDurations_start[] = 
{
  8, 8, 8
};
int melody_end[] = 
{
  NOTE_F3, NOTE_C3
};
int noteDurations_end[] = 
{
  4, 4
};
int melody_warning[] = 
{
  NOTE_AS4
};
int noteDurations_warning[] = 
{
  8
};

void setup()
{
  //buzzer setup
  pinMode(buzzer_pin, OUTPUT);  
  //start noise  
  playTuneStart(3);
  
  //gyro setup
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  //read gyro start values
  AcXs=AcYs=AcZs=GyXs=GyYs=GyZs=0;
  for(int i=0;i<smoothing;i++)
  {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  
    Wire.endTransmission(false);
    Wire.requestFrom(MPU,12,true);  
    AcXs+=Wire.read()<<8|Wire.read();    
    AcYs+=Wire.read()<<8|Wire.read();  
    AcZs+=Wire.read()<<8|Wire.read(); 
    GyXs+=Wire.read()<<8|Wire.read(); 
    GyYs+=Wire.read()<<8|Wire.read(); 
    GyZs+=Wire.read()<<8|Wire.read(); 
    delay(delay_time);
  }
  ac_start_x=int(AcXs/(float)smoothing);
  ac_start_y=int(AcYs/(float)smoothing); 
  ac_start_z=int(AcZs/(float)smoothing); 
  gy_start_x=int(GyXs/(float)smoothing);
  gy_start_y=int(GyYs/(float)smoothing); 
  gy_start_z=int(GyZs/(float)smoothing); 
  AcXs=AcYs=AcZs=GyXs=GyYs=GyZs=0;
  

  /*//debug console
  Serial.begin(9600);
  Serial.print("ACC: X = "); Serial.print(ac_start_x);
  Serial.print(" | Y = "); Serial.print(ac_start_y);
  Serial.print(" | Z = "); Serial.println(ac_start_z); 
  Serial.print("GYR: X = "); Serial.print(gy_start_x);
  Serial.print(" | Y = "); Serial.print(gy_start_y);
  Serial.print(" | Z = "); Serial.println(gy_start_z);*/ 
  
  //rgb led setup
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT); 
  //set player color
  setColor(led_red,led_green,led_blue);
}

void loop()
{
  if(!gameover)
  {
    //read gyro
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  
    Wire.endTransmission(false);
    Wire.requestFrom(MPU,12,true);  
    AcX=Wire.read()<<8|Wire.read();    
    AcY=Wire.read()<<8|Wire.read();  
    AcZ=Wire.read()<<8|Wire.read();  
    GyX=Wire.read()<<8|Wire.read();  
    GyY=Wire.read()<<8|Wire.read();  
    GyZ=Wire.read()<<8|Wire.read(); 
    AcXs+=AcX; AcYs+=AcY; AcZs+=AcZ;
    GyXs+=GyX; GyYs+=GyY; GyZs+=GyZ;

    step_counter-=1;
    if(step_counter<1)
    {
      AcX=AcXs/(float)smoothing;
      AcY=AcYs/(float)smoothing;
      AcZ=AcZs/(float)smoothing;
      GyX=GyXs/(float)smoothing;
      GyY=GyYs/(float)smoothing;
      GyZ=GyZs/(float)smoothing;
      AcXs=AcYs=AcZs=GyXs=GyYs=GyZs=0;
      
      step_counter=smoothing;
    

      if(GyX>gy_start_x+dead_zone || GyX<gy_start_x-dead_zone ||
         GyY>gy_start_y+dead_zone || GyY<gy_start_y-dead_zone ||
         GyZ>gy_start_z+dead_zone || GyZ<gy_start_z-dead_zone)
      /*if(AcX>ac_start_x+dead_zone || AcX<ac_start_x-dead_zone ||
         AcY>ac_start_y+dead_zone || AcY<ac_start_y-dead_zone ||
         AcZ>ac_start_z+dead_zone || AcZ<ac_start_z-dead_zone)*/
      {
        gameover=true;
  
        //play sound
        playTuneEnd(2);
        
        //turn off color
        ledFadeOut();
        setColor(0,0,0);
      }
      else //check if warning
      {
        if(GyX>gy_start_x+warning_zone || GyX<gy_start_x-warning_zone ||
           GyY>gy_start_y+warning_zone || GyY<gy_start_y-warning_zone ||
           GyZ>gy_start_z+warning_zone || GyZ<gy_start_z-warning_zone)
           /*if(AcX>ac_start_x+warning_zone || AcX<ac_start_x-warning_zone ||
           AcY>ac_start_y+warning_zone || AcY<ac_start_y-warning_zone ||
           AcZ>ac_start_z+warning_zone || AcZ<ac_start_z-warning_zone)*/
        {
          if(GyX>gy_start_x+warning_level_5 || GyX<gy_start_x-warning_level_5 ||
             GyY>gy_start_y+warning_level_5 || GyY<gy_start_y-warning_level_5 ||
             GyZ>gy_start_z+warning_level_5 || GyZ<gy_start_z-warning_level_5)
            playTuneBeep(NOTE_FS6);
          else
          if(GyX>gy_start_x+warning_level_4 || GyX<gy_start_x-warning_level_4 ||
             GyY>gy_start_y+warning_level_4 || GyY<gy_start_y-warning_level_4 ||
             GyZ>gy_start_z+warning_level_4 || GyZ<gy_start_z-warning_level_4)
            playTuneBeep(NOTE_DS6);
          else
          if(GyX>gy_start_x+warning_level_3 || GyX<gy_start_x-warning_level_3 ||
             GyY>gy_start_y+warning_level_3 || GyY<gy_start_y-warning_level_3 ||
             GyZ>gy_start_z+warning_level_3 || GyZ<gy_start_z-warning_level_3)
            playTuneBeep(NOTE_AS6);
          else
          if(GyX>gy_start_x+warning_level_2 || GyX<gy_start_x-warning_level_2 ||
             GyY>gy_start_y+warning_level_2 || GyY<gy_start_y-warning_level_2 ||
             GyZ>gy_start_z+warning_level_2 || GyZ<gy_start_z-warning_level_2)
            playTuneBeep(NOTE_FS5);
          else
          if(GyX>gy_start_x+warning_level_1 || GyX<gy_start_x-warning_level_1 ||
             GyY>gy_start_y+warning_level_1 || GyY<gy_start_y-warning_level_1 ||
             GyZ>gy_start_z+warning_level_1 || GyZ<gy_start_z-warning_level_1)
            playTuneBeep(NOTE_DS5);
          else 
            playTuneBeep(NOTE_CS5);
          
          
          /*//play sound pitch based
          float largest_diff=0;//3000 - 7000
          if((GyX-gy_start_x)*(GyX-gy_start_x)>largest_diff*largest_diff) 
              largest_diff=sqrt((GyX-gy_start_x)*(GyX-gy_start_x));
          if((GyY-gy_start_y)*(GyY-gy_start_y)>largest_diff*largest_diff) 
              largest_diff=sqrt((GyY-gy_start_y)*(GyY-gy_start_y));
          if((GyZ-gy_start_z)*(GyZ-gy_start_z)>largest_diff*largest_diff) 
              largest_diff=sqrt((GyZ-gy_start_z)*(GyZ-gy_start_z));
          float pitchf=(largest_diff-warning_zone)/(dead_zone-warning_zone);
          if     (pitchf>0.9) playTuneBeep(NOTE_C6);
          else if(pitchf>0.8) playTuneBeep(NOTE_B5);
          else if(pitchf>0.7) playTuneBeep(NOTE_A5);
          else if(pitchf>0.6) playTuneBeep(NOTE_G4);
          else if(pitchf>0.5) playTuneBeep(NOTE_F4);
          else if(pitchf>0.4) playTuneBeep(NOTE_E4);
          else if(pitchf>0.3) playTuneBeep(NOTE_D4);
          else if(pitchf>0.2) playTuneBeep(NOTE_C4);
          else if(pitchf>0.1) playTuneBeep(NOTE_B4);
          else                playTuneBeep(NOTE_A4);
          Serial.println(largest_diff);*/
        }
      }
    
      /*//debug code
      Serial.print("Accelerometer: ");
      Serial.print("X = "); Serial.print(AcX);
      Serial.print(" | Y = "); Serial.print(AcY);
      Serial.print(" | Z = "); Serial.println(AcZ); 
      Serial.print("Gyroscope: ");
      Serial.print("X = "); Serial.print(GyX);
      Serial.print(" | Y = "); Serial.print(GyY);
      Serial.print(" | Z = "); Serial.println(GyZ);
      Serial.println(" ");*/
      //delay(100);
    }
    delay(delay_time);
  }
}

void setColor(int red, int green, int blue)
{
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

void playTuneStart(int tunes)
{
  for (int thisNote = 0; thisNote < tunes; thisNote++) 
  {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations_start[thisNote];
    tone(buzzer_pin, melody_start[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(buzzer_pin);
  }
}

void playTuneEnd(int tunes)
{
  for (int thisNote = 0; thisNote < tunes; thisNote++) 
  {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations_end[thisNote];
    tone(buzzer_pin, melody_end[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(buzzer_pin);
  }
}

void playTuneWarning(int tunes)
{
  for (int thisNote = 0; thisNote < tunes; thisNote++) 
  {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations_warning[thisNote];
    tone(buzzer_pin, melody_warning[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(buzzer_pin);
  }
}

void playTuneBeep(int pitch)
{
  int tone_delay=100;
  tone(buzzer_pin,pitch,tone_delay);
  delay(tone_delay);
  noTone(buzzer_pin);
}

void ledFadeOut()
{
  int fade_step=10;
  while(true)
  {
    if(led_red>0) 
    {
      led_red-=fade_step;
      if(led_red<0) led_red=0;
    }
    if(led_green>0) 
    {
      led_green-=fade_step;
      if(led_green<0) led_green=0;
    }
    if(led_blue>0) 
    {
      led_blue-=fade_step;
      if(led_blue<0) led_blue=0;
    }
    setColor(led_red,led_green,led_blue);
    delay(20);
    if(led_red==0 && led_green==0 && led_blue==0) break;
  }
}

