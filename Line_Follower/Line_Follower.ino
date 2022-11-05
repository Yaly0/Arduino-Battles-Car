#include <SPI.h>
#include <AFMotor.h>

const byte LEFT_SENSOR = 43;
const byte RIGHT_SENSOR = 47;
const byte INTERR = 18; // button between GND and Pin 18

const byte SPEED = 150; // 59% of full power using PWM
const float SOUND_SPEED = 0.0343; // in cm/us

AF_DCMotor M_LF(1); // Left forward motor
AF_DCMotor M_RF(4); // Right forward motor
AF_DCMotor M_LB(2); // Left back motor
AF_DCMotor M_RB(3); // Right back motor


boolean lSen, rSen; // left and right sensors
volatile boolean pokret;

void setup() {
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
  M_LF.setSpeed(SPEED);
  M_RF.setSpeed(SPEED);
  M_LB.setSpeed(SPEED);
  M_RB.setSpeed(SPEED);

  pinMode(INTERR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERR), kreni, RISING);
  Serial.begin(9600);
  delay(100);
}

void loop() {
  if (pokret) {
    lSen = digitalRead(LEFT_SENSOR);
    rSen = digitalRead(RIGHT_SENSOR);
    if (!lSen && !rSen) { // Forward
      move(1, 1);
      delay(45);
    } else if (!lSen && rSen) { // Right
      move(1, 0);
      delay(40);
    } else if (lSen && !rSen) { // Left
      move(0, 1);
      delay(40);
    }
    move(0, 0);
    delay(100);
  } else {
    move(0, 0);
  }
}

void move(boolean left, boolean right) {
  if (!left && !right) {
    M_LF.run(RELEASE);
    M_RF.run(RELEASE);
    M_LB.run(RELEASE);
    M_RB.run(RELEASE);
  } else {
    M_LF.run(!left ? FORWARD : BACKWARD);  // ! is used due to switched - and + connection
    M_LB.run(!left ? FORWARD : BACKWARD);
    M_RF.run(right ? FORWARD : BACKWARD);
    M_RB.run(right ? FORWARD : BACKWARD);
  }
}

void kreni() {
  pokret = !pokret;
}
