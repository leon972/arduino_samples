/**
  Test features linguaggio Wiring per Arduino
*/ 

const char sep[]="----------------------------";

void setup()
{
  Serial.begin(9600); 
}

/**
  Testa l'output sulla porta seriale
*/
void testPrint()
{
  
  Serial.println(sep);
  Serial.println("Test language features");  
  Serial.println(sep);

  
  for (int i=0;i<10;i++) {

    Serial.println(i);  
  
  }
  
  int a=9;
  
  Serial.println(a/2);  
  
  Serial.println(sep);
  Serial.println("Fine test");  
  Serial.println(sep);
      
}

/**
 Legge caratteri dalla porta seriale
*/ 
void testSerialInput()
{
  int data=0x0;
  
  Serial.println("Inserire dei dati, q per terminare");
  
  while (data!=113)
  {
    if (Serial.available())
    {
      data=Serial.read();
      Serial.print("ricevuto :");      
      char ch=data;      
      Serial.print(ch);    
      Serial.print(" (");
      Serial.print(data);
      Serial.print(")");                 
      
      Serial.println(ch);
      
    }
    
  }
  
  Serial.println("Test input terminato!");
  
}

/**
  Acquisizione di un numero
*/  
void testSerialInput2()
{
  Serial.println(sep);
  Serial.println("Test input 2:immettere un numero.");
  Serial.println(sep);
  
  int a=0;
  
  do {
  
    a=Serial.parseInt();
  
  } while(!a);

  Serial.print("Ricevuto ");
  Serial.println(a); 
  
}

/**
  Acquisizione di una stringa
*/  
void testReadString()
{
 
  const int MAXLEN=10; 
  
  Serial.println(sep);
  Serial.print("Immettere una stringa (MAX ");
  Serial.print(MAXLEN);
  Serial.println(" caratteri)");  
  
  char a[MAXLEN];
  
  for (int i=0;i<MAXLEN;i++)
  {
    a[i]=0x0;  
  }
  
  a[MAXLEN-1]=0x0;
  
  Serial.println("Valore iniziale:");
  Serial.println(a);
  
  int cnt=0;
  
  while (!Serial.available())
  {    
  }

  while (Serial.available() && cnt<MAXLEN-1)
  {
    a[cnt++]=Serial.read();
    delay(50); //attende un po' perché dopo il primo byte risulta available()=false
    
  }  
  
  Serial.print("Ricevuta la stringa:");
  Serial.println(a);
  
  
};

void loop()
{
    testPrint();
    
    delay(3000); 
    
    testReadString();        
}
