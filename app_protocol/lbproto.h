/**
 Protocollo di invocazione funzioni attraverso comunicazione seriale.
 Lo scopo del protocollo è quello di permettere di invocare funzioni
 passando i parametri ed acquisendo il risultato.
 
 Code by Leonardo Berti (c) 2012
 
 Specifica:
 
 Invocazione di una funzione da parte del master: 
  
 <nome funzione>'('<elenco parametri>')'
 
 <elenco parametri> := <parametro>[','<parametro>]
 
 <parametro>:=<numero intero>|<numero floating point>|<stringa>|<bool>|<carattere>|<vettore>|<null>
 
 <numero intero>: esempio 10 , -12, 0 ecc..
 <numero floating point>: exempio: 0.009, 12.99 , -6 ecc...
 <stringa>:='"'<carattere>[<carattere>]'"' | <null>
 <bool>:= 'true'|'false'
 <carattere> := es. 'A', 'B', 'c' , '' ecc... 
 <vettore> := '{'<parametro>[,<parametro>]<'}' | <null>
 <null> := 'null'
   
 il master dopo l'invocazione può attendere il risultato della chiamata
 che puo' anche essere una eccezione.
 
 Il risultato viene inviato usando i separatori [ e ]

 <eccezione>:= 'EXCEPTION("<messaggio>")'
 <messaggio>:=<stringa> 
 
 Esempio:
 invocazione funzione dal master:
 
 MY_FUNCTION(12,-7.8,"test",true)
 
 risultato inviato dallo slave:
 [123]
 
 oppure:
 ["risultato stringa"]
 
 o in caso di vettore:
 
 [{1,2,3,199,202,77}]
 
 o in caso di eccezione
 [EXCEPTION("si è verificato un errore...")] 
 
 */

#ifndef _LBPROTO_INC_
#define _LBPROTO_INC_

//interfaccia SStream 
#include "isstream.h"
//string builder
#include "sbuilder.h"

#define ARG_SEP ','
#define PAR_OPEN '('
#define PAR_CLOSE ')'
#define VEC_OPEN '{'
#define VEC_CLOSE '}'
#define STRING_SEP '"'
#define RES_OPEN '['
#define RES_CLOSE ']'
#define QUOTE '"'

#define LTRUE "true"
#define LFALSE "false"

//#define TEMP_BUFF_SIZE 32

/**
  Invoca una funzione immettendo nello stream seriale
  il nome seguito dai parametri.
  Per invocare una funzione iniziare la chiamata con beginFunctionCall
  specificando il nome della funzione,immettere i parametri dell'invocazione,
  invocare il metodo call e acquisire l'output tramite le funzioni result
  se si è verifica una eccezione controllare exceptionCaught.
  
  (master)
 */
class FunctionCaller {
private:

    //stato interno

    static const int ST_READY = 0; //pronto per iniziare una nuova chiamata
    static const int ST_CALLING = 1; //fase di immissione parametri
    static const int ST_INVOKED = 2; //funzione invocata (il risultato è disponibile)
    static const int ST_BEGIN_VECT = 3; //avvio immissione di un vettore

    //costanti errore
    static const int ERR_NOERROR=0;
    static const int ERR_BUFFER_OVERRUN=1; //non c'è spazio per aggiungere altri elementi
    static const int ERR_INVALID_STATE=2; //stato non valido per compiere l'operazione
    static const int ERR_COMM_ERROR=3; //errore di comunicazione con l'interfaccia seriale

    ISStream *stream;

    //buffer di output su cui vengono immessi la chiamata e l'elenco parametri
    SBuilder outBuffer;

    char* resultBuffer;
    size_t resultBufferLen;
    int res_start;
    int res_end;

    int last_error; //ultimo errore
   
    int state; //stato corrente (vedere costanti ST_...)

    bool firstParam; //almeno un argomento già immesso

    bool checkState(int req_state);

    bool _hasResult;

    void sendParamSep(); //invia il separatore fra parametri (,)

    void resetResultBuffer();

    void getResultValue(char **resValue,size_t *resSize);

public:

    /**
      Crea un invocatore di funzioni.
      @param outStream stream seriale di output su cui viene inviata la chiamata
      @param callBuffer un buffer di caratteri a dimensione fissa che verrà utilizzato internamente per formattare la chiamata
      @param callBuffSize dimensione del buffer callBuffer
    */
    FunctionCaller(ISStream *outStream, char *callBuffer, size_t callBufferSize);

    virtual ~FunctionCaller() {
    }

    /**
     Restituisce il messaggio dell'ultimo errore
     */
    int getLastError() const;

    /**
    Resetta lo stato e si prepara per una nuova invocazione
     */
    void resetState();

    /**
     * Svuota il buffer di input della porta seriale
     * (legge tutti i dati in attesa, usato per iniziare una nuova chiamata)
     */
    void resetInputStream();


    /**
      Inizia una chiamata di funzione.
      Dopo l'inizio di chiamata utilizzare i metodi push per immettere gli
      argomenti.
      @param fnname nome funzione
      @return true se è possibile avviare una chiamata
     */
    bool beginFunctionCall(const char* fnname);

    /**Immette un parametro intero
    @return true se tutto è ok
     */
    bool pushParamInt(int paramValue);

    /**
    Immette un parametro float
     */
    bool pushParamFloat(float floatValue);

    /**
    Immette un parametro char
     */
    bool pushParamChar(char ch);

    /**
    Immette un parametro boolean
     */
    bool pushParamBool(bool value);

    /**
    Immette un parametro stringa
     */
    bool pushParamString(const char* stringValue);

    /**
     Avvia l'immisione di un vettore
     (dopo questo metodo usare i metodi pushParam per inviare gli elementi del vettore)
     */
    bool beginVector();

    /**
    Termina l'immisione di un vettore
     */
    bool endVector();

    /**
    Esegue la chiamata inviando i dati (nome funzione+elenco argomenti) attraverso la porta seriale
     * Il risultato della chiamata viene letto con getResult
     */
    bool call();
   
    /**
     Indica se c'è un risultato
     */
    bool hasResult();

    /**
     * Acquisisce il risultato della chiamata di funzione nel buffer resultBuffer
     * @param resultBuffer il buffer su cui viene caricato il risultato
     * (il buffer resultBuffer viene resettato in ogni caso)
     * @param resultBufferSize
     * @return true se c'è un risultato nel buffer di input
     */
    bool getResult(char* resultBuffer,size_t resultBufferSize);

    /**
     * Determina se dopo la chiamata, è stata restituita una eccezione
     * @return
     */
    bool exceptionCaught();

    /**
     * Acquisisce il messaggio di eccezione
     * @param errBuffer
     * @param errBufferSize
     * @return true se si è verificata una eccezione
     */
    bool getException(char* errBuffer,size_t errBufferSize);

    /**
     Acquisisce il risultato della chiamata come un intero
     */
    int intResult();

    /**
    Acquisisce il risultato come floating point
     */
    float floatResult();

    /**
    Acquisisce il risultato come carattere
     */
    char charResult();

    /**
     Acquisisce il risultato boolean
     */
    bool boolResult();

    /**
    Acquisisce il risultato come stringa
     */
    char* stringResult(char* dest,size_t dest_buff_size);    

};

#define MAX_ARG_SIZE 20

/**
 Wrapper per la chiamata di una funzione sulla porta seriale attraverso il protocollo
 lbproto
 */
class FunctionCall {
private:

    ISStream *stream;

    char* last_error;

    int state;

    //stato interno
    static const int ST_READY = 0; //pronto per lettura nome funzione invocata
    static const int ST_READ_ARGS = 1; //fase lettura argomenti
    static const int ST_ERROR = 2;

    //verifica se lo stato è uguale a quello richiesto
    bool checkState(int req_state);
    
    //legge fino alla chiusura dell'argomento 
    bool readCloseArg();

    char tempArg[MAX_ARG_SIZE];

public:

    FunctionCall(ISStream *stream);

    virtual ~FunctionCall();
    
    /**
     Restituisce il messaggio dell'ultimo errore
     */
    char* getLastError();

    /**
    Resetta lo stato, si prepara per una nuova acquisizione
     */
    void resetState(bool emptyBuffer);

    /**
    Determina se l'oggetto è pronto per la lettura di una funzione
    @return true dopo l'inizializzazione o quando ha finito la lettura di una funzione con successo
     */
    bool isReady();

    /**
      Determina se è in stato di errore.Per resettare lo stato usare resetState
     */
    bool isError();

    /**
     Legge dalla porta seriale l'eventuale invocazione di una funzione
     (es. MY_FUNCTION(....)
     e restituisce true se sullo stream c'è l'invocazione di una funzione   
     @return true se il parsing ha successo
     */
    bool readFunctionName(char *destName, size_t maxlen);
    
    /**
    Legge l'argomento successivo
    */ 
    bool readArg(char* dest, size_t destLen);

    /**
     Legge un argomento int
     */
    bool readIntArg(int *intValue);

    /**
    Legge un argomento float
     */
    bool readFloatArg(float *floatValue);

    /**
    Legge un argomento carattere
     */
    bool readCharArg(char *chValue);

    /**
     Legge un argomento bool
     */
    bool readBoolArg(bool *bValue);

    /**
    Legge un argomento stringa
     */
    bool readStringArg(char* argValue, unsigned arg_size);

    /**
    Invia una eccezione EXCEPTION("<messaggio>")
     */
    void raiseException(const char* message);

};

#endif


