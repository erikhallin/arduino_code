/*
PS2 Keyboard Input
Will send string of 0 and 1 representing the scan code sent by the keyboard

Make codes will start with Keyboard ID at pos[0]
Pos[10] will be 1 if keyboard is connected, otherwise fake code from unconnected pin

Break codes will be in two parts first 00000111111, which means that the next scan 
code if from that key when it was released.

*/

#define DATA_PIN 2
#define CLOCK_PIN 0

int g_scan_prog=0;//position in scancode, 0 = OFF
int g_last_clock_state=0;//0 if LOW, 1 if HIGH
char g_scan_code[]="00000000000";

void setup()
{
  //init data com
  Serial.begin(9600);
  while(!Serial) ;//wait for computer
  Serial.println("Startup done!");
   
  //init pins
  pinMode(DATA_PIN,INPUT);
  pinMode(CLOCK_PIN,INPUT);
}

void loop()
{ 
  //update clock
  bool check_time=false;
  int clock_state=digitalRead(CLOCK_PIN);
  if( clock_state==1 && g_last_clock_state==0 ) check_time=true;
  //update last clock
  if( clock_state==1 ) g_last_clock_state=1;
  else g_last_clock_state=0;
  
  //update scan code in progress
  if( check_time && g_scan_prog!=0 )
  {
    int data_state=digitalRead(DATA_PIN);
    if( data_state==0 )
    {//LOW
      g_scan_code[g_scan_prog]='0';
    }
    else
    {//HIGH
      g_scan_code[g_scan_prog]='1';
    }
    g_scan_prog++;//next pos in scan code
    
    //if scan code done, send to com
    if( g_scan_prog>10 )
    {//done, send to com
      //test if scan code is not fake (unconnected keyboard)
      if(g_scan_code[10]=='1')
      {//code OK, send
        Serial.println(g_scan_code);
      }
      //reset values
      g_scan_prog=0;
      check_time=false;
    }
  }
  
  //test if new scancode on the way
  if( check_time && g_scan_prog==0 )
  {
    if( digitalRead(DATA_PIN)==0 ) 
    {//incomming scancode
      g_scan_prog=1;
      //g_scan_code[0]=0;//Keyboard ID
    }
  }
}
