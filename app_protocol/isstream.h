/**
 Interfaccia stream per l'utilizzo con lbproto.
 Specifica il metodo che deve implementare lo stream seriale
 da usare con il protocollo lbproto
*/

#ifndef _ISSTREAM_INC_
#define _ISSTREAM_INC_

#define INFINITE_TIMEOUT -1

#include <stddef.h>

class ISStream
{
  public:
  
  virtual ~ISStream()
  {  
  }  
    
  /**
  Attende l'input per un timeout specificato.
  Se non ci sono dati disponibili nel buffer dopo il timeout termina l'attesa
  */
  virtual void waitForInput(int timeout_millis)=0;
  
  /**
    Legge un carattere dallo stream
  */  
  virtual char readChar()=0;  
  
  /**
   Restituisce il byte (carattere) successivo senza rimuoverlo dallo stream
  */ 
  virtual int peek()=0;
  
  /**
   Scrive un carattere
  */ 
  virtual void writeChar(const char ch)=0;
  
  /**
  Scrive un float
  */ 
  virtual void write(float f)=0;
  
  /**
  Scrive un double
  */
  virtual void write(double d)=0;
  
  /**
  Scrive un intero
  */
  virtual void write(int v)=0;
  
  /**
  Legge un buffer
  @length numero di caratteri nel buffer
  @return il numero di caratteri letti
  */
  virtual size_t readBuffer(char *buffer, size_t length)=0;
  
  /**
  Scrive un buffer
  @param length lunghezza del buffer
  @return il numero di caratteri scritti
  */
  virtual size_t writeBuffer(const char *buffer, size_t length)=0;

  /**
    Determina se ci sono dati disponibili
  */  
  virtual bool dataAvailable()=0;   
  
};

#endif
