#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // SIM800L Tx & Rx connected to Arduino pins 2 & 3

int irSensorPin1 = 8;
int irSensorPin2 = 7;
int irSensorPin3 = 9;
int ledRed = 13;
int ledGreen = 12;
int vs = 10; // Vibration sensor pin
int ledPin1 = 6;
int ledPin2 = 5;
int ledPin3 = 11;
int irSensorValue1, irSensorValue2, irSensorValue3;
int brightness;

void setup() {
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(vs, INPUT);
  Serial.begin(9600);
  pinMode(irSensorPin1, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(irSensorPin2, INPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(irSensorPin3, INPUT);
  pinMode(ledPin3, OUTPUT);

  mySerial.begin(9600);
}

void loop() {
  long measurement = vibration();
  delay(50);
  Serial.println(measurement);

  if (measurement > 20000) {
    digitalWrite(ledRed, HIGH);
    sendSMS("High Vibration Detected!");
  } else if (measurement > 1000 && measurement <= 10000) {
    digitalWrite(ledGreen, HIGH);
  } else {
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, LOW);
  }

  // IR Sensor-Based Lighting Control
  controlLED(irSensorPin1, ledPin1);
  controlLED(irSensorPin2, ledPin2);
  controlLED(irSensorPin3, ledPin3);
}

void controlLED(int sensorPin, int ledPin) {
  int sensorValue = digitalRead(sensorPin);
  int brightness = (sensorValue == HIGH) ? 15 : 255;
  analogWrite(ledPin, brightness);
}

long vibration() {
  return pulseIn(vs, HIGH);
}

void sendSMS(String message) {
  mySerial.println("AT+CMGF=1");
  delay(500);
  mySerial.println("AT+CMGS="+918147200157""); // Change with recipient's number
  delay(500);
  mySerial.print(message);
  delay(500);
  mySerial.write(26);
}
