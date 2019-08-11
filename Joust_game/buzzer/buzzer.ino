// This code is for testing the passive buzzer

int out =6;                                                                                         // initializing pin 11 as buzzer pin

void setup ()                                                                                      // Code written in it will only run once.
 {
   pinMode(out, OUTPUT);                                                            // Setting pin 11 as output pin
 }

void loop ()                                                                                         // Code written in it will run repeatedly
 {
   unsigned char i, j ;                                                                          // Declaring variables
   while (1){
     for (i = 0; i <80; i++){                                                                     // 100 cycles of sound
       digitalWrite (out, HIGH);                                                         // This will turn the buzzer ON
       delay (1) ;                                                                                      // Giving a Delay of 1ms will set frequency 1
       digitalWrite (out, LOW);                                                          // This will turn the buzzer OFF
       delay (1) ;                                                                                      // Giving a delay ms
     }
     for (i = 0; i <100; i++){                                                                 // 100 cycles of sound
       digitalWrite (out, HIGH);                                                         // This will turn the buzzer ON
       delay (2) ;                                                                                      // Giving a delay of 2ms will set frequency 2
       digitalWrite (out, LOW);                                                          // This will turn the buzzer OFF
       delay (2) ;                                                                                      // Giving a delay of 2ms
     }
   }
 }
