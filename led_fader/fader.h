/**
  Classe wrapper per la gestione del fading di un led
  usando la modulazione PWM su un pin analogico di Arduino
  
  Code by Leonardo Berti (c) 2012
  
*/  

#ifndef _FADER_INC_
#define _FADER_INC_

#include "Arduino.h"

class Fader
{
  private:

  int pin; //pin analogico
  int delay_interval; //intervallo in millisecondi fra un passo e l'altro
  int fade_step; //passo incremento/descrmento intensità
  bool is_fading;
  int repeat; //numero ripetizioni
  int cnt;
  bool fade_forward; //se settanto nel ciclo di fading va da spento a massima intensità
  bool init_ok;
  int cur_intensity;
  
  unsigned long last_time;
  
  public:

  Fader(int pin);  
  ~Fader();
  
  /**Collega il fader al pin analogico su cui è connesso il led
  i pin PWM di Arduino UNO sono 5,6,9,10,11
  */
  bool attachTo(int pin);
  
  /**Determina se è attivo un ciclo di fading*/
  bool isFading();
  
  /**Interrompe il fading*/
  void stopFading();

  /**Imposta l'intensità (0-255)*/
  bool setIntensity(byte intensity);
  
  /**Legge l'intensità*/
  byte getIntensity();
  
  /**Avvia il fading
  @param delay_millis intervallo in millisecondi fra uno step e l'altro
  @param fade_step incremento/decremento intensità ad ogni step
  @param forward se settato da sepnto a massima intensità
  @param one_shot se settato arrivato alla fine si interrompe (se false va in ciclo infinito fino a che si invoca stopFading)
  */
  bool startFading(int delay_millis,unsigned fade_step,bool forward,int repeat); 
  
  /**
  Aggiorna lo stato di fading:deve essere invocata ad ogni ciclo del programma
  @return false se ha terminato il ciclo
  */
  bool updateState();
     
};

#endif
