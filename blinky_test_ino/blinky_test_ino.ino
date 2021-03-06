/*
  Blink - versione modificata di prova
 */
 
//pin a cui è collegato il led sulla scheda
const int LED_PIN = 13;

const int ON=1;

const int OFF=0;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(LED_PIN, OUTPUT);     
}

/**
  Accende o spenge il led
  @param mode ON o OFF
*/  
void switchLed(int mode)
{
  digitalWrite(LED_PIN,mode); 
  
}

/**
 Ciclo di blinking
 @param seconds numero di secondi durata ciclo
 @param millis_on numero millisecondi durata led acceso
 @param millis_off numero millisecondi durata led spento
*/
void blinkCycle(int seconds,int millis_on,int millis_off)
{
  //Acquisisce il timer (numero millisecondi dall'inizio del programma)
  ////nb: overflow dopo 50 giorni circa 
  unsigned long curTime=millis();  
  
  unsigned long duration=curTime+seconds*1000;
  
  unsigned long ct;
  
  while (millis()<duration)
  {
    delay(10); //deleay per evitare il sovraccarico
    
    switchLed(ON);
    
    delay(millis_on);
    
    switchLed(OFF);
    
    delay(millis_off);
    
  }
  
}

//ciclo infinito di esecuzione programma
void loop() { 
  
  blinkCycle(5,500,1000);
  
  blinkCycle(5,100,100);
  
  blinkCycle(10,100,2000);
  
}
