#include <Servo.h>

// Ultrasonic sensor pins 6/4/26
const int TRIG_PIN = A0;
const int ECHO_PIN = A1;

// Servo
Servo scanServo;
const int SERVO_PIN = 10;

// Motor driver pins
const int ENA = 5, IN1 = 2, IN2 = 3; // Left motor
const int ENB = 6, IN3 = 7, IN4 = 8; // Right motor

// Tuning parameters
const int SAFE_DIST = 20;     // cm - stop if obstacle closer than this
const int MOTOR_SPEED = 180;  // 0-255 PWM
const int TURN_TIME = 500;    // ms for each turn

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  
  scanServo.attach(SERVO_PIN);
  scanServo.write(90);  // Face forward
  delay(500);
  
  Serial.begin(9600);
  Serial.println("Obstacle Avoiding Robot Ready");
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  if (duration == 0) return 400; // No echo = far away
  return duration * 0.034 / 2;
}

void motorForward() {
  analogWrite(ENA, MOTOR_SPEED); analogWrite(ENB, MOTOR_SPEED);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void motorStop() {
  analogWrite(ENA, 0); analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void motorTurnLeft() {
  analogWrite(ENA, MOTOR_SPEED); analogWrite(ENB, MOTOR_SPEED);
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); // Left backward
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);  // Right forward
}

void motorTurnRight() {
  analogWrite(ENA, MOTOR_SPEED); analogWrite(ENB, MOTOR_SPEED);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  // Left forward
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); // Right backward
}

void motorReverse() {
  analogWrite(ENA, MOTOR_SPEED); analogWrite(ENB, MOTOR_SPEED);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void loop() {
  long distForward = getDistance();
  Serial.print("Forward: "); Serial.print(distForward); Serial.println("cm");

  if (distForward > SAFE_DIST) {
    motorForward();
  } else {
    // Obstacle detected - stop and scan
    motorStop();
    delay(200);

    // Scan right
    scanServo.write(20);
    delay(400);
    long distRight = getDistance();

    // Scan left
    scanServo.write(160);
    delay(400);
    long distLeft = getDistance();

    // Return to centre
    scanServo.write(90);
    delay(300);

    Serial.print("Right: "); Serial.print(distRight);
    Serial.print(" Left: "); Serial.println(distLeft);

    // Decide direction
    if (distRight < 15 && distLeft < 15) {
      // Trapped - reverse and spin
      motorReverse();
      delay(500);
      motorTurnRight();
      delay(800);
    } else if (distRight >= distLeft) {
      motorTurnRight();
      delay(TURN_TIME);
    } else {
      motorTurnLeft();
      delay(TURN_TIME);
    }
    motorStop();
    delay(100);
  }
}