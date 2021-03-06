/**
 Implementazione dello stream caratteri utilizzando la comunicazione
 seriale con protocollo RS232 di Arduino
*/ 

#ifndef RS232STREAM
#define RS232STREAM

//interfaccia wrapper per lo stream seriale di default di Arduino
#include "isstream.h" 

class Rs232Stream:public ISStream
{
  private:
  
  public:
  
  /**
    @param baudRate velocità di comunicazione es. 9600
  */  
  Rs232Stream();
  
  virtual ~Rs232Stream();
  
  /**
  Apre la comunicazione seriale
  */
  void initComm(int baudRate);
  
  /**
  Attende l'input per un timeout specificato.
  Se non ci sono dati disponibili nel buffer dopo il timeout termina l'attesa
  */
  virtual void waitForInput(int timeout_millis);
  
  /**
    Legge un carattere dallo stream
  */  
  virtual char readChar(); 
 
  virtual int peek(); 
  
  /**
   Scrive un carattere
  */ 
  virtual void writeChar(const char ch);
  
  /**
  Scrive un float
  */ 
  virtual void write(float f);
  
  /**
  Scrive un double
  */
  virtual void write(double d);
  
  /**
  Scrive un intero
  */
  virtual void write(int v);
  
  /**
  Legge un buffer
  @length numero di caratteri nel buffer
  @return il numero di caratteri letti
  */
  virtual size_t readBuffer(char *buffer, size_t length);
  
  /**
  Scrive un buffer
  @param length lunghezza del buffer
  @return il numero di caratteri scritti
  */
  virtual size_t writeBuffer(const char *buffer, size_t length);

  /**
    Determina se ci sono dati disponibili
  */  
  virtual bool dataAvailable();    
  
};


#endif
