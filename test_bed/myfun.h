
#ifndef _MYFUN_
#define _MYFUN_

#include "Arduino.h"

#define DEFAULT_PIN 13

void my_display();

class MyBase
{
  private:   
      
  protected: 
  
  int _pin;
      
  public:
  
  MyBase(int pin);
  MyBase();
  ~MyBase()
  {
  }
  
  void init();
  void switchOn();
  void switchOff();  
  virtual void test()=0; 
    
};

class MyExt:public MyBase
{
  
  public:
  
  MyExt();
  MyExt(int pin);
  
  virtual void test();
  
  void blink(int delay_millis,int repeat); 
  
};

#endif
