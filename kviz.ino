#include <LiquidCrystal.h>

// Definicija pinov
const int buttonA = 8;
const int buttonB = 9;
const int buttonC = 10;
const int redLED = 11;
const int yellowLED = 12;
const int greenLED = 13;

// LCD povezave
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Spremenljivke za stanje
int pressedOrder[3] = {0, 0, 0};
int pressCount = 0;
bool quizComplete = false;
bool buttonsEnabled = true;

void setup() {
  // Inicializacija pinov
  pinMode(buttonA, INPUT);
  pinMode(buttonB, INPUT);
  pinMode(buttonC, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  
  // Zagotovimo, da so LED izklopljene na začetku
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, HIGH);
  
  // Inicializacija LCD
  lcd.begin(16, 2);
  
  // Začetni prikaz na LCD
  lcd.print("Kviz - pritisni");
  lcd.setCursor(0, 1);
  lcd.print("A, B ali C");
  
  // Počakamo, da se vse stabilizira
  delay(100);
}

void loop() {
  if (!quizComplete) {
    if (buttonsEnabled) {
      checkButtons();
    }
    
    // Posodobi LED indikatorje
    if (pressCount == 0) {
      digitalWrite(greenLED, HIGH);
      digitalWrite(yellowLED, LOW);
      digitalWrite(redLED, LOW);
    } else if (pressCount < 3) {
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, HIGH);
      digitalWrite(redLED, LOW);
    } else {
      // Vsi trije so pritisnili
      quizComplete = true;
      digitalWrite(yellowLED, LOW);
      digitalWrite(greenLED, LOW);
      showResults();
    }
  } else {
    // Utripanje rdeče LED
    digitalWrite(redLED, !digitalRead(redLED));
    delay(500);
  }
}

void checkButtons() {
  // Preveri tipko A
  if (digitalRead(buttonA) == HIGH && !isPressed(1)) {
    delay(50); // Debounce
    if (digitalRead(buttonA) == HIGH) {
      registerPress(1);
    }
  }
  
  // Preveri tipko B
  if (digitalRead(buttonB) == HIGH && !isPressed(2)) {
    delay(50); // Debounce
    if (digitalRead(buttonB) == HIGH) {
      registerPress(2);
    }
  }
  
  // Preveri tipko C
  if (digitalRead(buttonC) == HIGH && !isPressed(3)) {
    delay(50); // Debounce
    if (digitalRead(buttonC) == HIGH) {
      registerPress(3);
    }
  }
}

bool isPressed(int button) {
  for (int i = 0; i < 3; i++) {
    if (pressedOrder[i] == button) {
      return true;
    }
  }
  return false;
}

void registerPress(int button) {
  if (pressCount < 3) {
    pressedOrder[pressCount] = button;
    pressCount++;
    
    // Posodobi LCD
    lcd.clear();
    lcd.print("Pritisnjeno:");
    lcd.setCursor(0, 1);
    for (int i = 0; i < pressCount; i++) {
      switch (pressedOrder[i]) {
        case 1: lcd.print("A"); break;
        case 2: lcd.print("B"); break;
        case 3: lcd.print("C"); break;
      }
      if (i < pressCount - 1) lcd.print(", ");
    }
  }
  
  if (pressCount >= 3) {
    buttonsEnabled = false;
  }
}

void showResults() {
  lcd.clear();
  lcd.print("Vrstni red:");
  lcd.setCursor(0, 1);
  
  for (int i = 0; i < 3; i++) {
    switch (pressedOrder[i]) {
      case 1: lcd.print("A"); break;
      case 2: lcd.print("B"); break;
      case 3: lcd.print("C"); break;
    }
    
    lcd.print("(");
    lcd.print(i+1);
    lcd.print(")");
    
    if (i < 2) lcd.print(" ");
  }
}