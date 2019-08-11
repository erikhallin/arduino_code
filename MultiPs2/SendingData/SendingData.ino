int CLOCK=0;
int DATA=2;

#define numofstates 10
int states[numofstates]={0};

bool wait_for_low(int pin);
bool wait_for_high(int pin);

void setup()
{
  //init data com
  Serial.begin(9600);
  while(!Serial) ;//wait for computer
  Serial.println("Startup done!");
  
  pinMode(CLOCK,INPUT);
  pinMode(DATA,INPUT);
  
  delay(100);
  
  //1. CLOCK down
  pinMode(CLOCK,OUTPUT);
  digitalWrite(CLOCK,LOW);
  //wait
  delayMicroseconds(100);
  //2. DATA down
  pinMode(DATA,OUTPUT);
  digitalWrite(DATA,LOW);
  //3. release clock
  pinMode(CLOCK,INPUT);
  //4. wait for clock to go down
  int time_out=1000;
  bool LOST=true;
  for(int time=0;time<time_out;time++)
  {
    if( digitalRead(CLOCK)==LOW )
    {
      LOST=false;
    }
  }
  if(LOST) Serial.println("ERROR 1");
  
  //5-8. start to send data 11101101 <- this side first
  
  //Set start bit (always 0)
  digitalWrite(DATA,LOW);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set data 0
  digitalWrite(DATA,HIGH);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set data 1
  digitalWrite(DATA,LOW);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set data 2
  digitalWrite(DATA,HIGH);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set data 3
  digitalWrite(DATA,HIGH);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set data 4
  digitalWrite(DATA,LOW);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set data 5
  digitalWrite(DATA,HIGH);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set data 6
  digitalWrite(DATA,HIGH);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set data 7
  digitalWrite(DATA,HIGH);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set pararity bit (1 if even numbof 1)
  digitalWrite(DATA,HIGH);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set stop bit (always 1)
  digitalWrite(DATA,HIGH);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //release
  //9. release DATA
  pinMode(DATA,INPUT);
  //10. wait for data to be low
  LOST=true;
  for(int time=0;time<time_out;time++)
  {
    if( digitalRead(DATA)==LOW )
    {
      LOST=false;
    }
  }
  if(LOST) Serial.println("ERROR 20");
  
  //11. wait for clock to go low
  LOST=true;
  for(int time=0;time<time_out;time++)
  {
    if( digitalRead(CLOCK)==LOW )
    {
      LOST=false;
    }
  }
  if(LOST) Serial.println("ERROR 30");
  
  //capture again
  //1. CLOCK down
  pinMode(CLOCK,OUTPUT);
  digitalWrite(CLOCK,LOW);
  //wait
  delayMicroseconds(100);
  //2. DATA down
  pinMode(DATA,OUTPUT);
  digitalWrite(DATA,LOW);
  //3. release clock
  pinMode(CLOCK,INPUT);
  //4. wait for clock to go down
  LOST=true;
  for(int time=0;time<time_out;time++)
  {
    if( digitalRead(CLOCK)==LOW )
    {
      LOST=false;
    }
  }
  if(LOST) Serial.println("ERROR 1");
  
  //send command 00000111 //light all LED
  //small pause
  delayMicroseconds(100);
  
  //Set start bit (always 0)
  digitalWrite(DATA,LOW);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set data 0
  digitalWrite(DATA,HIGH);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set data 1
  digitalWrite(DATA,HIGH);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set data 2
  digitalWrite(DATA,HIGH);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set data 3
  digitalWrite(DATA,LOW);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set data 4
  digitalWrite(DATA,LOW);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set data 5
  digitalWrite(DATA,LOW);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set data 6
  digitalWrite(DATA,LOW);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set data 7
  digitalWrite(DATA,LOW);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set pararity bit (1 if even numbof 1)
  digitalWrite(DATA,LOW);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  //Set stop bit (always 1)
  digitalWrite(DATA,HIGH);
  //wait for CLOCK to go up (data is read)
  if(!wait_for_high(CLOCK)) Serial.println("ERROR");
  
  //wait for clock to go down
  if(!wait_for_low(CLOCK)) Serial.println("ERROR");
  
  
  
  //9. release DATA
  pinMode(DATA,INPUT);
  //10. wait for data to be low
  LOST=true;
  for(int time=0;time<time_out;time++)
  {
    if( digitalRead(DATA)==LOW )
    {
      LOST=false;
    }
  }
  if(LOST) Serial.println("ERROR 2");
  
  //11. wait for clock to go low
  LOST=true;
  for(int time=0;time<time_out;time++)
  {
    if( digitalRead(CLOCK)==LOW )
    {
      LOST=false;
    }
  }
  if(LOST) Serial.println("ERROR 3");
  
  //sen ska båda upp igen
  
}

void loop()
{
  ;
}

bool wait_for_low(int pin)
{
  int time_out=1000;
  for(int time=0;time<time_out;time++)
  {
    if( digitalRead(pin)==LOW )
    {
      return true;
    }
  }
  return false;
}

bool wait_for_high(int pin)
{
  int time_out=1000;
  for(int time=0;time<time_out;time++)
  {
    if( digitalRead(pin)==HIGH )
    {
      return true;
    }
  }
  return false;
}

