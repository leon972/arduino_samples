#include "rs232stream.h"
#include "Arduino.h"

Rs232Stream::Rs232Stream()
{
  
}
  
Rs232Stream::~Rs232Stream()
{
  
}

/**
Avvia la comunicazione seriale
*/
void Rs232Stream::initComm(int baudRate)
{  
  Serial.begin(baudRate);    
}

/**
Attende l'input per un timeout specificato.
Se non ci sono dati disponibili nel buffer dopo il timeout termina l'attesa
*/
void Rs232Stream::waitForInput(int timeout_millis)
{
  unsigned long t=millis();
  
  while (!Serial.available() && ((millis()-t)<timeout_millis || timeout_millis==INFINITE_TIMEOUT))
  {    
    delay(40);
  }
  
}
  
  /**
    Legge un carattere dallo stream
  */  
char Rs232Stream::readChar()
{
  if (Serial.available()) return Serial.read();
  else return 0x0;  
}

int Rs232Stream::peek()
{
  return Serial.peek();  
}
  
/**
 Scrive un carattere
*/ 
void Rs232Stream::writeChar(const char ch)
{
  Serial.write(ch);
}

/**
  Scrive un float
  */ 
  void Rs232Stream::write(float f)
  {
    Serial.print(f);  
  }
  
  /**
  Scrive un double
  */
  void Rs232Stream::write(double d)
  {
    Serial.print(d);  
  }
  
  /**
  Scrive un intero
  */
  void Rs232Stream::write(int v)
  {
    Serial.print(v);  
  }
  
  /**
  Legge un buffer
  @length numero di caratteri nel buffer
  @return il numero di caratteri letti
  */
size_t Rs232Stream::readBuffer(char *buffer, size_t length)
{
  return Serial.readBytes(buffer, length); 
}
  
  /**
  Scrive un buffer
  @param length lunghezza del buffer
  @return il numero di caratteri scritti
  */
size_t Rs232Stream::writeBuffer(const char *buffer, size_t length)
{
  Serial.print(buffer);  
}

/**
  Determina se ci sono dati disponibili
*/  
bool Rs232Stream::dataAvailable()
{
  return Serial.available();  
}
