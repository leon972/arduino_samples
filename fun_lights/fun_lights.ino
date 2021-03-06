
/**
 Questo programma mostra l'utilizzo delle classi e di altri costrutti C++ in arduino.
 Nella directory del progetto vengono creati i file blinker.h e blinker.cpp
 (nota:i file collegati non devono mai avere lo stesso nome del file principale .ino altrimenti
 da undefined reference in main,per aggiungere i file .h e .cpp allo sketch, adare su sketch->aggiungi file,selezionare i due file da aggiungere
 e cliccare "Annulla" quando chiede di sostituire i file.Poi chiudere lo sketch e riaprilo per vedere i file aggiunti)
 La classe blinker è un wrapper per un led attacato ad un pin digitale e consente la gestione del led e della
 sequenza di blinking senza usare la funzione delay che blocca l'esecuzione del programma.
 Usando una istanza di Blinker per ciascun led si possono pilotare i led in modo indipendente e non bloccante.

 Hardware:
 4 led collegati in serie a resistenze da 330 Ohm ai pin 10,11,12,13 della
 scheda Arduino UNO
 
 Code by Leonardo Berti (c) 2012
 
 This software is FREEWARE

*/ 

#include "blinker.h"
#include "flights.h"

//led collegati ai pin digitali di Arduino
Blinker led1;
Blinker led2;
Blinker led3;
Blinker led4;

FunLights funLights; //gestore giochi luci

#define NLEDS 4

IBlinkable* leds[NLEDS];


//tipo di programma da eseguire acquisito tramite richiesta 
//via porta seriale

/**testa la classe Blinker*/
static const int PRG_TEST_BLINKER=1;
/**Testa i giochi di luce*/
static const int PRG_FUN_LIGHTS=2;
/**Default*/
static const int PRG_NONE=0;

int cur_program=PRG_NONE;

//sequenza di blinking
static BLINK_PATTERN bpattern[]={
    500,100,10,
    100,2000,10,
    70,70,40,
    2000,200,5, 
    20,300,10,
    1000,1000,5,
    50,1000,5  
  
  };
  
  /**
  FUN_LIGHTS_MODE mode;
  bool forward;
  bool positive;
  int interval;
  int max_interval; //usato in modalità random 
  */
  
static FUN_LIGHTS_PATTERN fpattern[]=
{
  FL_MOVE,false,true,1300,0,
  FL_MOVE,false,true,1300,0,
  FL_BAR,true,true,1500,0,
  FL_BAR,false,true,1300,0,
  FL_MOVE,true,false,1400,0,
  FL_BAR,false,false,1000,0    
  
};

void stopAll()
{
  
  for (int i=0;i<NLEDS;i++)
  {
    leds[i]->stopBlink();    
  }
  
}

/**
  Visualizza il main menu e chiede via porta seriale il tipo di programma
  @return il programma
*/  
int mainMenu()
{
  
  int mode=0; 
  
  stopAll();
  
  //avvia il lampeggio durante l'attesa dell'input
  led1.startBlink(1000,1000,-1);
  
  //fa in modo che se non riceve l'input dopo un po' usa il programma di default
  unsigned long tm=millis();
  
  while(!(mode>0 & mode<=2)) {
     
    Serial.println("------- FUN LIGHTS -------");
    Serial.println("Seleziona il programma:");
    Serial.println("1] test blinker : test classe blinker, 4 led indipendenti");
    Serial.println("2] test fun lights: test giochi di luce");    
    Serial.println("");
    Serial.println("Scegli:");        
    
    //attende l'input (dopo un certo tempo procede con il default)
    while (!Serial.available()) 
    {
      
      delay(50); //non sovraccarica
      
      led1.updateState();
      
      //programma di default
      if ((millis()-tm)>7000) return PRG_NONE; 
            
      //attende l'input di un numero da parte dell'utente
      
    }
      
    mode=Serial.parseInt(); 
    
  } 
  
  return mode;     
}

/**
  Hardware setup
*/
void hwSetup()
{
  //avvia la comunicazione seriale
  Serial.begin(9600);
  
  //inizializza la sequenza di numeri casuali
  randomSeed(analogRead(0));
  
  //collega i led ai pin digitali
  led1.attachTo(13);   
  led2.attachTo(12);
  led3.attachTo(11);
  led4.attachTo(10);
  
  //puntatori ai led
  leds[0]=&led1;
  leds[1]=&led2;
  leds[2]=&led3;
  leds[3]=&led4;
      
}

/**
  Setup modalità blink test
*/  
void startBlinkTest()
{
  
  cur_program=PRG_TEST_BLINKER;
  
  Serial.println("BLINKER TEST!");
  
  //il led 1 esegue una sequenza di blinking definita all'inizio
  led1.startSequence(bpattern,sizeof(bpattern)/sizeof(BLINK_PATTERN));
  
  //il led 2 eseguen un blink infinito
  led2.startBlink(200,200,-1);
  
  //il led 3 esegue un certo numero di blink a intervalli di 1 secondo
  led3.startBlink(1000,1000,20);
    
  //led4 pulsa a intervalli di un secondo con 0,2 secondi acceso  
  led4.startBlink(200,1000,40);
  
}

/**
 Modalità fun lights
*/ 
void startFunLights()
{
  
  cur_program=PRG_FUN_LIGHTS;
  
  Serial.println("FUN LIGHTS!");
  
  funLights.setLeds(leds,NLEDS);
  
  funLights.startFunLightsSequence(fpattern,sizeof(fpattern)/sizeof(FUN_LIGHTS_PATTERN));  
  
}

/**
 Inizializza
*/
void setup()
{
  
  hwSetup();   
 
}

/**
  Aggiorna lo stato dei led
*/
void updateLeds()
{
  if (cur_program==PRG_TEST_BLINKER) 
  {  
    for (int i=0;i<NLEDS;i++)
    {
      leds[i]->updateState();    
    }  
  }
  else if (cur_program==PRG_FUN_LIGHTS)
  {
    funLights.updateState();  
  }
}

/**
 Determina se il programma di blinking è ancora in esecuzione
*/ 
bool isRunning()
{
  
  if (cur_program==PRG_TEST_BLINKER) {
  
    for (int i=0;i<NLEDS;i++)
    {
      if (leds[i]->isBlinking()) return true;     //almeno uno  ancora in blinking
    }  
  
  }
  else if (cur_program==PRG_FUN_LIGHTS)
  {
    return funLights.isBlinking();    
  }
  
  return false;
}


/**
 loop esecuzione programma
*/ 
void loop()
{  
  int mode=mainMenu();
  
  switch (mode)
  {
    case PRG_TEST_BLINKER:
      startBlinkTest();
    break;
    
    case PRG_FUN_LIGHTS:
      startFunLights();
    break;
    
    default:
      Serial.println("DEFAULT:"); 
      startBlinkTest();
    break;    
  }      
  
  boolean running=true;
  //timer corrente
  unsigned long tm=millis();
  unsigned long tm1=millis();
  
  while (running)
  {
    
    unsigned long ctm=millis();
    
    if ((ctm-tm)>50)
    {
        //esegue il controllo
        tm=millis();
        //controlla se deve ritornare di nuovo al menu
        running=isRunning();       
    }    
    else if ((ctm-tm1)>120000) //2 minuti max
    {
        //timeout:ritorna al menu
        Serial.println("TIMEOUT!");
        running=false;
    }
    
    updateLeds();    
  }  
  
}

