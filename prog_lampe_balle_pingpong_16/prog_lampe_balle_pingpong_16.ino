const int redLEDPin = 10;
const int greenLEDPin = 9;
const int blueLEDPin = 11;
const int buttonPin = 2;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

int targetRed = 0;
int targetGreen = 0;
int targetBlue = 0;

unsigned long derniertemps = 0;
unsigned long dernierchgmt = 0;

bool ledActive = true;      // état ON/OFF
bool lastButtonState = HIGH;
int lastChoice = -1;        // dernière couleur choisie

// --- Choisir une nouvelle couleur pure différente de la précédente ---
void nouvelleCouleur() {
  int choix;
  do {
    choix = random(3);  // 0 = rouge, 1 = vert, 2 = bleu
  } while (choix == lastChoice); // éviter la même couleur

  lastChoice = choix;

  if (choix == 0) {       // Rouge pur
    targetRed = 255; targetGreen = 0; targetBlue = 0;
  } 
  else if (choix == 1) {  // Vert pur
    targetRed = 0; targetGreen = 255; targetBlue = 0;
  } 
  else {                  // Bleu pur
    targetRed = 0; targetGreen = 0; targetBlue = 255;
  }
}

void setup() {
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // bouton avec résistance interne

  randomSeed(analogRead(A0)); 
  nouvelleCouleur(); // première couleur
}

void loop() {
  unsigned long maintenant = millis();

  // --- Gestion du bouton ---
  bool buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && lastButtonState == HIGH) { // détection d'appui
    ledActive = !ledActive; // on inverse l'état
    if (!ledActive) { // si OFF → éteindre tout
      analogWrite(redLEDPin, 0);
      analogWrite(greenLEDPin, 0);
      analogWrite(blueLEDPin, 0);
    }
    delay(50); // anti-rebond simple
  }
  lastButtonState = buttonState;

  if (ledActive) {
    // interpolation douce toutes les 10 ms
    if (maintenant - derniertemps > 10) {
      derniertemps = maintenant;

      if (redValue < targetRed) redValue++;
      else if (redValue > targetRed) redValue--;

      if (greenValue < targetGreen) greenValue++;
      else if (greenValue > targetGreen) greenValue--;

      if (blueValue < targetBlue) blueValue++;
      else if (blueValue > targetBlue) blueValue--;

      analogWrite(redLEDPin, redValue);
      analogWrite(greenLEDPin, greenValue);
      analogWrite(blueLEDPin, blueValue);
    }

    // toutes les 2 secondes → nouvelle couleur pure
    if (maintenant - dernierchgmt > 2000) {
      dernierchgmt = maintenant;
      nouvelleCouleur();
    }
  }
}
