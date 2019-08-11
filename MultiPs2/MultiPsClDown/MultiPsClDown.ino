/*
PS2 Keyboard Input
Will send string of 0 and 1 representing the scan code sent by the keyboard

Make codes will start with Keyboard ID at pos[0]
Pos[10] will be 1 if keyboard is connected, otherwise fake code from unconnected pin

Break codes will be in two parts first 00000111111, which means that the next scan 
code if from that key when it was released.

The CLOCK pin will be set to LOW to stop the keyboard of sending data.
When the arduino is ready it will set the CLOCK pin to HIGH and listen for scan codes,
then go to the next keyboard.
PS2 Keyboards will not send scan codes if CLOCK is down, but will store the code untill
the CLOCK is set to HIGH. 16 bytes can be stored.
*/

#define MAX_KEYBOARDS 6

int g_data_pins[MAX_KEYBOARDS]={2,4,6,8,10,12};
int g_clock_pins[MAX_KEYBOARDS]={0,3,5,7,9,11};
char g_num_to_char_id[MAX_KEYBOARDS]={'a','b','c','d','e','f'};

int read_scan_code(int index_keyboard);

void setup()
{
  //init data com
  Serial.begin(9600);
  while(!Serial) ;//wait for computer
  Serial.println("Startup done!");
  
  //init pins
  for(int i=0;i<MAX_KEYBOARDS;i++)
  {
    pinMode(g_data_pins[i],INPUT);
    pinMode(g_clock_pins[i],OUTPUT);//will be set to low to stop transmission
    digitalWrite(g_clock_pins[i],LOW);
  }
}

void loop()
{
  //Listen for scan codes from keyboards, one at a time
  for(int index_keyboard=0;index_keyboard<MAX_KEYBOARDS;index_keyboard++)
  {
    //set clock pin to HIGH to allow transmission
    digitalWrite(g_clock_pins[index_keyboard],HIGH);
    
    //change pin state to INPUT
    pinMode(g_clock_pins[index_keyboard],INPUT);
    
    //listen for scan code (if CLOCK goes LOW)
    //PROBLEM, OM KEYBOARD INTE ÄR INKOPPLADE SÅ FLIMMERPROBLEM...
    int time_out=100; //µs
    int time_step=5;
    for(int time=0;time<time_out;time+=time_step)
    {
      delayMicroseconds(time_step);//here of after test?
      if( digitalRead(g_clock_pins[index_keyboard])==LOW &&
          digitalRead(g_clock_pins[index_keyboard])==LOW )
      {//incomming scan code
        read_scan_code(index_keyboard);
        break;//done with this keyboard, ELLER INTE OM DET KOMMER FLER...
      }
      //delayMicroseconds(time_step);//here of before test?
    }
    //Men borde vänta på om det kommer en kod till
    
    //change pin state to OUTPUT
    pinMode(g_clock_pins[index_keyboard],OUTPUT);
    
    //set clock pin to LOW to stop transmission
    digitalWrite(g_clock_pins[index_keyboard],LOW);
  }
  //cycle done
}

int read_scan_code(int index_keyboard)
{//Read scan code and send data to Serial
  bool lost_code=true;
  //Both CLOCK and DATA is now LOW
  //wait for scancode
  int time_step=5;// 5 µs
  int time_out=1000;// 1 ms
  int numof_chars=11;
  char scan_code[]="00000000000";
  for(int i=0;i<numof_chars;i++)
  {
    //wait for clock to go up (time to read)
    for(int time=0;time<time_out;time+=time_step)
    {
      if( digitalRead(g_clock_pins[index_keyboard])==1 )
      {
        lost_code=false;
        break;
      }
      delayMicroseconds(time_step);
    }
    if(lost_code) return 1;
    
    //read DATA pin
    if( digitalRead(g_data_pins[index_keyboard]==1))
    {
      scan_code[i]='1';
    }
    else
    {
      scan_code[i]='0';
    }
  
    //wait for clock to go down
    lost_code=true;
    for(int time=0;time<time_out;time+=time_step)
    {
      if( digitalRead(g_clock_pins[index_keyboard])==0 )
      {
        lost_code=false;
        break;
      }
      delayMicroseconds(time_step);
    }
    if(lost_code) return 1;
    //next state
  }
  
  //tag string with keyboard index
  scan_code[0]=g_num_to_char_id[index_keyboard];
  //send string to Serial
  Serial.println(scan_code);
  
  return 0;//done
}
