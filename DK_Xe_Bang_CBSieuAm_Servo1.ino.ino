#include <Servo.h>
Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position


const int trig = 6;  
const int echo = 5;    
int enbA=13;
int in1 = 12;       
int in2 = 11;        
int in3 = 10;        
int in4 = 9;  
int enbB=8;      
int dongcoservo = 7;   

int gioihan = 25;//khoảng cách nhận biết vật 
int i;
int x = 0;
unsigned long thoigian; 
int khoangcach, khoangcachdo;          
int khoangcachtrai, khoangcachphai;
int maxspeed=150;


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
//  digitalWrite(in1, LOW);
//  digitalWrite(in2, LOW);
//  digitalWrite(in3, LOW);
//  digitalWrite(in4, LOW);
//  digitalWrite(enbA, LOW);
//  digitalWrite(enbB, LOW);
  myservo.write(90);
  delay(500);
  Serial.begin(9600);


}

void loop()
{
  khoangcach = 0;
  dokhoangcach();
  khoangcach = khoangcachdo;
  Serial.println(khoangcach);
  if (khoangcach > gioihan)
  {
      dithang();
      Serial.println("Di toi");
  }
  else 
  {
    dunglai();delay(1000);
    quaycbsangtrai();
    dokhoangcach();
    Serial.println(khoangcach);
    khoangcachtrai = khoangcachdo;
    quaycbsangphai();
    dokhoangcach();
    Serial.println(khoangcach);
    khoangcachphai = khoangcachdo;
    if (khoangcachphai < 20 && khoangcachtrai < 20) {
      dilui();delay(600);dunglai();delay(600);
      Serial.println("Di lui");
      
    }
    else
    {
      if (khoangcachphai >= khoangcachtrai)
      { dilui();delay(600);dunglai();delay(600);      
        disangphai();
        Serial.println("Di sang phai");
        delay(400);dunglai();delay(400);
      }
      if (khoangcachphai < khoangcachtrai)
      { dilui();delay(600);dunglai();delay(600);
        disangtrai();
        Serial.println("Di sang trai");
        delay(400);dunglai();delay(400);
      }
    }
  }

}
void dithang()
{
  analogWrite(enbA, 150);
  analogWrite(enbB, 150);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);


}
void dunglai(){
  analogWrite(enbA, 150);
  analogWrite(enbB, 150);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  }

void disangphai()
{
  analogWrite(enbA, 150);
  analogWrite(enbB, 150);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);


}
void disangtrai()
{
  analogWrite(enbA, 150);
  analogWrite(enbB, 150);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

}

void dilui()
{
  analogWrite(enbA, 150);
  analogWrite(enbB, 150);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

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

  khoangcachdo = thoigian / 2 / 29.412;
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
