#include <Servo.h>
#include <NewPing.h>
#define trig_pin 2 
#define echo_pin 3
#define TIME_OUT 5000

Servo myservo; 
NewPing sonar(trig_pin, echo_pin, 200); //sensor function
int in1 = 13;
int in2 = 12;
int in3 = 11;
int in4 = 10;
int PWM1 = 6;
int PWM2 = 5;
char val;
uint8_t speed_t = 0;
long distanceLeft = 0;
long distanceRight = 0;
long distance = 0;

void setup() 
{  
  Serial.begin(9600);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT); 
  pinMode(in3, OUTPUT); 
  pinMode(in4, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  myservo.attach(9);
  myservo.write(90);
}
uint8_t mode_t = 0;

int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}

int lookRight(){  // nhin phai lay khoang cach
  myservo.write(10);
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(90);
  return distance;
}

int lookLeft(){ // nhin trai lai khoang cach 
  myservo.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(90);
  return distance;
  delay(100);
}


void Backward()
{
   digitalWrite(in1, HIGH);
   digitalWrite(in2, LOW);
   digitalWrite(in3, HIGH);
   digitalWrite(in4, LOW); 
}

void Forward()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
}

void TurnRight()
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
}

void TurnLeft()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void Stop()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW); 
}
void loop()
{
  while (Serial.available() > 0)
  {
    val = Serial.read();
  }
  
  if(val == 'M')
  {
      mode_t  = 1;  
  }
  
  else if(val == 'A')
  {
     mode_t = 0;
  }
  while(mode_t == 1)
  {
      distance = readPing();
      Serial.print(distance);
      Serial.print("Cm ");
      Serial.println(" ");
      delay(50);
      while (Serial.available() > 0)
      {
         val = Serial.read();
      }
      if(val == 'A')
      {
          mode_t = 0;
          break;
      }
      if( val == '1') //Forward
      {
          Forward();
      }
      else if(val == '2') //Backward
      {
        Backward();
      }  
      else if(val == '3') //Left
      {
        TurnLeft();
        
      }
      else if(val == '4') //Right
      {
         TurnRight();
      }    
      else if(val == '5') //Stop
      {
        Stop();

      }

    if(val == '8')
    {
      speed_t += 20;
      analogWrite(PWM1, speed_t); 
      analogWrite(PWM2, speed_t); 
      if(speed_t > 200)speed_t = 0;
      val = 0;
    }
    else if(val == '9') 
    {
      if(speed_t > 0)  speed_t--;
      analogWrite(PWM1, speed_t); 
      analogWrite(PWM2, speed_t); 
      val = 0;
    }
  }

  while(mode_t == 0)
  {
     while (Serial.available() > 0)
      {
         val = Serial.read();
      }
      if(val == 'M')
      {
          mode_t = 1;
          break;
      }
       if(val == '8') 
        {
            speed_t += 20;
            analogWrite(PWM1, speed_t); 
            analogWrite(PWM2, speed_t); 
            if(speed_t > 200)speed_t = 0;
            val = 0;
        }
        else if(val == '9') 
        {
            if(speed_t > 0)  speed_t--;
            analogWrite(PWM1, speed_t); 
            analogWrite(PWM2, speed_t); 
            val = 0;
        }
      distance = readPing();
      
      if(distance > 0 && distance < 200)
      {
          Serial.print(distance);
          Serial.print("Cm ");
          Serial.println(" ");
          delay(2000);      
          if(distance < 20)
          {
              Stop();
              delay(500);
              Backward();
              delay(400);
              Stop();
              delay(400);
           
              distanceRight = lookRight(); // lay khoang cach ben trai
              delay(300);
            
              distanceLeft = lookLeft(); // lay khoang cach ben phai
              delay(300);
              
              if(distanceRight >  distanceLeft)
              {
                  TurnLeft();
                  delay(1000);
                  Stop();
              }

              else
              {
                  TurnRight();
                  delay(1000);
                  Stop();
              }
          }
          else{
              Forward();
         }
      }
  }
  
    
}
