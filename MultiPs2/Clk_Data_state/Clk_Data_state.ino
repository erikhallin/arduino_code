int CLOCK=0;
int DATA=2;

bool state_clk=true;
bool state_data=true;

#define _MAX 100
double times[_MAX]={0};
int counter=0;;

void setup()
{
  pinMode(CLOCK, INPUT);
  pinMode(DATA, INPUT);
  
  Serial.begin(9600);
  while(!Serial) ;//wait for computer
  Serial.println("Startup done!");
  
}

void loop()
{  
  if(counter>=_MAX) 
  {
    counter=0;
    Serial.println("VALUES");
    for(int i=0;i<_MAX;i++)
    {
      Serial.println(times[i]);
    }
    Serial.println("END");
  }
  
  if( digitalRead(CLOCK)!=state_clk )
  {
    if(state_clk) 
    {
      times[counter]=micros();
      counter++;
    }
    else 
    {
      times[counter]=micros();
      counter++;
    }
    state_clk=!state_clk;
  }

    
  /*if( digitalRead(DATA)!=state_data )
  {
    if(state_data) 
    {
      Serial.print(micros() );
      Serial.println(" DATA OFF");
    }
    else 
    {
      Serial.print(micros() );
      Serial.println(" DATA ON");
    }
    state_data=!state_data;
  }*/
}
