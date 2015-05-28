// This is a slightly modified version of the zumo BorderDetect.
// It has done remarkably well. By Jimmie Rodgers.

#include <ZumoBuzzer.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
 
#define LED 13
 
// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  1500 // microseconds
  
// these might need to be tuned for different motor types
#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        400
#define ATTACK_SPEED      400
#define FORWARD_SPEED     200
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     400 // ms
 
ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

#define IR_DISTANCE A0
#define DISTANCE_THRESHOLD 150

#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
 
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

boolean huntState = false;

void waitForButtonAndCountDown()
{
  digitalWrite(LED, HIGH);
  button.waitForButton();
  digitalWrite(LED, LOW);
   
  // play audible countdown
  for (int i = 0; i < 3; i++)
  {
    delay(1000);
    buzzer.playNote(NOTE_G(3), 200, 15);
  }
  delay(1000);
  buzzer.playNote(NOTE_G(4), 500, 15);  
  delay(1000);
}
 
void setup()
{
  // uncomment if necessary to correct motor directions
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);
   
  pinMode(LED, HIGH);
   
  waitForButtonAndCountDown();
  huntState = false;
}

void loop()
{
  if (button.isPressed())
  {
    // if button is pressed, stop and wait for another press to go again
    motors.setSpeeds(0, 0);
    button.waitForRelease();
    waitForButtonAndCountDown();
    huntState = false;
  }
   

  sensors.read(sensor_values);
  
  if (huntState){
    while(analogRead(IR_DISTANCE > DISTANCE_THRESHOLD)){
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
      delay(50);
    }
    huntState = false;
  }
  else if (sensor_values[0] < QTR_THRESHOLD)
  {
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(random(100,TURN_DURATION));
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    //huntState = true;
  }
  else if (sensor_values[5] < QTR_THRESHOLD)
  {
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(random(100,TURN_DURATION));
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    //huntState = true;
  }
  else if (analogRead(IR_DISTANCE) > DISTANCE_THRESHOLD){
     motors.setSpeeds(ATTACK_SPEED, ATTACK_SPEED);
  }
  else
  {
    // otherwise, go straight
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  //delay(50);
}
