/**
  String builder che utilizza un array di caratteri con
  lunghezza fissa.
  
  Code by Leonardo Berti (c) 2012
  
  7-10-2012
  
 */

#include "sbuilder.h"
#include <string.h>
#include <stdio.h>

/**
Crea lo string builder specificando il buffer e la ddimensione massima
 */
SBuilder::SBuilder(char* buffer, size_t buffSize) {
    
    setWrappedBuffer(buffer, buffSize);

}

SBuilder::SBuilder()
{
    buffer=0;
    buffSize=0;
    buffLen=0;
}

SBuilder::~SBuilder() {

}

/**
 * Imposta il buffer di output
 * @param ch
 * @return
 */
void SBuilder::setWrappedBuffer(char* buffer, size_t buffSize) {

    this->buffer = buffer;
    this->buffSize = buffSize;
    memset(buffer, 0, buffSize);
    buffLen = 0;

}

/**Controlla se è possibile aggiungere altri caratteri*/
bool SBuilder::checkBuffer(size_t charReq) {

    if ((buffLen + charReq) >= buffSize) return false;
    else return true;

}

/**
  Restituisce la lunghezza del buffer attuale
 */
size_t SBuilder::getLength() const {
    return buffLen;
}

/**
Imposta la lunghezza del buffer
 */
void SBuilder::setLength(size_t newLength) {
    if (newLength >= buffSize) return;

    for (size_t i = newLength; i < buffSize; i++) {
        buffer[i] = 0x0;
    }

    buffLen = newLength;
}

/**
Acquisisce la dimensione del buffer
 */
size_t SBuilder::getSize() const {
    return buffSize;
}

/**
 * Restituisce il buffer destinazione
 * @return
 */
const char* SBuilder::getBuffer() const {
    return buffer;
}

/**
Aggiunge un carattere
 */
bool SBuilder::append(char ch) {
    if (buffLen < buffSize - 1) {
        buffer[buffLen++] = ch;
    }

    return true;
}

/**
Aggiunge una stringa
@param stringLen specifica il numero di caratteri da aggiungere
@return true se la stringa può essere aggiunta
 */
bool SBuilder::append(const char* string, size_t stringLen) {

    if (string && checkBuffer(stringLen)) {

        memcpy(&buffer[buffLen], string, stringLen);

        buffLen += stringLen;

        return true;

    } else return false;

}

/**
 * Aggiunge una stringa
 * @param value
 * @return true se il buffer ha abbastanza memoria per accettare la stringa
 */
bool SBuilder::append(const char* astring) {

    return append(astring, strlen(astring));

}

/**
Aggiunge un int
 */
bool SBuilder::append(int value) {

    memset(tempConv, 0, MAX_CONV_BUFF_SIZE);

    sprintf(tempConv, "%d", value);

    size_t sz = strlen(tempConv);

    if (checkBuffer(sz)) {
        memcpy(&buffer[buffLen], tempConv, sz);
    }

    buffLen += sz;

    return true;
}

/**
Aggiunge un double
 */
bool SBuilder::append(double value) {

    memset(tempConv, 0, MAX_CONV_BUFF_SIZE);

    sprintf(tempConv, "%f", value);

    size_t sz = strlen(tempConv);

    if (checkBuffer(sz)) {
        memcpy(&buffer[buffLen], tempConv, sz);
    }

    buffLen += sz;

    return true;
}

/**
 *Aggiunge il testo preso da un altro builder
 */
bool SBuilder::append(const SBuilder& builder) {
    return append(builder.getBuffer(), builder.getLength());
}
