/**
  String builder che utilizza un array di caratteri con
  lunghezza fissa.
  
  Code by Leonardo Berti (c) 2012
  
  7-10-2012
  
*/

#ifndef _SBUILDER_INC_
#define _SBUILDER_INC_

#include <stddef.h>

#define MAX_CONV_BUFF_SIZE 32


class SBuilder {

  private:
  
  //buffer su cui vengono aggiunti i dati
  char *buffer;
  
  //dimensione del buffer buffer
  size_t buffSize;
  
  size_t buffLen;
  
  /**Controlla se è possibile aggiungere altri caratteri*/
  bool checkBuffer(size_t charReq);
  
  char tempConv[MAX_CONV_BUFF_SIZE];
  
  public:
  
  /**
  Crea lo string builder specificando il buffer e la dimensione massima
  @param buffsize dimensione massima
  */
  SBuilder(char* buffer,size_t buffSize);

  SBuilder();
  
  virtual ~SBuilder();
  
  /**
  Restituisce la lunghezza del buffer attuale
  */
  size_t getLength() const;
  
  /**
  Imposta la lunghezza del buffer
  */
  void setLength(size_t newLength);
  
  /**
  Acquisisce la dimensione del buffer
  */
  size_t getSize() const;

  /**
   * Imposta il buffer di output
   * @param ch
   * @return 
   */
  void setWrappedBuffer(char* buffer,size_t buffSize);

  /**
   * Restituisce il buffer destinazione
   * @return
   */
  const char *getBuffer() const;
  
  /**
  Aggiunge un carattere
  */
  bool append(char ch); 
  
  /**
  Aggiunge una stringa
  @param stringLen specifica il numero di caratteri da aggiungere
  @return true se la stringa può essere aggiunta
  */
  bool append(const char* string,size_t stringLen);

  /**
   * Aggiunge una stringa
   * @param value
   * @return true se il buffer ha abbastanza memoria per accettare la stringa
   */
  bool append(const char* string);
  
  /**
  Aggiunge un int
  */
  bool append(int value);
  
  /**
  Aggiunge un double
  */
  bool append(double value);

  /**
   *Aggiunge il testo preso da un altro builder
   */
  bool append(const SBuilder& builder);
  
};

#endif
