/***
 Questo programma mostra l'utilizzo di un push button
 (normalmente aperto).
 Quando il push button è premuto, il pin 13 risulta in stato alto.
 L'informazione viene inviata alla porta seriale del PC.

 Hardware:

 Il pin che fornisce 5V è collegato ad un push button,
 a valle del push button è collegata una resistenza di pull-down da 1 KOhm
 e la resistenza è in serie ad un diodo indicatore collegato fra la resistenza e massa.
 Il pin 13 è collegato subito a valle del push button.
 Al pin 12 è collegato tramite una resistenza da 330 ohm un led che viene
 accesso quando si preme il pulsante

 Code by Leonardo Berti (c) 2012

 */

//pin in modalità input collegato al pulsante
#define BUTTON_PIN 13

//pin in modalità output collegato al led di segnalazione
#define OUT_PIN 12

/**pin in modalità input che intercetta lo stato del
pulsante
*/
int button_pin;
/**pin output collegato al led di segnalazione pulsante premuto*/
int out_pin = 12;

boolean initV = true;

/**
 Inizializza il push button
 @param pin pin al quale è collegato il pulsante
*/
void setupButton(int pin)
{
  button_pin = pin;
  pinMode(pin, INPUT);

}

/**
 Determina se il pulsante è premuto
 Rende true se è premuto
*/
bool buttonPressed()
{
  return digitalRead(button_pin);
  //nota:sembra che il debouncing non serva

}

void setup()
{

  //avvia la comunicazione seriale
  Serial.begin(9600);
  //collega il pulsante normalmente aperto al pin 13
  setupButton(BUTTON_PIN);

  pinMode(OUT_PIN, OUTPUT);

}

void loop()
{
  delay(30);


  if (buttonPressed())
  {
   

      Serial.println("OK");
      initV = false;
    
  }
   else {
   // initV=true;
   }
  


  //accende il led di controllo quando il tasto è premuto
  digitalWrite(OUT_PIN, buttonPressed());

}


