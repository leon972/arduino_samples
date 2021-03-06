/**
  Classe usata per gestire più led contemporaneamente
  (giochi di luce)  
*/

#include "flights.h"
#include "Arduino.h"

FunLights::FunLights()
{
  state=ST_IDLE;
  cur_pattern.mode=FL_NONE;
}

FunLights::~FunLights()
{
  sequence=0;
}
    
/**
Imposta i leds da gestire
*/
void FunLights::setLeds(IBlinkable** leds,int nleds)
{
  if (state==ST_IDLE)
  {
      this->leds=leds;
      this->nleds=nleds;      
  }
    
}
    
/**
Accende o spenge il led
@param mode ON o OFF
*/
void FunLights::switchLed(int mode)
{  
  for (int i=0;i<nleds;i++)
  {
    leds[i]->switchLed(mode);      
  }        
}
    
/**Avvia il blinking su tutti i led contemporaneamente usando un blink pattern*/
bool FunLights::startBlink(BLINK_PATTERN &blink_pattern)
{
  bool r=true;
  
  if (leds)
  {
    for (int i=0;i<nleds;i++)
    {
      r=r & leds[i]->startBlink(blink_pattern);      
    }     
  }  
  
  return r;
}

bool FunLights::startBlink(int interval_on,int interval_off,int repeat)
{
  
  bool r=true;
  
  if (leds)
  {
    for (int i=0;i<nleds;i++)
    {
      r=r & leds[i]->startBlink(interval_on,interval_off,repeat);      
    }     
  }  
  
  return r;  
  
}
   
/**
Avvia una sequenza di blinking composta da più pattern
l'accensione/spegnimento avviene su tutti i blinker contemporaneamente
come se fossero un unico oggetto
Il blinker deve essere in stato IDLE
@return true se la sequenza puo' essere avviata    
*/    
bool FunLights::startSequence(BLINK_PATTERN* sequence,int seq_len)
{
  bool r=true;
  
  if (leds)
  {
    for (int i=0;i<nleds;i++)
    {
      r = r & leds[i]->startSequence(sequence,seq_len);      
    }     
  }    
  
  return r;
  
}
   
/**
 Interrompe il blink e lascia il led nello stato attuale
*/ 
void FunLights::stopBlink()
{
  if (leds)
  {
    for (int i=0;i<nleds;i++)
    {
      leds[i]->stopBlink();            
    }   
  }
  
}

void FunLights::moveLights(bool toggle_prev)
{
  
  unsigned long t=millis();    
      
  int cled=cur_cnt;
  int prev_led=cled-1;
 
  if (!cur_pattern.forward)
  {
    cled=nleds-cur_cnt-1;
    prev_led=cled+1;
  
  }  
  
  if ((t-last_time)>cur_pattern.interval) {            
                
        last_time=t;
        
        //accende il led successivo
        if (cled>=0 && cled<nleds) {
          leds[cled]->switchLed(cur_pattern.positive ? ON:OFF);  
         
           Serial.println("------");
         
           Serial.print("SWITCH  ");
           Serial.print(cled);
           Serial.print(" ");
           Serial.println(cur_pattern.positive ? ON:OFF);
        }
        
        if (prev_led>=0 && prev_led<nleds && toggle_prev)
        {
          //spegne il precedente
          leds[prev_led]->switchLed(cur_pattern.positive ? OFF:ON); 
          Serial.print("SWITCH PREV ");
           Serial.print(prev_led);
           Serial.print(" ");
           Serial.println(cur_pattern.positive ? OFF:ON);
          
        }  

        Serial.println("------");                
                
        ++cur_cnt;                         
        
        //nota:il limite è cnt+1 per permettere si spegnere il led precedente all'ultimo passo
        if (cur_cnt>=max_cnt+1)
        {
          nextPattern();  
        }
    
   }
}
      
/**
 Metodo da invocare nel ciclo loop del programma per aggiornare il led
*/ 
void FunLights::updateState()
{ 
  
  if (cur_pattern.mode==FL_NONE) {
    
    //blink sincrono
    if (leds)
    {
      for (int i=0;i<nleds;i++)
      {
        leds[i]->updateState();            
      }   
    }    
  }
  else
  {            
      
      switch (cur_pattern.mode)
      {
        //luce che si sposta in sequenza sui led
        case FL_MOVE:
          
          moveLights(true);
        
        break;
        //barra
        case FL_BAR:
        
          moveLights(false);
          
        break;
        //random
        case FL_RANDOM:
        
        unsigned long t=millis();
        
        if (t-last_time>7000)
        {
          nextPattern();  
        }
        else {
          
          for (int i=0;i<nleds;i++)
          {                        
            
            leds[i]->updateState();
            
            if (!leds[i]->isBlinking())
            {
                leds[i]->startBlink(random(cur_pattern.interval),random(cur_pattern.max_interval),random(5,40));                                  
            }            
          }
          
        }      
        
        break;
        
      }        
  }
  
}

bool FunLights::nextPattern()
{
  cur_pattern.mode=FL_NONE;  
    
  if (sequence)
  {
      if (++sequence_index<sequence_len)
      {
        
        Serial.print("idx=");
        Serial.print(sequence_index);
        Serial.print("len=");
        Serial.println(sequence_len);
        
        
        return startFunLights(&sequence[sequence_index]);        
      }             
  } 
  
  Serial.println("end seq");
  
  switchLed(OFF);
  
  stopBlink();
  
  return false;  
}
    
/**
  Restituisce lo stato corrente
*/  
bool FunLights::isBlinking()
{
  if (cur_pattern.mode!=FL_NONE) return true; //sequenza in corso
  
  if (leds)
  {
    for (int i=0;i<nleds;i++)
    {
      if (leds[i]->isBlinking()) return true;            
    }   
  }   
  
  return false;
}
   
 //metodi specifici di questa classe 
 /**
  Accende un led alla volta in sequenza in modo da generare un effetto movimento
  @param forward se settato, va dal led 0 all'ultimo
  altrimenti va in senso inverso
  @param interval intervallo di tempo nel passaggio da uno all'altro
  @param positive se settato il led che si sposta è accesso altrimenti è spento
  e tutti gli altri accesi
 */
 bool FunLights::startMoving(bool forward,int interval,bool positive)
 {
   
   Serial.println("startMoving");
   
   switchLed(positive ? OFF:ON);
   
   last_time=millis()-interval*2;
   
   cur_pattern.mode=FL_MOVE;
   state=ST_BLINKING;
   cur_pattern.forward=forward;
   cur_pattern.interval=interval;
   cur_pattern.positive=positive; 
   cur_cnt=0;
   max_cnt=nleds;
 }
   
   /**
    Accensione in sequenza (effetto barra indicatrice)
   */
bool FunLights::startBar(bool forward,int interval,bool positive)
{
  
  Serial.println("startBar");
  
  switchLed(positive ? OFF:ON);
  
  last_time=millis()-interval*2;
  
  cur_pattern.mode=FL_BAR;
  state=ST_BLINKING;
  cur_pattern.forward=forward;
  cur_pattern.interval=interval;
  cur_pattern.positive=positive;
  cur_cnt=0;
  max_cnt=nleds;   
  
}
   

/**Accensione random*/
bool FunLights::startRandom(int min_interval,int max_interval)
{
  
  switchLed(OFF);
  
  last_time=millis();
  
  cur_pattern.mode=FL_RANDOM;
  state=ST_BLINKING;  
  cur_pattern.interval=min_interval;
  cur_pattern.max_interval=max_interval;   
  cur_cnt=0;
  max_cnt=40;
  
}

bool FunLights::startFunLightsSequence(FUN_LIGHTS_PATTERN* sequence,int seq_len)
{
      
  if (!sequence || !seq_len) return false;
  
  this->sequence=sequence;
  sequence_len=seq_len;  
  sequence_index=0;
  
  startFunLights(&sequence[0]);
  
  return true;  
  
}

/**
    Avvia un effetto luce
   */
bool FunLights::startFunLights(FUN_LIGHTS_PATTERN* flights)
{
  if (!flights || flights->mode==FL_NONE) return false;      
    
  switch (flights->mode)
  {
    
    case  FL_MOVE:   //luce che si sposta
    return startMoving(flights->forward,flights->interval,flights->positive);
    
    case FL_BAR:   //barra
    return startBar(flights->forward,flights->interval,flights->positive);
       
    case FL_RANDOM: //random 
    return startRandom(flights->interval,flights->max_interval);   
    
  }

  return false;  
  
}

