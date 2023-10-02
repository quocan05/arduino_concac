
#include <NewPing.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#define MAX_DISTANCE 20
#define MAX_SPEED 50

SoftwareSerial HC05(3,2); //TX, RX
int ENA = 13;
int IN1 = 12;
int IN2 = 11;
int IN3 = 10;
int IN4 = 9;
int ENB = 8;

Servo my_servo;
int servoPin = 7;
int trigPin = 5;
int echoPin = 4;

NewPing sonar(trigPin, echoPin, MAX_DISTANCE);
SoftwareSerial bluetooth(3, 2); // RX, TX pins for Bluetooth

void setup() {
  bluetooth.begin(9600);
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
 pinMode(IN2, OUTPUT);
 pinMode(IN3, OUTPUT);
 pinMode(IN4, OUTPUT);
 pinMode(ENA, OUTPUT);
 pinMode(ENB, OUTPUT);

 pinMode(ENA, OUTPUT);
 pinMode(ENB, OUTPUT);

 analogWrite(ENA, MAX_SPEED);
 analogWrite(ENB, MAX_SPEED);

 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);

 my_servo.attach(servoPin);
}

void loop() {
  // Đọc dữ liệu từ Bluetooth
  if (bluetooth.available()) {
    char command = bluetooth.read();
    // Xử lý lệnh từ điện thoại thông qua Bluetooth
    switch (command) {
      case 'F':
        tien();
        break;
      case 'B':
        lui();
        break;
      case 'L':
        trai();
        break;
      case 'R':
        phai();
        break;
      case 'S':
        dung_lai();
        break;
      default:
        // Không có lệnh hợp lệ
        break;
    }
  }

  // Đo khoảng cách bằng cảm biến siêu âm
  int distance = sonar.ping_cm();

  // Nếu có vật cản trong khoảng cách an toàn, thực hiện hành động
  if (distance <= 10) {
    // Thực hiện một hành động để tránh vật cản, ví dụ: quay trái
    avoidObstacle();
  }
}


void avoidObstacle() {
  // Thực hiện hành động tránh vật cản ở đây, ví dụ: quay trái
  trai();
  delay(1000); // Đợi một khoảng thời gian để tránh vật cản
  dung_lai();
}

void tien(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);  
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  
}

void lui(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  
}

void phai(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  
}

void trai(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);  
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  
}

void dung_lai(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);  
}
