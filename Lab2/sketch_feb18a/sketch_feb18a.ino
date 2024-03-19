const int buttonPin1 = 46;
const int buttonPin2 = 44;
const int ledCount = 8;
int ledPins[] = {22,23,24,25,26,27,28,29};

void algo_3() {
  for (int i = 0; i < ledCount / 2; i++) {
    digitalWrite(ledPins[i], HIGH);
    digitalWrite(ledPins[ledCount - 1 - i], HIGH);
    delay(900);
    digitalWrite(ledPins[i], LOW);
    digitalWrite(ledPins[ledCount - 1 - i], LOW);
  }
}

void algo_6() {
  for (int i = ledCount - 1; i >= 0; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(900);
    digitalWrite(ledPins[i], LOW);
  }
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < ledCount; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin2, INPUT_PULLUP);
}

void loop() {
  if (Serial.available()) {
    int inByte = Serial.read();
    if (inByte == 0xA1) {
      algo_3();
    } else if (inByte == 0xB1) {
      algo_6();
    }
  }
  if (digitalRead(buttonPin1) == LOW) {
    Serial.write(0xA1);
    delay(200);
  }
  if (digitalRead(buttonPin2) == LOW) {
    Serial.write(0xB1);
    delay(200);
  }
}
