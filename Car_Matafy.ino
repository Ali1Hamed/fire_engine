#include <Wire.h>
#include <Servo.h>
#include <HUSKYLENS.h>

HUSKYLENS huskylens;
Servo servoMotor;
int servoPin = 7;
int servoPos = 90;
int servoStep = 2;
int servoSpeed = 10;
int servoMin = 20;
int servoMax = 160;

int leftMotorPin1 = 10;
int leftMotorPin2 = 12;
int leftMotorSpeedPin = 4;
int rightMotorPin1 = 11;
int rightMotorPin2 = 13;
int rightMotorSpeedPin = 3;
int motorSpeed = 100;
int maxMotorSpeed = 255;
int minMotorSpeed = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  servoMotor.attach(servoPin);
  servoMotor.write(servoPos);
  huskylens.begin(Wire);
  huskylens.writeAlgorithm(ALGORITHM_OBJECT_TRACKING);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(leftMotorSpeedPin, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(rightMotorSpeedPin, OUTPUT);
}

void loop() {
  if (huskylens.request()) {
    if (huskylens.available()) {
      HUSKYLENSResult result = huskylens.read();
      int motorError = result.xCenter - 160;
      servoPos += motorError / servoStep;
      if (servoPos < servoMin) {
        servoPos = servoMin;
      }
      if (servoPos > servoMax) {
        servoPos = servoMax;
      }
      servoMotor.write(servoPos);
      if (result.width > 60) {
        int motorSpeedLeft = motorSpeed;
        int motorSpeedRight = motorSpeed;
        if (result.xCenter < 110) {
          motorSpeedRight = 0;
          digitalWrite(rightMotorPin1, LOW);
          digitalWrite(rightMotorPin2, LOW);
        } else if (result.xCenter > 210) {
          motorSpeedLeft = 0;
          digitalWrite(leftMotorPin1, LOW);
          digitalWrite(leftMotorPin2, LOW);
        }
        analogWrite(leftMotorSpeedPin, motorSpeedLeft);
        analogWrite(rightMotorSpeedPin, motorSpeedRight);
        digitalWrite(leftMotorPin1, HIGH);
        digitalWrite(leftMotorPin2, LOW);
        digitalWrite(rightMotorPin1, HIGH);
        digitalWrite(rightMotorPin2, LOW);
      } else {
        digitalWrite(leftMotorPin1, LOW);
        digitalWrite(leftMotorPin2, LOW);
        digitalWrite(rightMotorPin1, LOW);
        digitalWrite(rightMotorPin2, LOW);
      }
    } else {
      digitalWrite(leftMotorPin1, LOW);
      digitalWrite(leftMotorPin2, LOW);
      digitalWrite(rightMotorPin1, LOW);
      digitalWrite(rightMotorPin2, LOW);
    }
  }
}