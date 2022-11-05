#include <SPI.h>
#include <AFMotor.h>

const byte LEFT_SENSOR = 43;
const byte RIGHT_SENSOR = 47;
const byte USS_F = 33; // Front ultrasonic sensor
const byte USS_R = 34; // Right ultrasonic sensor

const byte SPEED = 150; // 59% of full power using PWM
const float SOUND_SPEED = 0.0343; // in cm/us

AF_DCMotor M_LF(1); // Left forward motor
AF_DCMotor M_RF(4); // Right forward motor
AF_DCMotor M_LB(2); // Left back motor
AF_DCMotor M_RB(3); // Right back motor

boolean lSen, rSen; // left and right sensors
long duration;
int fDistance, rDistance, dTime;

void setup() {
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
  M_LF.setSpeed(SPEED);
  M_RF.setSpeed(SPEED);
  M_LB.setSpeed(SPEED);
  M_RB.setSpeed(SPEED);

  Serial.begin(9600);
  delay(100);
}

void loop() {
  fDistance = readUSDistance(USS_F);
  if (fDistance <= 15) { // avoiding an obsticle routine
    rotate(60, 0); // rotate 60 degrees counterclockwise
    move(1, 1); // go forward for 0.4s
    delay(400);
    rDistance = readUSDistance(USS_R);
    if (rDistance > 12) {
      rotate(45, 1);
    }
    lSen = digitalRead(LEFT_SENSOR);
    while (!lSen) {
      rDistance = readUSDistance(USS_R);
      if (rDistance < 13) {
        turn(1);
        delay(50);
        move(1, 1);
        delay(10); // this is used to make the turn wider
      } else if (rDistance > 15) {
        turn(0);
        delay(50);
        move(1, 1);
        delay(10);
      } else {
        move(1, 1);
        delay(60);
      }
      move(0, 0); // stop
      delay(40);
      lSen = digitalRead(LEFT_SENSOR);
    }
  }
  
  lSen = digitalRead(LEFT_SENSOR);
  rSen = digitalRead(RIGHT_SENSOR);
  if (!lSen && !rSen) { // Forward
    move(1, 1);
    delay(30);
  } else if (!lSen && rSen) { // Right
    move(1, 0);
    delay(200);
  } else if (lSen && !rSen) { // Left
    move(0, 1);
    delay(200);
  }
  move(0, 0);
  delay(30);
}

void move(boolean left, boolean right) {
  if (!left && !right) {
    M_LF.run(RELEASE);
    M_RF.run(RELEASE);
    M_LB.run(RELEASE);
    M_RB.run(RELEASE);
  } else {
    M_LF.run(!left ? FORWARD : BACKWARD); // ! is used due to switched - and + connection
    M_LB.run(!left ? FORWARD : BACKWARD);
    M_RF.run(right ? FORWARD : BACKWARD);
    M_RB.run(right ? FORWARD : BACKWARD);
  }
}

void turn(boolean right) {
  if (right) {
    M_RF.run(FORWARD);
    M_RB.run(FORWARD);
  } else {
    M_LF.run(BACKWARD); // BACKWARD is used due to switched - and + connection
    M_LB.run(BACKWARD);
  }
}

void rotate(int angle, boolean clockwise) {
  move(clockwise, !clockwise);
  dTime = angle * 9;
  delay(dTime);
  move(0, 0);
}

int readUSDistance(byte pin) {
  byte trig = pin, echo = pin;
  pinMode(trig, OUTPUT);
  digitalWrite(trig, 0);
  delayMicroseconds(2);
  digitalWrite(trig, 1);
  delayMicroseconds(10);
  digitalWrite(trig, 0);

  pinMode(echo, INPUT);
  duration = pulseIn(echo, HIGH);
  return SOUND_SPEED / 2 * duration;
}
