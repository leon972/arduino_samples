/**
  Classe usata per gestire più led contemporaneamente.  
  (giochi di luce)
  
*/  

#ifndef _FUN_LIGHTS_INC_
#define _FUN_LIGHTS_INC_

//si riferisce alla classe Blinker
#include "blinker.h"

enum FUN_LIGHTS_MODE
{
  FL_NONE,   //nessun effetto speciale corrente
  FL_MOVE,   //luce che si sposta
  FL_BAR,   //barra
  FL_RANDOM //random  
  
};

struct FUN_LIGHTS_PATTERN
{
  FUN_LIGHTS_MODE mode;
  bool forward;
  bool positive;
  int interval;
  int max_interval; //usato in modalità random 
  
};

class FunLights:public IBlinkable
{
  private:
  
    //vettore dei puntatori agli oggetti di tipo Blinkable (non è detto che siano led!)
    IBlinkable** leds;
    //numero di elementi nel vettore leds
    unsigned nleds;
    
    FUN_LIGHTS_PATTERN* sequence;
    int sequence_index; //indice puntatore al pattern nella sequenza corrente
    int sequence_len; //lunghezza sequenza sequence  
   
    BLINKER_STATE state;     
    FUN_LIGHTS_PATTERN cur_pattern;
    //contatore interno per il pattern FUN_LIGHTS
    int cur_cnt;
    int max_cnt; 
    unsigned long last_time;   
           
    //passa al pattern successivo       
    bool nextPattern();  
    
    //sposta la luce da un led all'altro
    void moveLights(bool toggle_prev);
   
  
  public:
  
    FunLights();
    ~FunLights();
    
    /**
    Imposta i leds da gestire
    */
    void setLeds(IBlinkable** leds,int nleds);
    
    /**
    Accende o spenge il led
    @param mode ON o OFF
    */
    void switchLed(int mode);
    
    /**Avvia il blinking su tutti i led contemporaneamente usando un blink pattern*/
    bool startBlink(BLINK_PATTERN &blink_pattern);
    
    bool startBlink(int interval_on,int interval_off,int repeat);
   
   /**
    Avvia una sequenza di blinking composta da più pattern
    Il blinker deve essere in stato IDLE
    @return true se la sequenza puo' essere avviata    
    */    
    bool startSequence(BLINK_PATTERN* sequence,int seq_len);  
   
    /**
     Interrompe il blink e lascia il led nello stato attuale
    */ 
    void stopBlink();
      
    /**
     Metodo da invocare nel ciclo loop del programma per aggiornare il led
    */ 
    void updateState();  
        
    /**
      Restituisce lo stato corrente
    */  
    bool isBlinking();
   
   //metodi specifici di questa classe 
   /**
    Accende un led alla volta in sequenza in modo da generare un effetto movimento
    @param forward se settato, va dal led 0 all'ultimo
    altrimenti va in senso inverso
    @param interval intervallo di tempo nel passaggio da uno all'altro
    @param positive se settato il led che si sposta è accesso altrimenti è spento
    e tutti gli altri accesi
   */
   bool startMoving(bool forward,int interval,bool positive);
   
   /**
    Accensione in sequenza (effetto barra indicatrice)
   */
   bool startBar(bool forward,int interval,bool positive);
   
   /**Accensione random*/
   bool startRandom(int min_interval,int max_interval);
   
   /**
   Avvia una sequenza di effetti luce
   */
   bool startFunLightsSequence(FUN_LIGHTS_PATTERN* sequence,int seq_len);  
   
   /**
    Avvia un effetto luce
   */
   bool startFunLights(FUN_LIGHTS_PATTERN* flights);
   
     
};

#endif
