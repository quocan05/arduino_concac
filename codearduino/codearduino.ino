
/**** Giới thiệu code ***************
 *  Điều kiển theo các nút bấm và điểu kiển tự động như sau:
 *  Code sử dụng biến giới hạn là 25 cm để xác định khoảng cách cần dừng.
 *  Khi ta muốn thay đổi khoảng cách, ta thay đổi tham số "gioihan" đang để mặc định là 25. ( đơn vị cm)
 *  Xe ưu tiên rẽ trái trong các trường  hợp phía trước có vật cản. 
 *  Ví dụ: khi khoang cách phía trước <25 , cảm biến đo bên trái trước, nếu không có vật cản thì xe rẽ trái. nếu bên trái cũng có vật cản
 *  lúc đó xe mới kiểm tra bên phải.
 *  Khi cả phía trước, trái ,phải, sau đều có chướng ngại vật. xe đứng im và đo lại khoảng cách
 *  Khi cả phía trước, trái ,phải đều có chướng ngại vật. xe đi lùi 1 đoạn và đo lại khoảng cách
 */

#include "SoftwareSerial.h"


#include <Servo.h>
#define PIN_IN_1 12
#define PIN_IN_2 11
#define PIN_IN_3 10
#define PIN_IN_4 9
#define trig 5
#define echo 4
Servo myservo;
SoftwareSerial HC05(3,2);
char command;
int gioihan = 25;
unsigned long thoigian;
int khoangcach;
int khoangcachtrai,khoangcachphai, khoangcachsau;

void setup() {
  // put your setup code here, to run once:
  myservo.attach(7); //chân của servo nối với arduino
  pinMode(trig,OUTPUT); //chân trig phát tín hiệu
  pinMode(echo,INPUT); //chân echo nhân tín hiệu
  HC05.begin(9600);
  Serial.begin(9600);
  pinMode(PIN_IN_1, OUTPUT);
  pinMode(PIN_IN_2, OUTPUT);
  pinMode(PIN_IN_3, OUTPUT);
  pinMode(PIN_IN_4, OUTPUT);
  digitalWrite(PIN_IN_1,LOW);
  digitalWrite(PIN_IN_2,LOW);
  digitalWrite(PIN_IN_3,LOW);
  digitalWrite(PIN_IN_4,LOW);
  myservo.write(90);
  delay(500);
}

void loop() {
  if (HC05.available()>0) {
  command=HC05.read();
  HC05.println(command);
  if(command=='F'){
    khoangcach=0;
    dokhoangcach();
    if(khoangcach<=gioihan){
      stop();
    HC05.println("stop");
    }
    else{
      moveForward();
    }
  }
  if(command=='B'){
     moveBack();
  }
  if(command=='L'){
     moveLeft();
  }
  if(command=='R'){
     moveRight();
  }
  if(command=='S'){
    stop();
  }
  if(command=='X'){
     Serial.println("carauto");
    carauto();
    while(Serial.available()==0){
      carauto();
    } 
}
}
}

void carauto(){
  khoangcach=0;
  dokhoangcach();
  if(khoangcach>gioihan||khoangcach==0){
    dokhoangcach();
    if(khoangcach>gioihan||khoangcach==0) 
     {
       moveForward();   
     }   
  }
  else{
     resetdongco();
      quaycbtrai();
      
      khoangcachtrai=khoangcach;
     
      quaycbphai();
      khoangcachphai=khoangcach;
      if(khoangcachphai<10 && khoangcachtrai<10){
        quaycbvesau();
        khoangcachsau=khoangcach;
        if(khoangcachsau < 10) {
          resetdongco();
        }
        else {
        moveBack();
        }
      }
      else
      {
        if(khoangcachphai>khoangcachtrai) 
        {
          moveLeft();
          delay(500);
        }
        if(khoangcachphai<khoangcachtrai) 
        {
          moveRight();
          delay(500);
        }
      }
  }
}
void dokhoangcach(){
  digitalWrite(trig,0); //tắt chân trig
  delayMicroseconds(2);
  digitalWrite(trig,1); //phát sóng từ chân trig
  delayMicroseconds(10);
  digitalWrite(trig,0); //tắt chân trig
  //tính thời gian
  thoigian = pulseIn(echo,HIGH);
  //khoảng cách đến vật
  khoangcach = int(thoigian/2/29.412);
}

void resetdongco()
{
  digitalWrite(PIN_IN_1,LOW);
  digitalWrite(PIN_IN_2,LOW);
  digitalWrite(PIN_IN_3,LOW);
  digitalWrite(PIN_IN_4,LOW);
}

void moveForward() {
  digitalWrite(PIN_IN_1, HIGH);
  digitalWrite(PIN_IN_2, LOW);
  digitalWrite(PIN_IN_3, HIGH);
  digitalWrite(PIN_IN_4, LOW);
}

void moveBack() {
  digitalWrite(PIN_IN_1, LOW);
  digitalWrite(PIN_IN_2, HIGH);
  digitalWrite(PIN_IN_3, LOW);
  digitalWrite(PIN_IN_4, HIGH);
}

void moveLeft() {
  digitalWrite(PIN_IN_1, LOW);
  digitalWrite(PIN_IN_2, LOW);
  digitalWrite(PIN_IN_3, HIGH);
  digitalWrite(PIN_IN_4, LOW);
}

void moveRight() {
  digitalWrite(PIN_IN_1, HIGH);
  digitalWrite(PIN_IN_2, LOW);
  digitalWrite(PIN_IN_3, LOW);
  digitalWrite(PIN_IN_4, LOW);
}

void stop() {
  digitalWrite(PIN_IN_1, LOW);
  digitalWrite(PIN_IN_2, LOW);
  digitalWrite(PIN_IN_3, LOW);
  digitalWrite(PIN_IN_4, LOW);
}

void quaycbtrai(){
  myservo.write(180);
  delay(500);
  dokhoangcach();
  myservo.write(90);
}
void quaycbphai(){
  myservo.write(0);
  delay(500);
  dokhoangcach();
  myservo.write(90);
}

void quaycbvesau()
{
  myservo.write(-90);      
  delay(1000);
   dokhoangcach();
  myservo.write(90);   
}

void resetservo(){
  myservo.write(90);
}
