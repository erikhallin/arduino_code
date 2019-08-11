/*
The keyboards clocks will be connected to a pin 
that will report when will call a function when
the signal goes up.

This function will read the data state of that keyboard
and if signal is complete sent it to the computer.

BUGS:
-Vid inkoppling av PS2 så connectas, lostas, och connectas igen
-tar lång tid för att tangentbord ska disconnectas
-första koden vid tangenttryckning är fel
-Keyboard 3 hittas inte, kontakt 4 (data) är av i socket

*/

#include <PinChangeInt.h>

#define MAX_KEYBOARDS 6//change requires more changes

int  g_scan_prog[MAX_KEYBOARDS]={-1};
char g_scan_code[MAX_KEYBOARDS][12]={'\0'};
char g_num_to_char_id[MAX_KEYBOARDS]={'a','b','c','d','e','f'};

int g_clk_pin_to_keyboard_id[12]={4,0,0,2,0,0,0,3,0,0,5,1};
int g_data_pins[MAX_KEYBOARDS]={6,12,4,8,2,9};
int g_clock_pins[MAX_KEYBOARDS]={5,11,3,7,0,10};
/*
int g_clk_pin_to_keyboard_id[12]={0,0,1,1,1,2,2,3,3,4,4,5};
int g_data_pins[MAX_KEYBOARDS]={2,4,6,8,10,12};
int g_clock_pins[MAX_KEYBOARDS]={0,3,5,7,9,11};
*/
int g_connected_keyboard[MAX_KEYBOARDS]={0};
int g_low_data_counter[MAX_KEYBOARDS]={0};
int g_low_data_tol=100;//cycles of low before kick

void clock_rise();

void setup()
{
  Serial.begin(9600);
  while(!Serial) ;
  Serial.println("STARTUP DONE");
  
  //init pins
  for(int i=0;i<MAX_KEYBOARDS;i++)
  {
    pinMode(g_data_pins[i],INPUT);
    pinMode(g_clock_pins[i],INPUT); digitalWrite(g_clock_pins[i], HIGH);
    PCintPort::attachInterrupt(g_clock_pins[i], &clock_rise, RISING);
  }
  
  //check for connected keyboards
  for(int i=0;i<MAX_KEYBOARDS;i++)
  {
    if( digitalRead(g_data_pins[i])==HIGH )//is connected
    {
      g_connected_keyboard[i]=1;
      Serial.print("Found keyboard ");
      Serial.println(i+1);
    }
  }
}

void loop()
{
  //check for disconnected keyboards, if LOW state for too long
  for(int i=0;i<MAX_KEYBOARDS;i++)
  {
    if( g_connected_keyboard[i]==0 ) continue;
    if( digitalRead( g_data_pins[i] )==LOW )
    {
      //Serial.print("low count: ");Serial.println(g_low_data_counter[i]);
      g_low_data_counter[i]++;
      if( g_low_data_counter[i]>g_low_data_tol )//kick
      {
        g_low_data_counter[i]=0;
        g_connected_keyboard[i]=0;
        g_scan_prog[i]=-1;
        Serial.print("Lost connection to keyboard ");
        Serial.println(i+1);
      }
    }
    else g_low_data_counter[i]=0;//reset counter
  }
  
}

void clock_rise()
{
  int latest_interrupted_pin=PCintPort::arduinoPin;//to let function know which pin that called
  int keyboard_id=g_clk_pin_to_keyboard_id[latest_interrupted_pin];
  //connectetion test, first rising signal will be from connecting the keyboard
  if( g_connected_keyboard[keyboard_id]==0 ) 
  {
    Serial.print("Connected keyboard ");
    Serial.println(keyboard_id+1);
    g_connected_keyboard[keyboard_id]=1;//now connected
    g_scan_prog[keyboard_id]=-1;
    return;
  }
  g_scan_prog[keyboard_id]++;
  if( digitalRead( g_data_pins[keyboard_id] )==HIGH ) g_scan_code[ keyboard_id ][ g_scan_prog[keyboard_id] ]='1';
  else g_scan_code[ keyboard_id ][ g_scan_prog[keyboard_id] ]='0';
  //test if ready to send
  if( g_scan_prog[keyboard_id]>=10 )
  {
    g_scan_code[ keyboard_id ][ 0 ]=g_num_to_char_id[ keyboard_id ];//set keyboard id
    Serial.println( g_scan_code[keyboard_id] );//send
    g_scan_prog[keyboard_id]=-1;//clear
  }
  
}
