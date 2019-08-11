
int CLOCK=0;
int DATA=2;

int read_scan_code(void);

void setup()
{
  //init data com
  Serial.begin(9600);
  while(!Serial) ;//wait for computer
  Serial.println("Startup done!");
  
  pinMode(CLOCK,INPUT);
  pinMode(DATA,INPUT);
}

void loop()
{
  if( digitalRead(CLOCK)==LOW && digitalRead(DATA)==LOW )
  {
    int result=read_scan_code();
    if(result)
    {
      Serial.println(result);
    }
  }
}

int read_scan_code(void)
{//Read scan code and send data to Serial
  bool lost_code=true;
  //Both CLOCK and DATA is now LOW
  //wait for scancode
  int time_step=1;// 5 µs
  int time_out=1000;// 1 ms
  int numof_chars=11;
  char scan_code[]="00000000000";
  int times[11]={0};
  for(int i=0;i<numof_chars;i++)
  {
    //wait for clock to go up (time to read)
    for(int time=0;time<time_out;time+=time_step)
    {
      if( digitalRead(CLOCK)==HIGH )
      {
        lost_code=false;
        times[i]=time;
        break;
      }
      //delayMicroseconds(time_step);
    }
    if(lost_code) break;
    
    //read DATA pin
    if( digitalRead(DATA)==HIGH )
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
      if( digitalRead(CLOCK)==LOW )
      {
        lost_code=false;
        //Serial.println(time);
        break;
      }
      //delayMicroseconds(time_step);
    }
    if(lost_code) break;
    //next state
  }
  
  /*if(lost_code)
  {
    Serial.println("ERROR val:");
    for(int i=0;i<numof_chars;i++)
    {
      Serial.println(times[i]);
    }
  }*/
  
  //tag string with keyboard index
  scan_code[0]='a';
  //send string to Serial
  Serial.println(scan_code);
  
  return 0;//done
}
