#include <SPI.h>
#include <AFMotor.h>

const byte USS_F = 33; // Front ultrasonic sensor
const byte USS_R = 34; // Right ultrasonic sensor
const byte USS_L = 22; // Left ultrasonic sensor

const byte SPEED = 150; // 59% of full power using PWM
const float SOUND_SPEED = 0.0343; // in cm/us
const int MIN_DISTANCE = 8; // min wall distance
const int MAX_DISTANCE = 10; // max wall distance

AF_DCMotor M_LF(1); // Left forward motor
AF_DCMotor M_RF(4); // Right forward motor
AF_DCMotor M_LB(2); // Left back motor
AF_DCMotor M_RB(3); // Right back motor

int wall; // 0 - start, 1 - right, 2 - left, 3 - closed end
long duration;
int fDistance, rDistance, lDistance, dTime;

void setup() {
  M_LF.setSpeed(SPEED);
  M_RF.setSpeed(SPEED);
  M_LB.setSpeed(SPEED);
  M_RB.setSpeed(SPEED);

  Serial.begin(9600);
  delay(100);
  wall = 0;
}

void loop() {

  fDistance = readUSDistance(USS_F);
  rDistance = readUSDistance(USS_R);
  lDistance = readUSDistance(USS_L);

  if (fDistance <= 15) {
    if (rDistance > 20) {
      rotate(60, 1);
      wall = 2;
    } else if (lDistance > 20) {
      rotate(90, 0);
      wall = 1;
    } else {
      wall = 3;
    }
  }

  if (wall == 2) { // Right wall
    if (lDistance < MIN_DISTANCE) {
      turn(0);
      delay(50);
      move(1, 1);
      delay(10);
    } else if (lDistance > MAX_DISTANCE) {
      turn(1);
      delay(50);
      move(1, 1);
      delay(10);
    } else {
      move(1, 1);
      delay(60);
    }
    move(0, 0);
    delay(50);
  } else if (wall == 1) { // Left wall
    if (rDistance < MIN_DISTANCE) {
      turn(1);
      delay(50);
      move(1, 1);
      delay(10);
    } else if (rDistance > MAX_DISTANCE) {
      turn(0);
      delay(50);
      move(1, 1);
      delay(10);
    } else {
      move(1, 1);
      delay(60);
    }
    move(0, 0);
    delay(50);
  } else  if (wall == 3) { // Surrounded by walls
    rotate(180, 0); // not tested
  }
  else {
    move(1, 1);
    delay(50);
  }
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
  dTime = angle * 11;
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
