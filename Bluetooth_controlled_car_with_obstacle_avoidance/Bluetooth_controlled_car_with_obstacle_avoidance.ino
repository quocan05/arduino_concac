#include <SoftwareSerial.h>
#include <Servo.h>
SoftwareSerial bluetooth(3,2); // RX, TX
Servo Myservo;
char t;

#define MLa 12     //left motor 1st pin
#define MLb 11    //left motor 2nd pin
#define MRa 10    //right motor 1st pin
#define MRb 9    //right motor 2nd pin

int ENA = 13;
int IN1 = 12;
int IN2 = 11;
int IN3 = 10;
int IN4 = 9;
int ENB = 8;

#define trigPin 5           // Trig Pin Of HC-SR04
#define echoPin 4        // Echo Pin Of HC-SR04
long duration, distance;
 
void setup()
{
  Serial.begin(9600);
bluetooth.begin(9600);
pinMode(IN1,OUTPUT);   //left motors forward
pinMode(IN2,OUTPUT);   //left motors reverse
pinMode(IN3,OUTPUT);   //right motors forward
pinMode(IN4,OUTPUT);   //right motors reverse
pinMode(trigPin, OUTPUT);       // Set Trig Pin As O/P To Transmit Waves
pinMode(echoPin, INPUT);        //Set Echo Pin As I/P To Receive Reflected Waves
  Myservo.attach(7);


}
 
void loop() 
{
if(bluetooth.available()>0)
{
  t = bluetooth.read();
  Serial.println(t);
}
 switch(t)
 {
  case 'F':
        {            //move forward(all motors rotate in forward direction)
          tien();

           digitalWrite(trigPin, LOW);
           delayMicroseconds(2);   
           digitalWrite(trigPin, HIGH);       // Transmit Waves For 10us
           delayMicroseconds(10);
           duration = pulseIn(echoPin, HIGH);        // Receive Reflected Waves
           distance = duration / 58.2;                       // Get Distance
           Serial.println(distance);
           delay(10);
  if (distance > 3)               // Condition For Absence Of Obstacle            
  {
    Myservo.write(90);
    tien();                                                       
  }
  else if ((distance < 2)&&(distance > 0))            // Condition For Presence Of Obstacle
  {
    dung_lai();
    delay(100);
    
    Myservo.write(0);
    delay(500);
    Myservo.write(180);
    delay(500);
    Myservo.write(90);
    delay(500);
    
    lui();
    delay(500);
    dung_lai();  
    delay(100);  
    tien_phai();  
    delay(500);
          delay(10);
        }
        }
        
   break;
   
  case 'B':
        {      //move reverse (all motors rotate in reverse direction)
          lui();
         
          delay(10);
        }
        
   break;
   
  case 'L':
        {      //turn right (left side motors rotate in forward direction, right side motors doesn't rotate)
          trai();
        }
   break;
   
  case 'R':
        {      //turn left (right side motors rotate in forward direction, left side motors doesn't rotate)
          phai();
        }
   break;

   case 'S':
        {           
          //STOP (all motors stop)
          dung_lai();
        }
        break; 
 }
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

void tien_trai(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);  
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  
}

void tien_phai(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);  
}

void lui_trai(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  
}

void lui_phai(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);  
}

void dung_lai(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);  
}
