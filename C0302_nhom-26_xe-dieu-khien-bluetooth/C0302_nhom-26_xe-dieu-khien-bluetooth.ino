#include <Servo.h>;
#include "SoftwareSerial.h"


#define PIN_IN_1 12
#define PIN_IN_2 11
#define PIN_IN_3 10
#define PIN_IN_4 9

#define PIN_ECHO 5
#define PIN_TRIG 6
#define PIN_SERVO 7

#define COMMAND_MODE_AUTO_ON 'X'
#define COMMAND_MODE_AUTO_OFF 'x'

#define ANGLE_RIGHT 0
#define ANGLE_FORWARD 90
#define ANGLE_LEFT 180

#define COMMAND_MOVE_FORWARD 'F'
#define COMMAND_MOVE_RIGHT 'R'
#define COMMAND_MOVE_BACK 'B'
#define COMMAND_MOVE_LEFT 'L'
#define COMMAND_STOP 'S'

#define COLLISION_LIMIT 25
SoftwareSerial HC05(4,3); //TX, RX

Servo servo;
char command;
char commandCurrent = COMMAND_STOP;
boolean isAuto = false;

void setup() {
  pinMode(PIN_IN_1, OUTPUT);
  pinMode(PIN_IN_2, OUTPUT);
  pinMode(PIN_IN_3, OUTPUT);
  pinMode(PIN_IN_4, OUTPUT);

  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_TRIG, OUTPUT);

  HC05.begin(9600);
  servo.attach(PIN_SERVO);
}

void loop() {
  if (HC05.available() > 0) {
    command = Serial.read();
    HC05.println(command);
    handleCommand();
  } else {
    commandCurrent = COMMAND_STOP;
    handleMoveCommand();
  }
}

void handleCommand() {
  if (command == COMMAND_MODE_AUTO_OFF) {
    setModeAuto(false);
    return;
  }

  if (command == COMMAND_MODE_AUTO_ON) {
    setModeAuto(true);
    return;
  }

  if(isAuto) {
    moveAuto();
    return;
  }

  if (command != commandCurrent) {
    commandCurrent = command;
    handleMoveCommand();
  }
}

void setModeAuto(boolean isTurnOn) {
  isAuto = isTurnOn;
  if (isTurnOn) {
    moveAuto();
    return;
  }

  commandCurrent = COMMAND_STOP;
  handleMoveCommand();
}

void moveAuto() {
  int collisionDistance;
  int collisionDistanceLeft, collisionDistanceRight;

  while (HC05.available() == 0) {
    HC05.println("moveAuto");
    collisionDistance = getCollisionDistance(ANGLE_FORWARD);
    if (collisionDistance > COLLISION_LIMIT) {
      moveForward();
      continue;
    }

    moveBack();
    delay(300);
    stop();
    delay(100);

    collisionDistanceLeft = getCollisionDistance(ANGLE_LEFT);
    collisionDistanceRight = getCollisionDistance(ANGLE_RIGHT);

    if  (collisionDistanceLeft > collisionDistanceRight) {
      if (collisionDistanceLeft > COLLISION_LIMIT || collisionDistanceLeft == 0) {
        moveLeft();
        delay(300);
        continue;
      }

      if (collisionDistanceRight > COLLISION_LIMIT || (collisionDistanceRight == 0)) {
        moveRight();
        delay(300);
      }
    } else {
      if (collisionDistanceRight > COLLISION_LIMIT || (collisionDistanceRight == 0)) {
        moveRight();
        delay(300);
        continue;
      }

      if (collisionDistanceLeft > COLLISION_LIMIT || collisionDistanceLeft == 0) {
        moveLeft();
        delay(300);
      }
    }
  }
}

int getCollisionDistance(int angle) {
  servo.write(angle);

  unsigned long time;
  int collisionDistance;

  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  time = pulseIn(PIN_ECHO, HIGH);
  collisionDistance = int (time / 2 / 29.412);
  return collisionDistance;
}

void handleMoveCommand() {
  switch (commandCurrent) {
    case COMMAND_MOVE_FORWARD:
      moveForward();
      break;

    case COMMAND_MOVE_BACK:
      moveBack();
      break;

    case COMMAND_MOVE_LEFT:
      moveLeft();
      break;

    case COMMAND_MOVE_RIGHT:
      moveRight();
      break;

    case COMMAND_STOP:
      stop();
      break;
  }
}

void moveForward() {
  HC05.println("moveForward");
  HC05.println(command);
  digitalWrite(PIN_IN_1, HIGH);
  digitalWrite(PIN_IN_2, LOW);
  digitalWrite(PIN_IN_3, HIGH);
  digitalWrite(PIN_IN_4, LOW);
}

void moveBack() {
  HC05.println("moveBack");
  digitalWrite(PIN_IN_1, LOW);
  digitalWrite(PIN_IN_2, HIGH);
  digitalWrite(PIN_IN_3, LOW);
  digitalWrite(PIN_IN_4, HIGH);
}

void moveLeft() {
  HC05.println("moveLeft");
  digitalWrite(PIN_IN_1, HIGH);
  digitalWrite(PIN_IN_2, LOW);
  digitalWrite(PIN_IN_3, LOW);
  digitalWrite(PIN_IN_4, LOW);
}

void moveRight() {
  HC05.println("moveRight");
  digitalWrite(PIN_IN_1, LOW);
  digitalWrite(PIN_IN_2, LOW);
  digitalWrite(PIN_IN_3, HIGH);
  digitalWrite(PIN_IN_4, LOW);
}

void stop() {
  digitalWrite(PIN_IN_1, LOW);
  digitalWrite(PIN_IN_2, LOW);
  digitalWrite(PIN_IN_3, LOW);
  digitalWrite(PIN_IN_4, LOW);
}
