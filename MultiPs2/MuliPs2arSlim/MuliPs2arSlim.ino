/*
PS2 Keyboard Input
Will send string of 0 and 1 representing the scan code sent by the keyboard

Make codes will start with Keyboard ID at pos[0]
Pos[10] will be 1 if keyboard is connected, otherwise fake code from unconnected pin

Break codes will be in two parts first 00000111111, which means that the next scan 
code if from that key when it was released.

FÖR LÅNGSAM FÖR FLER ÄN 2 KEYBOARDS...
*/

#define MAX_KEYBOARDS 6

int g_scan_prog[MAX_KEYBOARDS]={0};//position in scancode, 0 = OFF
int g_last_clock_state[MAX_KEYBOARDS]={0};//0 if LOW, 1 if HIGH
char g_scan_code[MAX_KEYBOARDS][11]={'0'};
char g_num_to_char_id[MAX_KEYBOARDS]={'a','b','c','d','e','f'};

int g_data_pins[MAX_KEYBOARDS]={2,4,6,8,10,12};
int g_clock_pins[MAX_KEYBOARDS]={0,3,5,7,9,11};

void setup()
{
  //init pins
  for(int i=0;i<MAX_KEYBOARDS;i++)
  {
    pinMode(g_data_pins[i],INPUT);
    pinMode(g_clock_pins[i],INPUT);
    g_scan_code[i][0]=g_num_to_char_id[i];//Keyboard ID
  }
  
  //init data com
  Serial.begin(9600);
  while(!Serial) ;//wait for computer
  Serial.println("Startup done!");
}

void loop()
{ 
  for(int i=0;i<MAX_KEYBOARDS;i++)//go through all keyboards
  {
    //update clock
    int clock_state=digitalRead( g_clock_pins[i] );
    if(i!=0) continue;
    if( clock_state==1 && g_last_clock_state[i]==0 ) 
    {
      if( g_scan_prog[i]!=0 )
      {
        int data_state=digitalRead( g_data_pins[i] );
        if( data_state==0 )
        {//LOW
          g_scan_code[i][ g_scan_prog[i] ]='0';
        }
        else
        {//HIGH
          g_scan_code[i][ g_scan_prog[i] ]='1';
        }
        g_scan_prog[i]++;//next pos in scan code
      }
      else
      {
        if( digitalRead(g_data_pins[i])==LOW ) 
        {//incomming scancode
          g_scan_prog[i]=1;
        }
      }
      
      //if scan code done, send to com
      if( g_scan_prog[i]>10 )
      {//done, send to com
        //test if scan code is not fake (unconnected keyboard)
        if(g_scan_code[i][10]=='1')
        {//code OK, send
          Serial.println(g_scan_code[i]);
        }
        //reset values
        g_scan_prog[i]=0;
      }
    }

    //update last clock
    if( clock_state==1 ) g_last_clock_state[i]=1;
    else g_last_clock_state[i]=0;
       
    //go to next keyboard
  }
  //all keyboards checked this cycle
}
