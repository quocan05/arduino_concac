#include <SoftwareSerial.h> //2.5.0
const byte TX = 3;
const byte RX = 4;
SoftwareSerial MySerial = SoftwareSerial(RX, TX);

#include<Servo.h>
Servo myServo;
int PWM_MAX_SPEED = 150;
int PWM_SPEED = 204;
char SET_Mode;
int MOTOR_LEFT_1 = 12;
int MOTOR_LEFT_2 = 11;
int MOTOR_RIGHT_1 = 10;
int MOTOR_RIGHT_2 = 9;
int ENA   = 13;
int ENB   = 8;
int trig = 6;
int echo = 5;

char StatusButton = 'x';
int Save_StatusButton = 0;

float  Duration; //thoi gian
float Distance; //khoang cach
float Distance_Left; //Kc trai
float Distance_Right; //Kc phai
int last = 0;
void setup()
{
  pinMode(MOTOR_LEFT_1, OUTPUT);
  pinMode(MOTOR_LEFT_2, OUTPUT);
  pinMode(MOTOR_RIGHT_1, OUTPUT);
  pinMode(MOTOR_RIGHT_2, OUTPUT);
  //pinMode(Button, INPUT_PULLUP);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  myServo.attach(7);
  myServo.write(90);
  MySerial.begin(9600);
  Serial.begin(9600);
  last = millis();
}
void Back()
{
  analogWrite(ENA, PWM_MAX_SPEED);
  analogWrite(ENB, PWM_MAX_SPEED);
  digitalWrite(MOTOR_LEFT_1, 0);
  digitalWrite(MOTOR_LEFT_2, 1);
  digitalWrite(MOTOR_RIGHT_1, 0);
  digitalWrite(MOTOR_RIGHT_2, 1);
}
void Forward()
{
  analogWrite(ENA, PWM_MAX_SPEED);
  analogWrite(ENB, PWM_MAX_SPEED);
  digitalWrite(MOTOR_LEFT_1, 1);
  digitalWrite(MOTOR_LEFT_2, 0);
  digitalWrite(MOTOR_RIGHT_1, 1);
  digitalWrite(MOTOR_RIGHT_2, 0);
}
void Left()
{
  analogWrite(ENA, PWM_MAX_SPEED);
  analogWrite(ENB, PWM_MAX_SPEED);
  analogWrite(MOTOR_LEFT_1, PWM_SPEED);
  digitalWrite(MOTOR_LEFT_2, 0);
  digitalWrite(MOTOR_RIGHT_1, 0);
  digitalWrite(MOTOR_RIGHT_2, 1);
}
void Right()
{
  analogWrite(ENA, PWM_MAX_SPEED);
  analogWrite(ENB, PWM_MAX_SPEED);
  digitalWrite(MOTOR_LEFT_1, 0);
  digitalWrite(MOTOR_LEFT_2, 1);
  analogWrite(MOTOR_RIGHT_1, PWM_SPEED);
  digitalWrite(MOTOR_RIGHT_2, 0);
}
void Stop()
{
  analogWrite(ENA, PWM_MAX_SPEED);
  analogWrite(ENB, PWM_MAX_SPEED);
  digitalWrite(MOTOR_LEFT_1, 0);
  digitalWrite(MOTOR_LEFT_2, 0);
  digitalWrite(MOTOR_RIGHT_1, 0);
  digitalWrite(MOTOR_RIGHT_2, 0);
}
void KC()
{
  digitalWrite(trig, LOW); // tat chan trig
  delayMicroseconds(2);
  digitalWrite(trig, HIGH); // phat xung tu chan trig
  delayMicroseconds(10);
  digitalWrite(trig, LOW); // tat chan trig

  Duration = pulseIn(echo, HIGH);
  Distance = ( Duration * .0343) / 2;
  Serial.print("Distance: ");
  Serial.println(Distance);
  delay(200);
}

void resetSv() // quay servo vào giữa
{
  myServo.write(90);
}
void SvTrai() //quay servo sang trai
{
  myServo.write(50);
  delay(200);
  KC();
  resetSv();
}
void SvPhai() // quay servo sang phai
{
  myServo.write(150);
  delay(200);
  KC();
  resetSv();
}
void loop()
{
  BUTTON();
}
void BUTTON()
{
  StatusButton = MySerial.read();
  Serial.println(StatusButton);
  if (MySerial.read() == 'X')
  {
    Serial.println("Nút nhấn đã dc nhấn");
    delay(500);
    while (1)
    {
      //Car_Auto();
      //Car_BlueTooth();
      delay(100);
      Serial.print("nut save");
      Serial.println(Save_StatusButton);
      Change_Status();
    }
  }
  Serial.println(Save_StatusButton);
  
}
void Change_Status()
{
   char receivedData = MySerial.read();
  if(receivedData == 'x'){
    SET_Mode = MySerial.read();
      Serial.print(SET_Mode);
      Car_BlueTooth();
  } else if(receivedData == 'X'){
    Car_Auto();
  }
//  if ( MySerial.available() )
//  {
//    if ( Save_StatusButton == 1)
//    {
//      SET_Mode = MySerial.read();
//      Serial.print(SET_Mode);
//      Car_BlueTooth();
//    }
//    else if ( Save_StatusButton == 0)
//    {
//      Car_Auto();
//    }
//
//  }
//  else
//  {
//    if ( Save_StatusButton == 0)
//    {
//      Car_Auto();
//    }
//    else
//    {
//      Car_BlueTooth();
//    }
//  }

}
void Car_BlueTooth()
{
  switch (SET_Mode)
  {
    case'F':
      Forward();
      break;
    case'B':
      Back();
      break;
    case'L':
      Left();
      break;
    case'R':
      Right();
      break;
    default:
      Stop();
  }
}
void Car_Auto()
{
  KC();
  if (Distance > 25) // nếu khoang cach >50cm
  {
    Forward(); // chay thẳng
    Serial.println("Tien");
  }
  else
  {
    Stop(); // dừng
    SvTrai(); // quay servo sang trai
    Distance_Left = Distance; // gán khoảng cách đo đc == biến Distance_Left
    SvPhai(); // quay servo sang phai
    Distance_Right = Distance; // gan khoang cach do dc == biến Distance_Right
    if (Distance_Left < 15 && Distance_Right < 15)
    {
      Back();
      Serial.println("Lui");
      delay(100);
    }
    else
    {
      if (Distance_Left < Distance_Right) // nếu khoảng cách trái < khoang cach phai
      {
        Right();  // xe chạy sang phai
        Serial.println("Phai");
        delay(100);
      }
      if (Distance_Left > Distance_Right)
      {
        Left(); // xe chay sang trai
        Serial.println("Trai");
        delay(100);
      }
    }
  }
}
