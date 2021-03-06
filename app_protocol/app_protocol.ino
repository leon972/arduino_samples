/**
 Realizzazione di un semplice protocollo basato sulla comunicazione
 attraverso la porta seriale per l'invocazione di funzioni e acquisizione
 dei risultati della chiamata alla funzione
 
 Code by Leonardo Berti (c) 2012
 
 30-9-2012
 
 */
 
 #include "lbproto.h"
 #include "rs232stream.h"
 
 #define BUFF_SIZE 128
 
 static char call_buff[BUFF_SIZE];
 static char err_buff[BUFF_SIZE];
 
 //wrapper comunicazione seriale
 Rs232Stream rs232;
 //generatore di chiamate di funzione tramite interfaccia seriale
 FunctionCaller caller(&rs232,call_buff,BUFF_SIZE);
 //ricevitore e parser di chiamate di funzione
 FunctionCall callRecv(&rs232);
 
 void testCall();
 void testRecv();
 
 void outCallerError()
 {
     Serial.println("Caller error:");
     Serial.println(caller.getLastError());  
     caller.resetState();
 }
 
 void outRecvError()
 {
     if (callRecv.isError()) {
       Serial.println("Callee error:");
       Serial.println(callRecv.getLastError());    
       callRecv.resetState(true);     
     }
 }
 
 void mainMenu()
 {
      
   Serial.println("\n---------------------------------------------------");
   Serial.println(" Test LBPROTO - calling function protocol on RS232");
   Serial.println("---------------------------------------------------\n");
   Serial.println("Please choose:");
   Serial.println("1] Test function call");
   Serial.println("2] Test function callee");
   Serial.println("");
   
   while (!Serial.available())
   {
     delay(100);  
   }
   
   int ch=Serial.parseInt();
   
   switch (ch)
   {
     case 1:
        
        //test chiamata di funzione
        testCall();
        break;
               
     case 2:
     
        //test ricevimento chiamata
        testRecv();
        break;
                 
     default:
     
       Serial.println("Invalid choice!");
     
     break;
     
   }      
 }
 
 /**
 Esegue una chiamata sulla porta seriale e attende che
 l'utente immetta il risultato del tipo "[valore]"
 */
 void testCall()
 {
   
   Serial.println("\nCall a function on serial port...\n");
   
   if (!caller.beginFunctionCall("FN_TEST"))
   {
     outCallerError();
     return;
   }    
   
   if (!caller.pushParamInt(123))
   {
     outCallerError();
     return;     
   }
   
   if (!caller.pushParamFloat(111.2f))
   {
     outCallerError();
     return;        
   }
   
   if (!caller.pushParamChar('A'))
   {
     outCallerError();
     return;      
   }
   
   if (!caller.pushParamString("prova arg text"))
   {
     outCallerError();
     return;      
   }
   
   if (!caller.pushParamBool(false))
   {
     outCallerError();
     return;      
   }
   
   //invoca la funzione
   if (!caller.call())
   {
      outCallerError();
      return;           
   }    
   

   //immettere nella porta seriale usando il serial monitor il valore del risultato es. [8]
   Serial.println("\nWAIT RESULT...\n");

   while (!rs232.dataAvailable())
   {
      delay(100);
   }   
   
   if (caller.getResult(call_buff,BUFF_SIZE))
   {
     
     if (caller.exceptionCaught())
     {
       
       Serial.println("\nException caught!\n");       
       caller.getException(err_buff,BUFF_SIZE);
       Serial.println("error message=");
       Serial.println(err_buff);
       
     }
     else {
     
       Serial.println("OK RESULT!\n");
       Serial.println(call_buff);  
       
       Serial.print("int result=");
       Serial.println(caller.intResult());
       
       Serial.print("float result=");
       Serial.println(caller.floatResult());
       
       Serial.print("char result=");
       Serial.println(caller.charResult());
       
       Serial.print("bool result=");
       Serial.println(caller.boolResult());
       
       Serial.print("string result=");
       Serial.println(caller.stringResult(err_buff,BUFF_SIZE));
       
     }
   }   
   else
   {
     Serial.println("No result!");  
   }
   
 } 
  
 void testRecv()
 {
   const size_t sz=64;
   char buff[sz];
   
   Serial.println("Receive a call on serial port...");
   Serial.println("Write a function call es. FN(8,8)");
      
   while (!rs232.dataAvailable()) 
   {
     delay(100);  
   }
   
   if (callRecv.readFunctionName(buff,sz))
   {     
       int a1=0,a2=0;     
       
       while (!rs232.dataAvailable())
       {
         delay(20);
       }
       
       if (!callRecv.readIntArg(&a1))
       {
         outRecvError(); 
         return;         
       }
       
        while (!rs232.dataAvailable())
       {
         delay(20);
       }
       
       if (!callRecv.readIntArg(&a2))
       {
         outRecvError();            
         return;
       }      

      Serial.println("function called:");
      Serial.println(buff);

      Serial.print("a1=");
      Serial.println(a1);
      
      Serial.print("a2=");
      Serial.println(a2);
      
   }
   else
   {
     outRecvError();    
   }
   
 } 
  
 void setup()
 {
 
   Serial.begin(9600);
     
   Serial.println("TEST LB SERIAL APP PROTOCOL");
        
 }
  
 void loop()
 {

   mainMenu();
   
   //testRecv();
   delay(100);       
 }
 
