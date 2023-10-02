#include <Arduino.h>
#line 1 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
#include <SoftwareSerial.h>

const byte RX = 4;
const byte TX = 3;

SoftwareSerial BluetoothSerial(RX, TX);

#include<Servo.h>
Servo myServo;

enum CarMode {
  MANUAL,
  AUTO
};

//CarMode mode = MANUAL; // Khởi tạo chế độ là MANUAL ban đầu


int PWM_MAX_SPEED = 255;
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

bool isAutoModeActive = false;


const char MANUAL_MODE = 'x';
const char AUTO_MODE = 'X';

char mode = MANUAL_MODE;
char command;


float  Duration; //thoi gian
float Distance; //khoang cach
#define MAX_DISTANCE 25
float Distance_Left; //Kc trai
float Distance_Right; //Kc phai
int last = 0;


#line 49 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
void setup();
#line 64 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
void loop();
#line 89 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
void handleBluetoothCommand(char command);
#line 106 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
void Car_BlueTooth();
#line 131 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
void Car_Auto();
#line 172 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
void Auto();
#line 205 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
void Back();
#line 213 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
void Forward();
#line 221 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
void Left();
#line 229 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
void Right();
#line 237 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
void Stop();
#line 245 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
void KC();
#line 260 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
void resetSv();
#line 264 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
void SvTrai();
#line 271 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
void SvPhai();
#line 49 "F:\\KMA\\Arduino\\ditmecuocdoigpt\\ditmecuocdoigpt.ino"
void setup() {
  Serial.begin(9600);
  BluetoothSerial.begin(9600);
  pinMode(MOTOR_LEFT_1, OUTPUT);
  pinMode(MOTOR_LEFT_2, OUTPUT);
  pinMode(MOTOR_RIGHT_1, OUTPUT);
  pinMode(MOTOR_RIGHT_2, OUTPUT);
  //pinMode(Button, INPUT_PULLUP);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  myServo.attach(7);
  myServo.write(90);
  last = millis();
}

void loop() {
  if (BluetoothSerial.available()) {
    command = BluetoothSerial.read();
    handleBluetoothCommand(command);
  }

  if (mode == MANUAL_MODE) {
    Serial.println("thong thuong");
    // Xử lý các lệnh khi ở chế độ điều khiển thông thường
    if (BluetoothSerial.available()) {
      SET_Mode = BluetoothSerial.read();
      Serial.println(SET_Mode);
      Car_BlueTooth();
    }
  }
  if (mode == AUTO_MODE) {
    Serial.println("tranh vat can");
    // Xử lý chế độ tự động tránh vật cản
    // Sử dụng cảm biến và logic để quyết định hướng di chuyển

    //Car_Auto();
    Auto();
  }
}

void handleBluetoothCommand(char command) {
  if (command == MANUAL_MODE) {
    mode = MANUAL_MODE;
    Serial.println("Chế độ điều khiển thông thường.");
    isAutoModeActive = false;

  } else if (command == AUTO_MODE) {
    mode = AUTO_MODE;
    Serial.println("Chế độ tự động tránh vật cản.");
    isAutoModeActive = true;
  } else {
    Serial.println(command);

  }
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
  Serial.print("Mode now:");
  Serial.println(mode);
}



void Car_Auto()
{
  
  //Serial.println("di chuyen tu dong");
  KC();
  if (Distance > 25) 
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

void Auto(){
  static unsigned long previousMillis = 0;
  const unsigned long interval = 1000;  // Đợi 1 giây (1000ms)

  unsigned long currentMillis = millis();

  // Kiểm tra xem đã đủ thời gian chưa
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Cập nhật thời gian lần cuối

    // Đọc dữ liệu từ BluetoothSerial
    while (BluetoothSerial.available()) {
      char receivedChar = BluetoothSerial.read();
      // Kiểm tra xem ký tự nhận được có phải là 'S' không
      if (receivedChar != 'S') {
        // Nếu không phải là 'S', xử lý dữ liệu
        handleBluetoothCommand(receivedChar);
        Serial.print("command handle:");
        Serial.println(receivedChar);
      }
      
      // Nếu là 'S', không làm gì cả và tiếp tục đọc ký tự tiếp theo từ BluetoothSerial.
    }
  }
  //Distance = 0;
  Car_Auto();
  Serial.print("Mode now:");
  Serial.println(mode);
  //delay(1000);
  
}


void Back()
{
Serial.println("lui");
  digitalWrite(MOTOR_LEFT_1, 0);
  digitalWrite(MOTOR_LEFT_2, 1);
  digitalWrite(MOTOR_RIGHT_1, 0);
  digitalWrite(MOTOR_RIGHT_2, 1);
}
void Forward()
{
  Serial.println("tien");
  digitalWrite(MOTOR_LEFT_1, 1);
  digitalWrite(MOTOR_LEFT_2, 0);
  digitalWrite(MOTOR_RIGHT_1, 1);
  digitalWrite(MOTOR_RIGHT_2, 0);
}
void Left()
{
Serial.println("trai");
  analogWrite(MOTOR_LEFT_1, PWM_SPEED);
  digitalWrite(MOTOR_LEFT_2, 0);
  digitalWrite(MOTOR_RIGHT_1, 0);
  digitalWrite(MOTOR_RIGHT_2, 1);
}
void Right()
{
  Serial.println("phai");
  digitalWrite(MOTOR_LEFT_1, 0);
  digitalWrite(MOTOR_LEFT_2, 1);
  analogWrite(MOTOR_RIGHT_1, PWM_SPEED);
  digitalWrite(MOTOR_RIGHT_2, 0);
}
void Stop()
{
  Serial.println("dung");
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
  Distance =  Duration / 2 / 29.412;
  Serial.print("Distance: ");
  Serial.println(Distance);
  
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

