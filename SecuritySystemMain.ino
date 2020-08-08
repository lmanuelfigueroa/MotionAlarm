#include <Keypad.h>
const byte ROWS = 4;
const byte COLS = 4;
int calibrationTime = 30;
bool alarmOn = true;
int pirPin = 12;
int ledPin = 13;
int buzzerPin = 10;


char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
String adminPassword = "123*";
String userPassword = "";
String hiddenPassword = "*";
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.println("Calibrating Sensor");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    delay(50);
  }

  Serial.println("Done");
  Serial.println("Sensor Activated");
  delay(50);
}

void loop() {

  if (digitalRead(pirPin) == HIGH) {
    alarmOn=true;
    Serial.println("ALARM ACTIVATED");
    digitalWrite(ledPin, HIGH);
    alarmActivated();
  }

}



void stopProgram() {
  while (true) {}
}
void alarmActivated() {
  tone(buzzerPin,1000);
  while (alarmOn) {
    char customKey = customKeypad.getKey();
    if (customKey) {
      userPassword += customKey;
      Serial.print(hiddenPassword);
      if (userPassword.length() == adminPassword.length()) {
        if (validatePassword(userPassword)) {
          Serial.println("\nCorrect Password");
          digitalWrite(ledPin, LOW);
          alarmOn = false;
          userPassword = "";
          noTone(buzzerPin);
          Serial.println("ALARM OFF");
        }

        else {
          Serial.println("\nIncorrect Password");
          userPassword = "";
          hiddenPassword = '*';
        }

      }
    }
  }
}

bool validatePassword(String password) {
  bool passwordIsvalid = false;
  if (password == adminPassword) {
    passwordIsvalid = true;
  }
  return passwordIsvalid;
}
