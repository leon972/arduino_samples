/**
 Sequenze di di accensione di 4 led collegati ai pin digitali
 13,12,11,10 della scheda ARDUINO UNO
 
 Descrizione circuito:
 I led sono collegati ai pin e con in serire una resistenza da 330 ohm (?)
 ciascuno.
 
 Code by Leonardo Berti (c) 2012
 
*/
 
/**Numero di led utilizzati*/ 
const int NLEDS=4; 

const int DEBUG_ON=1;

const int ON=1;

const int OFF=0;
 
/**mappatura pin a cui sono collegati i led*/
const int led_pins[4]={10,11,13,12};

void initHardware()
{
  
  for (int i=0;i<NLEDS;i++)
  {
    //inizializza ogni pin collegato ai led come pin di output
    //digitale
    pinMode(led_pins[i],OUTPUT);  
  } 
  
  //avvia la comunicazione seriale
  Serial.begin(9600);  
  
  Serial.println("HW init ok");
  
}


//routine di setup invocata all'inizio 
void setup() {                
    initHardware();
}

/**
  Accende o spenge il led
  @param led indice led da 0 a NLEDS
  @param mode ON o OFF
*/  
void switchLed(int led,int mode)
{
  digitalWrite(led_pins[led],mode);   
}

//ciclo infinito di esecuzione programma
void loop() { 
  
  switchLed(0,ON);
  
  delay(500);
  
  switchLed(1,ON);
  
  delay(500);
  
  switchLed(2,ON);
  
  delay(500);
  
  switchLed(3,ON);
  
  delay(500);
  
  switchLed(0,OFF);
  switchLed(1,OFF);
  switchLed(2,OFF);
  switchLed(3,OFF);
  
  delay(500);
  
  
}
