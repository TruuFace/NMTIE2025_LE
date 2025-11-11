// === Ultrasonic Distance Sensor (Arduino UNO R3) ===
// LEDs: D10–D13  |  Buzzer: D5  |  HC-SR04: Trig=A0, Echo=A1

// === Pin assignments ===
const int trigPin = A0;
const int echoPin = A1;
const int led1 = 10;  // red – farthest
const int led2 = 11;  // orange
const int led3 = 12;  // purple
const int led4 = 13;  // blue – closest
const int buzzer = 5;

// === Distance thresholds (cm) ===
const int RANGE1 = 15;
const int RANGE2 = 10;
const int RANGE3 = 5;

// === Timing for serial output ===
unsigned long lastPrint = 0;
const unsigned long printDelay = 5000;  // every 5 s

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(buzzer, OUTPUT);
  noTone(buzzer);

  Serial.println("Ultrasonic distance demo ready!");
}

long readDistance() {
  // Trigger a measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure echo time
  long duration = pulseIn(echoPin, HIGH, 30000);  // 30 ms timeout (~5 m)
  if (duration == 0) return -1;                   // no echo
  long cm = duration * 0.0343 / 2;
  return cm;
}

void loop() {
  long dist = readDistance();

  // print distance every 5 s
  if (millis() - lastPrint >= printDelay) {
    Serial.print("Distance: ");
    if (dist >= 0) {
      Serial.print(dist);
      Serial.println(" cm");
    } else {
      Serial.println("Out of range");
    }
    lastPrint = millis();
  }

  // reset LEDs and buzzer
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  noTone(buzzer);

  // distance-based logic
  if (dist < 0) {
    // out of range → all off
  }
  else if (dist > RANGE1) {
    digitalWrite(led1, HIGH);
  }
  else if (dist > RANGE2) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
  }
  else if (dist > RANGE3) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    tone(buzzer, 900, 100);   // short beep
  }
  else {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    tone(buzzer, 1500);       // continuous tone
  }

  delay(150);
}
