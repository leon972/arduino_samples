/**

  Questo programma esegue il fading (variazione di luminosità)
  su un led collegato al pin 9 usando la funzione analogWrite
  Il fading avviene in modalità non bloccante. (non usa delay) 
  Code by Leonardo Berti (c) 2012

*/  

#include "fader.h"

#define LED_PIN 9

//led collegato al pin con modulazione PWM 9
Fader fader(LED_PIN);

int cnt=0;

void setup()
{

  Serial.begin(9600);

  Serial.println("FADER!");  
  
  fader.setIntensity(12);
  
  fader.startFading(10,1,true,10);
  
}


void loop()
{
  if (!fader.updateState())
  {
    switch (cnt++) {
    
      case 0:  
        //altra sequenza
        fader.startFading(100,10,true,10);    
      break;
      case 1:
        fader.startFading(4,1,true,5);
        break;
      default:
        fader.startFading(300,16,true,4);
        cnt=0;
        break;         
    
    }
  }
  
}
