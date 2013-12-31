// Include the Stepper Library
#include <Stepper.h>

// Map our pins to constants to make things easier to keep track of
const int pwmA = 3;
const int pwmB = 11;
const int brakeA = 9;
const int brakeB = 8;
const int dirA = 12;
const int dirB = 13;

// The amount of steps for a full revolution of your motor.
// 360 / stepAngle
const int STEPS = 200;
int speed = 60;

// Initialize the Stepper class
Stepper myStepper(STEPS, dirA, dirB);

void setup() {
  // Set the RPM of the motor
  myStepper.setSpeed(60);

  // Turn on pulse width modulation
  pinMode(pwmA, OUTPUT);
  digitalWrite(pwmA, HIGH);
  pinMode(pwmB, OUTPUT);
  digitalWrite(pwmB, HIGH);

  // Turn off the brakes
  pinMode(brakeA, OUTPUT);
  digitalWrite(brakeA, LOW);
  pinMode(brakeB, OUTPUT);
  digitalWrite(brakeB, LOW);

  // Log some shit
  Serial.begin(9600);
  
  myStepper.setSpeed(2);
}

void loop() {
  // Move the motor X amount of steps
 // speed = speed  + 10;
  
  myStepper.step(STEPS);
  //Serial.println(speed);
  // Pause
  //delay(100);

  // Move the motor X amount of steps the other way
 // myStepper.step(-STEPS * 10);
 // Serial.println(-STEPS);
 // // Pause
 // delay(1000);
}
