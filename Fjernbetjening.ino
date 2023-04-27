/*
  Knappernes funktion:
  Knapperne slår en LED til og fra afhængig af, om der er trykket ned eller ej. Der er knapper fra digital port 9 til 13, mens LED'erne er fra digital port 2 til 4.
  Desuden bruges der også et joystick, der bruger digital port 8 samt port A0 og A1 dvs. analog port 0 og 1.
  
  Kredsløbet fra port 13:
    - Knap på port 13 får tilsendt +5 V, hvoraf den går til ground gennem en 10 kiloohm resistor. Der skal bruges en resistor,
      så der ikke bliver fanget et tilfældigt, forkert signal
    - Signalet fra knappen sender et signal til port 2.
    - LED på port 2 går til ground gennem en 220 ohm resistor.
    
  Stort set det samme vil ske fra de andre porte.

  Lavet i 2023.
  Lavet af Sebastian Blichfeldt.
  Klasse: 2.yz.
  */
const int buttonPause = 12;  // Giver navn til en knap-portene.
const int buttonPlay = 11;
const int buttonVolUp = 9;
const int buttonVolDown = 13;
const int buttonHome = 10;
int buttonSelect = 8;

#define VRX_PIN  A0 // Arduino pin forbundet til VRX pin - altså en pin, der følger x-aksen.
#define VRY_PIN  A1 // Arduino pin forbundet til VRY pin - altså en pin, der følger y-aksen.

#define LEFT_THRESHOLD  400 // Venstre-grænse sættes ved 400.
#define RIGHT_THRESHOLD 800 // Højre-grænse sættes ved 800.
#define UP_THRESHOLD    400 // Opad-grænse sættes ved 400.
#define DOWN_THRESHOLD  800 // Nedad-grænse sættes ved 800.

#define COMMAND_NO     0x00 // Kommandoerne får et hexadecimal, da de er kompakte og kan udvise op til 255 i værdi.
#define COMMAND_LEFT   0x01
#define COMMAND_RIGHT  0x02
#define COMMAND_UP     0x04
#define COMMAND_DOWN   0x08

int xValue = 0 ; // Til at gemme x-aksens værdi.
int yValue = 0 ; // Til at gemme y-aksens værdi.
int command = COMMAND_NO; // command og COMMAND_NO betyder det samme.
const int ledRed = 2; // Navne tildeles portene.
const int ledYellowDown = 3;
const int ledYellowUp = 5;
const int ledGreen = 4;

int buttonStatePause = 0;  // Variabel der tjekker knappernes status.
int buttonStatePlay = 0;
int buttonStateVolUp = 0;
int buttonStateVolDown = 0;
int buttonStateHome = 0;
int buttonStateSelect = 0;

void setup() {
  Serial.begin(9600) ;
  pinMode(ledRed, OUTPUT); // Fortæller at LED'erne skal give output (f.eks. lys).
  pinMode(ledYellowUp, OUTPUT);
  pinMode(ledYellowDown, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(buttonPause, INPUT); // Fortæller at knapperne skal give input (f.eks. sende signal).
  pinMode(buttonPlay, INPUT);
  pinMode(buttonVolUp, INPUT);
  pinMode(buttonVolDown, INPUT);
  pinMode(buttonHome, INPUT);
  pinMode(buttonSelect, INPUT_PULLUP);

  for (int count = 0; count < 3; count++) { // Her sættes lamperne til at tænde og slukke i rækkefølge 3 gange.
    digitalWrite(ledYellowUp, LOW);
    digitalWrite(ledRed, HIGH);
    delay(250);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellowDown, HIGH);
    delay(250);
    digitalWrite(ledYellowDown, LOW);
    digitalWrite(ledGreen, HIGH);
    delay(250);
    digitalWrite(ledYellowUp, HIGH);
    digitalWrite(ledGreen, LOW);
    delay(250);
  }
}

void loop() {
  // Tjekker status på knapperne:
  buttonStatePause = digitalRead(buttonPause);
  buttonStatePlay = digitalRead(buttonPlay);
  buttonStateVolUp = digitalRead(buttonVolUp);
  buttonStateVolDown = digitalRead(buttonVolDown);
  buttonStateHome = digitalRead(buttonHome);
  buttonStateSelect = digitalRead(buttonSelect);
  // Aflæser analog x- og y-værdier.
  xValue = analogRead(VRX_PIN); // x-værdien aflæses gennem pin VRX (0-1023).
  yValue = analogRead(VRY_PIN); // y-værdien aflæses gennem pin VRY (0-1023).

  // Analog-værdierne konverteres til kommandoer.
  command = COMMAND_NO;

  // Tjekker for venstre/højre-kommandoer
  if (xValue < LEFT_THRESHOLD) // Hvis x er mindre end venstre-grænsen, så går den til venstre.
    command = command | COMMAND_LEFT;
  else if (xValue > RIGHT_THRESHOLD) // Hvis x er større end venstre-grænsne, så går den til højre.
    command = command | COMMAND_RIGHT;

  // Tjekker for op/ned-kommandoer.
  if (yValue < UP_THRESHOLD)
    command = command | COMMAND_UP; // Hvis y er mindre end opad-grænsen, så går den op.
  else if (yValue > DOWN_THRESHOLD)
    command = command | COMMAND_DOWN; // Hvis y er større end opad-grænsen, så går den ned.

  // Tjekker om knappen "pause" er trykket ned eller om joystick rykkes mod højre. Hvis det er tilfældet, så udfører den følgende:
  if (buttonStatePause == HIGH) {
    // Tænd LED:
    digitalWrite(ledRed, HIGH);
  } else if (command & COMMAND_RIGHT){
    Serial.println("COMMAND RIGHT");
    digitalWrite(ledRed, HIGH);
  } else if (buttonStateVolDown == HIGH){
    digitalWrite(ledRed, HIGH);
  } else {
    // Sluk LED:
    digitalWrite(ledRed, LOW);
     }
  // Tjekker om knappen "play" er trykket ned eller om joystick rykkes mod venstre. Hvis det er tilfældet, så udfører den følgende:
  if (buttonStatePlay == HIGH){
    digitalWrite(ledGreen, HIGH);
  } else if (command & COMMAND_LEFT) {
    Serial.println("COMMAND LEFT");
    digitalWrite(ledGreen, HIGH);
  } else if (buttonStateVolUp == HIGH){
    digitalWrite(ledGreen, HIGH);
  } else {
    digitalWrite(ledGreen, LOW);
    }
  // Tjekker om knappen "home" er trykket ned eller om joystick rykkes nedad. Hvis det er tilfældet, så udfører den følgende:
  if (buttonStateHome == HIGH){
    digitalWrite(ledYellowDown, HIGH);
  } else if (command & COMMAND_DOWN){
    digitalWrite(ledYellowDown, HIGH);
  } else {
    digitalWrite(ledYellowDown, LOW);
  }
  // Tjekker om joystick rykkes opad. Hvis det er tilfældet, så udfører den følgende:
   if (command & COMMAND_UP) {
    Serial.println("COMMAND UP");
    digitalWrite(ledYellowUp, HIGH);
  } else {
    digitalWrite(ledYellowUp, LOW);
  }
  // Tjekker om joystick-knappen er trykket ned. Hvis det er tilfældet, så udfører den følgende:
  if (buttonStateSelect == LOW){ // Som den eneste skal den have status LOW - altså 0, da den fungerer omvendt. Den giver altså value = 1 indtil man trykker.
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledYellowUp, HIGH);
    digitalWrite(ledYellowDown, HIGH);
  }
}
