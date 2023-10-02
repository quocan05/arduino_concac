#include <Servo.h>
#include <SoftwareSerial.h>
const byte RX = 4;
const byte TX = 3;
Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position
SoftwareSerial BluetoothSerial(RX, TX);


enum CarMode {
  MANUAL,
  AUTO
};
bool isAutoModeActive = false;
const char MANUAL_MODE = 'x';
const char AUTO_MODE = 'X';
CarMode mode = MANUAL;

char command;

const int trig = 6;  
const int echo = 5;    
int enbA=13;
int in1 = 12;       
int in2 = 11;        
int in3 = 10;        
int in4 = 9;  
int enbB=8;      
int dongcoservo = 7;   

int gioihan = 20;//khoảng cách nhận biết vật 
int i;
int x = 0;
unsigned long thoigian; 
int khoangcach;          
int khoangcachtrai, khoangcachphai;
int maxspeed=30;

void dokhoangcach();
void dithang(int duongdi);
void disangtrai();
void disangphai();
void dilui();
void resetdongco();
void quaycbsangphai();
void quaycbsangtrai();
void setup() {
  // put your setup code here, to run once:
  myservo.attach(dongcoservo); 
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);   

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enbA, OUTPUT);
  pinMode(enbB, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(enbA, LOW);
  digitalWrite(enbB, LOW);
  myservo.write(90);
  delay(500);
  Serial.begin(9600);
  

}

void loop()
{
  if(BluetoothSerial.available()){
    command = BluetoothSerial.read();
    handleBluetoothCommand(command);
  }

  if (mode == MANUAL) {
    Serial.println("thong thuong");
    // Xử lý các lệnh khi ở chế độ điều khiển thông thường
      char SET_Mode = BluetoothSerial.read();
      Serial.println(SET_Mode);
      Car_BlueTooth(SET_Mode);
  }
  if (mode == AUTO) {
    Serial.println("tranh vat can");
    // Xử lý chế độ tự động tránh vật cản
    // Sử dụng cảm biến và logic để quyết định hướng di chuyển

    Auto();
    
  }
  
}

void handleBluetoothCommand(char command) {
  if (command == MANUAL_MODE) {
    mode = MANUAL;
    Serial.println("Chế độ điều khiển thông thường.");
    isAutoModeActive = false;

  } else if (command == AUTO_MODE) {
    mode = AUTO;
    Serial.println("Chế độ tự động tránh vật cản.");
    isAutoModeActive = true;
  } else {
    Serial.println(command);

  }
}


void Auto(){
  khoangcach = 0;
  dokhoangcach();
  Serial.println(khoangcach);
  if (khoangcach > gioihan || khoangcach == 0)
  {
      dithang();
      Serial.println("Di toi");
  }
  else
  {
    dunglai();delay(300);
    quaycbsangtrai();
    dokhoangcach();
    Serial.println(khoangcach);
    khoangcachtrai = khoangcach;
    quaycbsangphai();
    dokhoangcach();
    Serial.println(khoangcach);
    khoangcachphai = khoangcach;
    if (khoangcachphai < 15 && khoangcachtrai < 15) {
      dilui();delay(300);dunglai();delay(300);
      Serial.println("Di lui");
      
    }
    else
    {
      if (khoangcachphai >= khoangcachtrai)
      { dilui();delay(300);dunglai();delay(300);      
        disangphai();
        Serial.println("Di sang phai");
        delay(400);dunglai();delay(300);
      }
      if (khoangcachphai < khoangcachtrai)
      { dilui();delay(300);dunglai();delay(300);
        disangtrai();
        Serial.println("Di sang trai");
        delay(400);dunglai();delay(300);
      }
    }
  }

}

void Car_BlueTooth(char SET_Mode)
{
  switch (SET_Mode)
  {
    case'F':
      dithang();
      break;
    case'B':
      dilui();
      break;
    case'L':
      disangtrai();
      break;
    case'R':
      disangphai();
      break;
    default:
      dunglai();
  }
}


void dithang()
{
  analogWrite(enbA, 100);
  analogWrite(enbB, 100);
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 0);
  digitalWrite(in4, 1);


}
void dunglai(){
  analogWrite(enbA, 0);
  analogWrite(enbB, 0);
  digitalWrite(in1, 0);
  digitalWrite(in2, 0);
  digitalWrite(in3, 0);
  digitalWrite(in4, 0);
  }

void disangphai()
{
  analogWrite(enbA, 100);
  analogWrite(enbB, 0);
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 0);
  digitalWrite(in4, 0);


}
void disangtrai()
{
  analogWrite(enbA, 0);
  analogWrite(enbB, 100);
  digitalWrite(in1, 0);
  digitalWrite(in2, 0);
  digitalWrite(in3, 0);
  digitalWrite(in4, 1);

}

void dilui()
{
  analogWrite(enbA, 100);
  analogWrite(enbB, 100);
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);

}



void dokhoangcach()
{

  digitalWrite(trig, LOW); 
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);  
  delayMicroseconds(10); 
  digitalWrite(trig, LOW); 


  // Đo độ rộng xung HIGH ở chân echo.
  thoigian = pulseIn(echo, HIGH);

  khoangcach = thoigian / 2 / 29.412;

}


void quaycbsangtrai()
{
  myservo.write(150);              // tell servo to go to position in variable 'pos'
  delay(1000);
  dokhoangcach();
  myservo.write(90);              // tell servo to go to position in variable 'pos'
}
void quaycbsangphai()
{
  myservo.write(30);              // tell servo to go to position in variable 'pos'
  delay(1000);
  dokhoangcach();
  myservo.write(90);              // tell servo to go to position in variable 'pos'
}
void resetservo()
{
  myservo.write(90);
}
