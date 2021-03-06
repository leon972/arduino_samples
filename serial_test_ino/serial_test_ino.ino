/**
  Test porta seriale con Arduino
  Code by Leonardo Berti (c) 2012
  20-9-2012
*/  

//pin a cui è collegato il led
const int LED_PIN=13; 

void setup() {                

  // initialize the digital pin as an output.
  pinMode(LED_PIN, OUTPUT);    
  
  Serial.begin(9600);    
    
}

void writeHello()
{
    Serial.println("Hello from Arduino");  
}

/**
 Ciclo infinito
*/ 
void loop()
{
  
   delay(2500);
   writeHello();
  
  
}


