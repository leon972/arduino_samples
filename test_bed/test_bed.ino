/**

 Test bed
 
*/
#include "myfun.h"
#include <stdio.h>
#include <string.h>

#define BUFF_SZ 20

static char buff[BUFF_SZ];

void setup()
{  

  Serial.begin(9600);
  
  Serial.println("STARTED");
  
  memset(buff,0,BUFF_SZ);  
    
  
  sprintf(buff,"v=%d",123);
  
  Serial.println(buff);  
  
  int a=0;
  
  sscanf("123","%d",&a);
  
  Serial.println(a);
  
  double f=0.0f;
  
 
  
  f=77.88f;
  
  Serial.print(f);
  
  
}

void loop()
{
  
  
}

