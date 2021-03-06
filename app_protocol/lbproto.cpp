/**
 Protocollo di invocazione funzioni attraverso comunicazione seriale.
 Lo scopo del protocollo è quello di permettere di invocare funzioni
 passando i parametri ed acquisendo il risultato.
 
 Code by Leonardo Berti (c) 2012
 
 */

#include "lbproto.h"
#include "Arduino.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

///////////////////////// FunctionCaller ////////////////////////////

FunctionCaller::FunctionCaller(ISStream *outStream, char *callBuffer, size_t callBufferSize) {

    this->stream = outStream;

    outBuffer.setWrappedBuffer(callBuffer, callBufferSize);

    firstParam = false;

    last_error = ERR_NOERROR;

    state = ST_READY;

}

/**Inizia una chiamata di funzione
@param fnname nome funzione
@return true se è possibile avviare una chiamata 
 */
bool FunctionCaller::beginFunctionCall(const char* fname) {
    if (!(state == ST_READY || state == ST_INVOKED)) {
        last_error = ERR_INVALID_STATE;
        return false;
    }

    size_t sz = strlen(fname);

    outBuffer.setLength(0);
    
    firstParam=false;

    if (!outBuffer.append(fname)) {
        last_error = ERR_BUFFER_OVERRUN;
    }

    outBuffer.append(PAR_OPEN);

    state = ST_CALLING;

    return true;
}

/**
 * Svuota il buffer di input della porta seriale
 * (legge tutti i dati in attesa, usato per iniziare una nuova chiamata)
 */
void FunctionCaller::resetInputStream() {

    while (stream->dataAvailable()) {
        stream->readChar();
    }
}

/**
  Restituisce il messaggio di errore dell'ultimo errore verificatosi
 */
int FunctionCaller::getLastError() const {
    return last_error;
}

void FunctionCaller::resetState() {

    outBuffer.setLength(0);
    last_error = ERR_NOERROR;
    state = ST_READY;

}

void FunctionCaller::sendParamSep() {
    if (firstParam) {
        outBuffer.append(ARG_SEP); // ','
    } else firstParam = true;

}

bool FunctionCaller::checkState(int req_state) {
    if (state != req_state) {
        last_error = ERR_INVALID_STATE;
        return false;
    }

    return true;
}

/**
Immette un parametro intero
@return true se tutto è ok
 */
bool FunctionCaller::pushParamInt(int paramValue) {
    if (!checkState(ST_CALLING)) return false;

    sendParamSep();

    return outBuffer.append(paramValue);

}

/**
Immette un parametro float
 */
bool FunctionCaller::pushParamFloat(float floatValue) {
    if (!checkState(ST_CALLING)) return false;

    sendParamSep();

    if (!outBuffer.append(floatValue)) {
        last_error = ERR_BUFFER_OVERRUN;
        return false;
    }

    return true;

}

/**
Immette un parametro char
 */
bool FunctionCaller::pushParamChar(char ch) {
    if (!checkState(ST_CALLING)) return false;

    sendParamSep();

    bool r = true;

    r &= outBuffer.append('\'');
    r &= outBuffer.append(ch);
    r &= outBuffer.append('\'');

    if (!r) {
        last_error = ERR_BUFFER_OVERRUN;
    }

    return r;
}

/**
Immette un parametro boolean
 */
bool FunctionCaller::pushParamBool(bool value) {

    if (!checkState(ST_CALLING)) return false;

    sendParamSep();

    bool r = true;

    if (value) {

        r = outBuffer.append(LTRUE);

    } else {

        r = outBuffer.append(LFALSE);
    }

    return r;

}

/**
Immette un parametro stringa
 */
bool FunctionCaller::pushParamString(const char* stringValue) {
    if (!checkState(ST_CALLING)) return false;

    sendParamSep();

    if (!outBuffer.append(STRING_SEP)) return false;

    if (!outBuffer.append(stringValue));

    if (!outBuffer.append(STRING_SEP)) return false;

    return true;

}

/**
 Avvia l'immisione di un vettore
 (dopo questo metodo usare i metodi pushParam per inviare gli elementi del vettore)
 */
bool FunctionCaller::beginVector() {

    if (!checkState(ST_CALLING)) return false;

    sendParamSep();

    if (!outBuffer.append(VEC_OPEN)) return false;

    state = ST_BEGIN_VECT;

    return true;

}

/**
Termina l'immisione di un vettore
 */
bool FunctionCaller::endVector() {

    if (!checkState(ST_BEGIN_VECT)) return false;

    if (!outBuffer.append(VEC_CLOSE)) return false;

    state = ST_CALLING;

    return true;

}

/**
Esegue la chiamata inviando i dati attraverso la porta seriale  
 */
bool FunctionCaller::call() {

    if (!checkState(ST_CALLING)) return false;

    if (!outBuffer.append(PAR_CLOSE)) return false; //parentisi fine parametri

    size_t sz = stream->writeBuffer(outBuffer.getBuffer(), outBuffer.getLength());

    if (sz != outBuffer.getLength()) {
        last_error = ERR_COMM_ERROR;
        return false;
    }

    resetResultBuffer();

    state = ST_INVOKED;

    return true;

}

void FunctionCaller::resetResultBuffer() {
  
    resultBuffer = 0;
    resultBufferLen = 0;
    res_start = -1;
    res_end = -1;
}

void FunctionCaller::getResultValue(char **resValue, size_t *resSize) {

}

/**
     Indica se c'è un risultato
 */
bool FunctionCaller::hasResult() {

    return resultBuffer != 0;

}

/**
 * Acquisisce il risultato della chiamata di funzione nel buffer resultBuffer
 * @param resultBuffer il buffer su cui viene caricato il risultato
 * (il buffer resultBuffer viene resettato in ogni caso) 
 * il buffer del risultato contiene una stringa del tipo [<valore>] dove valore puo' essere un numero,una stringa ecc...
 * @param resultBufferSize
 * @return true se c'è un risultato nel buffer di input
 */
bool FunctionCaller::getResult(char* resultBuffer, size_t resultBufferSize) {

    resetResultBuffer();
  
    int i = 0;      
    char ch = 0x0; 
  
    if (stream->peek() == RES_OPEN) {  //[
     
      //risultato    
                      
      memset(resultBuffer,0,resultBufferSize);
        
      while (stream->dataAvailable() && i < resultBufferSize - 1) {
  
          ch = stream->readChar();
  
          resultBuffer[i++] = ch;
  
          if (ch == RES_CLOSE) {
              break;
          }
  
      }
  
      if (ch == RES_CLOSE) {
  
          this->resultBuffer = resultBuffer;
          this->resultBufferLen = i;
          return true;
          
      } else return false;
    
    }
    else if (stream->peek()=='E')
    {
       //eccezione
       
       //sequenza attesa
       const char expected[]={PAR_OPEN,QUOTE,QUOTE,PAR_CLOSE};
       const char* pexpc=expected;
                    
       memset(resultBuffer,0,resultBufferSize);
       
       while (stream->dataAvailable() && i < resultBufferSize - 1) {
      
          ch = stream->readChar();         
                  
          if (ch == *pexpc) 
          {              
              pexpc++;                                                      
          }
          
          resultBuffer[i++] = ch;
  
          if (ch == PAR_CLOSE) {
              break;
          }  
      } 
        
      if ((pexpc-expected)==4) {
  
          this->resultBuffer = resultBuffer;
          this->resultBufferLen = i;
          
          if (exceptionCaught())
          {
            return true;            
          }
          else
          {
            resetResultBuffer();            
          }         
                    
      } else return false;    
             
      
    }    
    else return false;
}

/**
 * Determina se dopo la chiamata, è stata restituita una eccezione
 * @return
 */
bool FunctionCaller::exceptionCaught() {

    if (!hasResult()) return false;
    
    if (strncmp(resultBuffer,"EXCEPTION(",10)==0)
    {
        return true;
    }
    return false;
}

/**
 * Acquisisce il messaggio di eccezione
 * @param errBuffer
 * @param errBufferSize
 * @return true se si è verificata una eccezione
 */
bool FunctionCaller::getException(char* errBuffer, size_t errBufferSize) {

    if (exceptionCaught())
    {

        int i=10; //punta alla parentesi di apertura EXCEPTION(
        int p=0;

        if (resultBuffer[i++]!='"') {
            return false;
        }

        memset(errBuffer,0,errBufferSize);

        while (p<errBufferSize-1 && i<resultBufferLen)
        {
            char ch=resultBuffer[i++];

            if (ch!='"') {
                errBuffer[p++]=ch;
            }
            else break;
        }

        return true;
    }
}

/**
 Acquisisce il risultato della chiamata come un intero
 */
int FunctionCaller::intResult() {
  
  if (hasResult())
  {
        
    int intValue=0;
    sscanf(&resultBuffer[1], "%d", &intValue);  
    return intValue;  
  }  
  
  return 0;

}

/**
Acquisisce il risultato come floating point
 */
float FunctionCaller::floatResult() {
  
  if (hasResult())
  {
        
    float fValue=0;
    sscanf(&resultBuffer[1], "%d", &fValue);  
    return fValue;  
  }  
  
  return 0;

}

/**
Acquisisce il risultato come carattere
 */
char FunctionCaller::charResult() {
  
  if (hasResult() && resultBufferLen==5) //es. ['A']
  {
    if (resultBuffer[1]=='\'' && resultBuffer[3]=='\'') return resultBuffer[2];        
  }
  
  return 0;

}

/**
 Acquisisce il risultato boolean
 */
bool FunctionCaller::boolResult() {  //es. [true]
  
  if (hasResult())
  {
    
    const char* ptr=&resultBuffer[1];
    
    if (strncmp(ptr,"true",4)==0) return true;
    if (strncmp(ptr,"false",4)==0) return false;
    if (resultBuffer[1]=='1' && resultBufferLen==3) return true;
    if (resultBuffer[1]=='0' && resultBufferLen==3) return false;
    
  }
  
  return false;

}

/**
Acquisisce il risultato come stringa
 */
char* FunctionCaller::stringResult(char* dest,size_t dest_buff_size)
{
  if (!dest) return dest;  
  
  memset(dest,0,dest_buff_size);
  
  if (hasResult())
  {
    if (resultBuffer[1]==QUOTE && resultBuffer[resultBufferLen-2]==QUOTE)
    {
      size_t sz=resultBufferLen-4 < dest_buff_size-1 ? resultBufferLen-4:dest_buff_size-1; //nota:preserva lo zero terminatore del dest buffer
 
      memcpy(dest,&resultBuffer[2],sz);
     
     return dest;  
      
    }     
  }
  
  return 0;

}

///////////////////////// FunctionCall ///////////////////////////

/**
 Wrapper per la chiamata di una funzione sulla porta seriale attraverso il protocollo
 lbproto
 */
//class FunctionCall
//{
//  
//  private:
//  
//  ISStream& stream;
//  
//  char* last_error;
//  
//  public:

FunctionCall::FunctionCall(ISStream *stream) {
    this->stream = stream;

    last_error = "<no error>";

    state = ST_READY;
}

FunctionCall::~FunctionCall() {
}

/**
 Legge dalla porta seriale l'eventuale invocazione di una funzione
 (es. MY_FUNCTION(....)
 e restituisce true se sullo stream c'è l'invocazione di una funzione
 @return true se il parsing ha successo
 */
bool FunctionCall::readFunctionName(char *destName, size_t maxlen) {

    if (stream->dataAvailable()) {
        //procede nella lettura se ci sono dati disponibili
        if (!checkState(ST_READY)) return false;

        int rd = 0;
        char ch = 0x0;
        bool parseOk = false;

        //resetta
        memset(destName, 0, maxlen);

        size_t limit = maxlen - 1;

        //Serial.println("begin read fun....");

        while (rd < limit && ch != PAR_OPEN && stream->dataAvailable()) {
            ch = stream->readChar();

            // Serial.print("ch=");
            // Serial.println(ch);

            if (ch == PAR_OPEN) {
                // Serial.println("PARSE OK");
                parseOk = true;
                break;
            }

            destName[rd++] = ch;

            // Serial.println(destName);
        }

        if (parseOk) {
            state = ST_READ_ARGS;
        } else {
            //resetta
            memset(destName, 0, maxlen);
            state = ST_ERROR;
            last_error = "invalid call";

        }

        return state == ST_READ_ARGS;
    } else return false;
}

char* FunctionCall::getLastError() {
    return last_error;
}

void FunctionCall::resetState(bool emptyBuffer) {

    last_error = "<no error>";
    state = ST_READY;    

    if (emptyBuffer) {
        while (stream->dataAvailable()) {
            stream->readChar();
        }
    }
}

/**
    Determina se l'oggetto è pronto per la lettura di una funzione
    @return true dopo l'inizializzazione o quando ha finito la lettura di una funzione con successo
 */
bool FunctionCall::isReady() {
    return state == ST_READY;
}

/**
  Determina se è in stato di errore.Per resettare lo stato usare resetState
 */
bool FunctionCall::isError() {
    return state = ST_ERROR;
}

bool FunctionCall::checkState(int req_state) {
    if (state != req_state) {
        last_error = "Invalid state";
        return false;
    }

    return true;
}

/**
  Legge un argomento non stringa
 */
bool FunctionCall::readArg(char* dest, size_t destLen) {
    if (!checkState(ST_READ_ARGS)) return false;

    int rd = 0;
    char ch = 0x0;
    size_t limit = destLen - 1;

    memset(dest, 0, destLen);

    while (rd < limit && stream->dataAvailable()) {

        ch = stream->readChar();
       
        if (ch == ARG_SEP) {           
            return true;
        }

        if (ch == PAR_CLOSE) {
            state = ST_READY; //fine funzione
            return true;
        }

        dest[rd++] = ch;
    }

    state = ST_ERROR;
    last_error = "error reading argument";
    return false;
}

//legge fino alla chiusura dell'argomento

bool FunctionCall::readCloseArg() {
    char ch = stream->readChar();

    if (ch == ARG_SEP) return false;
    if (ch == PAR_CLOSE) {
        state = ST_READY;
        return false;
    }

    last_error = "bad argument separator";
    state = ST_ERROR;
    return false;
}

/**
 Legge un argomento int
 */
bool FunctionCall::readIntArg(int *intValue) {
    if (readArg(tempArg, MAX_ARG_SIZE)) {
        sscanf(tempArg, "%d", intValue);
        return true;
    }
    return false;
}

/**
 Legge un argomento float
 */
bool FunctionCall::readFloatArg(float *floatValue) {
    if (readArg(tempArg, MAX_ARG_SIZE)) {
        sscanf(tempArg, "%f", floatValue); //sscanf non va nel caso float!
        return true;
    }
    return false;

}

/**
Legge un argomento carattere es. 'A'
 */
bool FunctionCall::readCharArg(char *chValue) {
    if (!checkState(ST_READ_ARGS)) return false;

    char ch = stream->readChar();

    if (ch != '\'') {
        state = ST_ERROR;
        last_error = "error reading char arg";
        return false;
    }

    *chValue = stream->readChar();

    if (*chValue == 0x0) {
        state = ST_ERROR;
        last_error = "error reading char arg";
        return false;
    }

    ch = stream->readChar();

    if (ch != '\'') {
        state = ST_ERROR;
        last_error = "error reading char arg";
        return false;
    }

    if (!readCloseArg()) return false;

    return true;

}

/**
 Legge un argomento bool le costanti valide sono: 1,0,true,false
 */
bool FunctionCall::readBoolArg(bool *bValue) {
    if (readArg(tempArg, MAX_ARG_SIZE)) {
        if (strcmp(tempArg, "true") == 0 || strcmp(tempArg, "1") == 0) {
            *bValue = true;
        } else if (strcmp(tempArg, "false") == 0 || strcmp(tempArg, "0") == 0) {
            *bValue = false;
        } else {
            state = ST_ERROR;
            last_error = "error reading bool arg";
        }
    } else {
        last_error = "error reading bool arg";
        return false;
    }
}

/**
Legge un argomento stringa es. "ABCD"
 */
bool FunctionCall::readStringArg(char* argValue, unsigned arg_size) {
    if (!checkState(ST_READ_ARGS)) return false;

    char ch = stream->readChar();

    if (ch != '"') {
        last_error = "string arg bad format";
        state = ST_ERROR;
        return false;
    }

    int rd = 0;
    ch = 0x0;
    bool escape;

    while (rd < arg_size - 1) {

        ch = stream->readChar();

        if (ch == '\\') {
            escape = true;
            ch = stream->readChar();
        } else escape = false;

        if (ch == 0x0) {
            state = ST_ERROR;
            last_error = "unterminated string arg";
        }

        if (ch == '"' && !escape) //fine stringa
        {
            if (!readCloseArg()) {
                return false;
            }
            return true;
        }

        argValue[rd] = ch;

    }
}

/**
Invia una eccezione in risposta alla chiamata di funzione
 */
void FunctionCall::raiseException(const char* message) {

    const char *msg = "EXCEPTION";
    stream->writeBuffer(msg, strlen(msg));
    stream->writeChar(PAR_OPEN);
    stream->writeChar('"');
    stream->writeBuffer(message, strlen(message));
    stream->writeChar('"');
    stream->writeChar(PAR_CLOSE);
}


