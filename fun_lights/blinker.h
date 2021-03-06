/**
 Classi wrapper per i led 
 programma fun lights
 Code by Leonardo Berti 
*/

#ifndef _BLINKER_INC_
#define _BLINKER_INC_

//#define _DEBUG_BLINKER_

#define UNDEF_LED -1

#define ON 1
#define OFF 0

/**
 Definisce la modalità di blinking
 corrente
*/ 
struct BLINK_PATTERN
{
  int interval_on;  //tempo accesso (ms)
  int interval_off; //tempo spento(ms)
  int repeat;  //numero di ripetizioni (-1=ripete all'infinito)
  
};

/**
Stato del blinker
*/
enum BLINKER_STATE
{
  ST_IDLE,
  ST_BLINKING    
  
};

///////////////////////////// IBlinkable ///////////////////////////////////////

/**Definisce una interfaccia usata per oggetti che possono eseguire il blinking*/
class IBlinkable
{
  public:
  
  ~IBlinkable()
  {
  }
  
  /**
    Accende o spenge il led
    @param mode ON o OFF
    */
   virtual void switchLed(int mode)=0;
  
  /**
      Avvia una sequenza di blink
      @param interval intervallo in millisecondi fra lo stato acceso e spento
      @param repeat numero di ripetizioni (-1 = ripete all'infinito)
    */
  virtual bool startBlink(int interval_on,int interval_off,int repeat)=0;  
    
    /**Avvia il blinking usando un blink pattern*/
  virtual bool startBlink(BLINK_PATTERN &blink_pattern)=0;
  
  /**
    Avvia una sequenza di blinking composta da più patern
    Il blinker deve essere in stato IDLE
    @return true se la sequenza puo' essere avviata    
    */    
  virtual bool startSequence(BLINK_PATTERN* sequence,int seq_len)=0;    
    
  /**
     Interrompe il blink e lascia il led nello stato attuale
  */ 
  virtual void stopBlink()=0;
      
  /**
     Metodo da invocare nel ciclo loop del programma per aggiornare il led
  */ 
  virtual void updateState()=0;  
        
  /**
      Restituisce lo stato corrente
  */  
  virtual bool isBlinking()=0;   
    
};

/////////////////////////////// Blinker ///////////////////////////////////

/**
  Gestisce un led su un pin digitale
  senza usare delay in modo da usare più led contemporaneamente
*/  
class Blinker:public IBlinkable
{
  private:  
   
    
    unsigned long last_time; //millisec.
    
    int interval_on;//intervallo di blinking corrente in ms
    int interval_off; //intervallo spento
    int cur_interval;
    int repeat_cnt; //conteggio ripetizioni correnti
    int repeat_num; //numero di ripetizioni da fare
    int led_state; //stato del led (ON o OFF)
    BLINK_PATTERN *sequence; //sequenza di pattern di blinking corrente
    int sequence_index; //indice puntatore al pattern nella sequenza corrente
    int sequence_len; //lunghezza sequenza sequence
    
    BLINKER_STATE state;  
        
  protected:
  
    //pin a cui è collegato il blinker
    int _pin; 
      
  public:
  
    Blinker(); 
    Blinker(int pin);
    ~Blinker();   
    
    /**
      Restituisce il pin a cui è collegato
    */   
    int getPin();
    
    /**
     Collega il blinker al pin
    */ 
    bool attachTo(int pin);
    
    /**
    Accende o spenge il led
    @param mode ON o OFF
    */
    void switchLed(int mode);
    
    /**
      Avvia una sequenza di blink
      @param interval intervallo in millisecondi fra lo stato acceso e spento
      @param repeat numero di ripetizioni (-1 = ripete all'infinito)
    */
    bool startBlink(int interval_on,int interval_off,int repeat);  
    
    /**Avvia il blinking usando un blink pattern*/
    bool startBlink(BLINK_PATTERN &blink_pattern);
    
    /**
    Avvia una sequenza di blinking composta da più patern
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
    
};

#endif
