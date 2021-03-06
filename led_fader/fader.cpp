
#include "fader.h"

Fader::Fader(int pin)
{
  attachTo(pin);
   pinMode(pin,OUTPUT); //questo non è necessario con analogWrite  
}

Fader::~Fader()
{  
}
  
  /**Collega il fader al pin analogico su cui è connesso il led
  i pin analogici di Arduino UNO sono 3,5,6,9,10,11
  */
bool Fader::attachTo(int pin)
{
  init_ok=false;
  
  if (pin==3 || pin==5 || pin==6 || pin==9 || pin==10 || pin==11)
  {
    this->pin=pin;
    init_ok=true;
    return true; 
  } 
  
  return false;
}
  
  /**Determina se è attivo un ciclo di fading*/
bool Fader::isFading()
{
  return is_fading;
}
  
  /**Interrompe il fading*/
void Fader::stopFading()
{
  is_fading=false;
}

  /**Imposta l'intensità (0-255)*/
bool Fader::setIntensity(byte intensity)
{
  if (init_ok)
  {
    analogWrite(pin,intensity);    
  }  
}
  
/**Legge l'intensità*/
byte Fader::getIntensity()
{
  return analogRead(pin);
}
  
  /**Avvia il fading
  @param delay_millis intervallo in millisecondi fra uno step e l'altro
  @param fade_step incremento/decremento intensità ad ogni step
  @param forward se settato da sepnto a massima intensità
  @param repeat numero ripetizioni, se -1 ciclo infinito
  */
bool Fader::startFading(int delay_millis,unsigned fade_step,bool forward,int repeat)
{
  delay_interval=delay_millis;
  this->fade_step=fade_step;
  fade_forward=forward;  
  this->repeat=repeat;
  cnt=0;
  
  is_fading=true;
  last_time=millis();
  
  if (forward)
  {
    cur_intensity=0;
    this->fade_step=fade_step;
    
  }
  else 
  {
    cur_intensity=0xFF;
    this->fade_step=-fade_step;    
    
  }      
      
  setIntensity(cur_intensity);
   
}
  
  /**
  Aggiorna lo stato di fading:deve essere invocata ad ogni ciclo del programma
  @return false se ha terminato il ciclo
  */
bool Fader::updateState()
{
  if (!is_fading) return false;
  
  unsigned long t=millis();
  
  if ((t-last_time)>=delay_interval)
  {
    
    last_time=t;
    
    cur_intensity+=fade_step;
    
    if (cur_intensity>0xFF) cur_intensity=0xFF;
    else if (cur_intensity<0) cur_intensity=0;
    
    setIntensity(cur_intensity);    
       
    if (cur_intensity==0xFF || cur_intensity==0)
    {
      //inverte la direzione
      this->fade_step=-this->fade_step;                  
      
      ++cnt;
      
      if (cnt>=repeat)
      {
        is_fading=false;        
      }
      
    }              
        
  }
  
  return is_fading;
  
}
