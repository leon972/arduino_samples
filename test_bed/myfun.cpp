
#include "myfun.h"
#include "Arduino.h"

///////////////// MyBase ///////////////////////

MyBase::MyBase()
{
  _pin=DEFAULT_PIN;  
    
};

MyBase::MyBase(int pin)
{
  _pin=pin;     
 
};

void MyBase::init()
{
  pinMode(_pin,OUTPUT); 
}

void MyBase::switchOn()
{
  digitalWrite(_pin,HIGH);  
}

void MyBase::switchOff()
{
  digitalWrite(_pin,LOW); 
}

/////////////////// MyExt ////////////////////////////

MyExt::MyExt()
{
  _pin=DEFAULT_PIN;   
}

MyExt::MyExt(int pin)
{
  _pin=pin;  
}
  
void MyExt::test()
{
  Serial.println("called virtual function!");  
}
  
void MyExt::blink(int delay_millis,int repeat)
{
  for(int i=0;i<repeat;i++)
  {
    switchOn();
    delay(delay_millis);
    switchOff();   
    delay(delay_millis);
  }     
} 

void my_display()
{

  int a=10;
  a++; 
  Serial.println("Hello from myfun!");
  
}
