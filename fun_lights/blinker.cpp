/**
 Classi wrapper per i led 
 programma fun lights
 Code by Leonardo Berti 
*/

#include "blinker.h"
//libreria base Arduino
#include "Arduino.h"

//////////////////////// Blinker /////////////////////////////////////////

Blinker::Blinker()
{
  _pin=UNDEF_LED; //inattivo  
  state=ST_IDLE;
  
}

Blinker::Blinker(int  pin)
{
  state=ST_IDLE;
  attachTo(pin);    
}

Blinker::~Blinker()
{
  state=ST_IDLE;
  _pin=UNDEF_LED;  
}

/**
     Collega il blinker al pin
*/ 
bool Blinker::attachTo(int pin)
{
  if (pin>1 && pin<=13)
  {    
    _pin=pin;
    //mette il pin digitale in modalità output
    pinMode(pin,OUTPUT);   
    //spenge il led
    this->switchLed(OFF);
    return true;
  }
  else return false;
  
}
   
/**
  Restituisce il pin a cui è collegato
*/   
int Blinker::getPin()
{
  return _pin;  
}
            
/**
Accende o spenge il led
@param mode ON o OFF
*/
void Blinker::switchLed(int mode)
{
  if ((mode==ON || mode==OFF) && _pin!=UNDEF_LED)
  {
    digitalWrite(_pin,mode);  
    led_state=mode;  
  }  
}
    
/**
  Avvia una sequenza di blink
  per aggiornare lo stato invocare updateState nel loop del programma
  @param interval intervallo in millisecondi fra lo stato acceso e spento
  @param repeat numero di ripetizioni 
*/
bool Blinker::startBlink(int interval_on,int interval_off,int repeat)
{
  if (state==ST_IDLE)
  {
    
    this->interval_on=interval_on; //intervallo in millisec.
    this->interval_off=interval_off;
    
    cur_interval=led_state ? interval_on:interval_off;
    
    repeat_num=repeat; //ripetizioni ciclo
    repeat_cnt=0;
    last_time=millis(); //memorizza il tempo corrente
    
    #ifdef _DEBUG_BLINKER_
    
    Serial.println("START BLINK:");
    Serial.print("on=");
    Serial.println(interval_on);
    Serial.print("off=");
    Serial.println(interval_off);
    Serial.print("repeat=");
    Serial.println(repeat);
    
    #endif
    
    state=ST_BLINKING;    
        
    return true;
  }
  else return false; 
  
}

/**
 Avvia il blinking
*/ 
bool Blinker::startBlink(BLINK_PATTERN &blink_pattern)
{
  return startBlink(blink_pattern.interval_on,blink_pattern.interval_off,blink_pattern.repeat);  
}

/**
 Avvia una seuqneza di blinking
*/ 
bool Blinker::startSequence(BLINK_PATTERN* sequence,int seq_len)
{
    if (state!=ST_IDLE || seq_len<=0 || !sequence) return false;  
    
    #ifdef _DEBUG_BLINKER_
    
      Serial.println("Start sequence");
      Serial.print("sequence len=");
      Serial.println(seq_len);   
    
    #endif
    
    sequence_len=seq_len;   
    sequence_index=0;
    this->sequence=sequence;
  
    state=ST_BLINKING; 
  
    startBlink(sequence[0]);    
}
    
/**
 Interrompe il blink e lascia il led nello stato attuale
*/ 
void Blinker::stopBlink()
{
  if (state==ST_BLINKING)
  {
    
    #ifdef _DEBUG_BLINKER_
    Serial.println("stop blinking");  
    #endif
    
    state=ST_IDLE; 
    //resetta la eventuale sequenza
    sequence=0;
    sequence_len=0;
    sequence_index=0;
    
  }  
}
      
/**
 Metodo da invocare nel ciclo loop del programma per aggiornare il led
*/ 
void Blinker::updateState()
{
  if (state==ST_BLINKING)
  {        
    unsigned long t=millis();
    
    if ((t-last_time)>=cur_interval)
    {
      
      
#ifdef _DEBUG_BLINKER_
      
        Serial.print("t=");
        Serial.println(t);
        Serial.print("last_time=");
        Serial.print(last_time);
        Serial.print("cur_int=");
        Serial.println(cur_interval);
        Serial.print("repeat_cnt=");
        Serial.println(repeat_cnt);
        Serial.print("led_state=");
        Serial.println(led_state);  
        Serial.print("state=");
        Serial.println(state);

#endif            
        
        last_time=t;
        
        led_state = !led_state;
        
        cur_interval=led_state ? interval_on:interval_off;
        
        //commuta il led
        this->switchLed(led_state);         
        
        if (repeat_num!=-1) {
                
          ++repeat_cnt;
        
        }
        
        if (repeat_cnt>=repeat_num && repeat_num!=-1)
        {

            if (sequence && sequence_len && ++sequence_index<sequence_len) {                
                               
                state=ST_IDLE;
                
                #ifdef _DEBUG_BLINKER_
                  
                  Serial.print("**** next sequence_index=");
                  Serial.print(sequence_index);
                  Serial.println("****");
                  
                #endif
              
                startBlink(sequence[sequence_index]);                  
            }          
            else {
              
              stopBlink();  
            
            }
        }
      
    }    
  }
  
}
        
/**
  Restituisce lo stato corrente
*/  
bool Blinker::isBlinking()
{
    return state==ST_BLINKING;  
}


