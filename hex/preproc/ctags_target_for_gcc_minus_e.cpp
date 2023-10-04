# 1 "F:\\KMA\\cnpmn\\bcao\\gew\\gew.ino"
# 2 "F:\\KMA\\cnpmn\\bcao\\gew\\gew.ino" 2
# 3 "F:\\KMA\\cnpmn\\bcao\\gew\\gew.ino" 2
# 4 "F:\\KMA\\cnpmn\\bcao\\gew\\gew.ino" 2
Servo myServo;
int servoPin = 7;

SoftwareSerial HC05(4, 3); //TX, RX
int ENA = 13;
int IN1 = 12;
int IN2 = 11;
int IN3 = 10;
int IN4 = 9;
int ENB = 8;


float Duration; //thoi gian
float Distance; //khoang cach


float Distance_Left; //Kc trai
float Distance_Right; //Kc phai
int last = 0;


enum CarMode {
  MANUAL,
  AUTO
};



char dieu_khien;
int Speed = 150;

char command;
CarMode mode = MANUAL;

const int trigPin = 6;
const int echoPin = 5;
NewPing sonar(trigPin, echoPin, 200);

unsigned long thoigian;
int khoangcach;
int khoangcachtrai, khoangcachphai;
int gioihan = 30;
long duration, distance;

void setup() {
  HC05.begin(9600);
  Serial.begin(9600);
  pinMode(IN1, 0x1);
  pinMode(IN2, 0x1);
  pinMode(IN3, 0x1);
  pinMode(IN4, 0x1);
  pinMode(ENA, 0x1);
  pinMode(ENB, 0x1);

  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);

  pinMode(trigPin, 0x1);
  pinMode(echoPin, 0x0);
  //last = millis();
  //Serial.begin(9600);

  myServo.attach(servoPin);

  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void loop() {
  if (HC05.available()) {
    command = HC05.read();
    handleMode();
    if (mode == MANUAL) {
    Control_Mode();
  }

  if (mode == AUTO) {
    //Auto_Mode();
    Auto_avoid();
  }
  }


}

void handleMode() {
  Serial.print("receive: ");
  Serial.println(command);

  if (command == 'F') {
    Serial.println("Mode now: CONTROL MANUAL");
    mode = MANUAL;
  } else if (command == 'J') {
    Serial.println("Mode now: AUTO");
    mode = AUTO;
  }

  Serial.println(mode);
}


void Auto_avoid() {
  Serial.println("auto avoiding");
  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);

  if (distance <= 45) {
    Stop();
    delay(300);
    lui();
    delay(400);
    Stop();
    delay(300);
    distanceRight = lookRight();
    delay(300);
    distanceLeft = lookLeft();
    delay(300);

    if (distance >= distanceLeft) {
      phai();
      Stop();
    }
    else {
      trai();
      Stop();
    }
  }
  else {
    tien();
  }
  distance = readPing();
  Serial.print("Distance ");
  Serial.println(distance);
}

int lookRight() {
  myServo.write(30);
  delay(500);
  int distance = readPing();
  delay(100);
  myServo.write(85);
  return distance;
}

int lookLeft() {
  myServo.write(150);
  delay(500);
  int distance = readPing();
  delay(100);
  myServo.write(85);
  return distance;
  delay(100);
}

void Auto_Mode() {
  Serial.println("auto avoid.....");
  KC();
  if (Distance > 25)
  {
    tien(); // chay thẳng
    Serial.println("Tien");
  }
  else
  {
    Stop(); // dừng
    SvTrai(); // quay servo sang trai
    Distance_Left = Distance; // gán khoảng cách đo đc == biến Distance_Left
    SvPhai(); // quay servo sang phai
    Distance_Right = Distance; // gan khoang cach do dc == biến Distance_Right
    if (Distance_Left < 25 && Distance_Right < 25)
    {
      lui();
      Serial.println("Lui");
      delay(300);
    }
    else
    {
      if (Distance_Left < Distance_Right) // nếu khoảng cách trái < khoang cach phai
      {
        phai(); // xe chạy sang phai
        Serial.println("Phai");
        delay(300);
      }
      if (Distance_Left > Distance_Right)
      {
        trai(); // xe chay sang trai
        Serial.println("Trai");
        delay(300);
      }
    }
  }
}

void Control_Mode() {
  dieu_khien = HC05.read();
  Serial.println(command);
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
      Stop();
      break;

  }
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
  }
  return cm;
}

void KC()
{
  digitalWrite(trigPin, 0x0); // tat chan trig
  delayMicroseconds(2);
  digitalWrite(trigPin, 0x1); // phat xung tu chan trig
  delayMicroseconds(10);
  digitalWrite(trigPin, 0x0); // tat chan trig

  Duration = pulseIn(echoPin, 0x1);
  Distance = Duration / 2 / 29.412;
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




void tien() {
  Serial.println("tien");
  digitalWrite(IN1, 0x1);
  digitalWrite(IN2, 0x0);
  digitalWrite(IN3, 0x1);
  digitalWrite(IN4, 0x0);
}

void lui() {
  Serial.println("lui");
  digitalWrite(IN1, 0x0);
  digitalWrite(IN2, 0x1);
  digitalWrite(IN3, 0x0);
  digitalWrite(IN4, 0x1);
}

void phai() {
  Serial.println("phai");
  digitalWrite(IN1, 0x1);
  digitalWrite(IN2, 0x0);
  digitalWrite(IN3, 0x0);
  digitalWrite(IN4, 0x1);
}

void trai() {
  Serial.println("trai");
  digitalWrite(IN1, 0x0);
  digitalWrite(IN2, 0x1);
  digitalWrite(IN3, 0x1);
  digitalWrite(IN4, 0x0);
}

void tien_trai() {

  digitalWrite(IN1, 0x0);
  digitalWrite(IN2, 0x0);
  digitalWrite(IN3, 0x1);
  digitalWrite(IN4, 0x0);
}

void tien_phai() {

  digitalWrite(IN1, 0x1);
  digitalWrite(IN2, 0x0);
  digitalWrite(IN3, 0x0);
  digitalWrite(IN4, 0x0);
}

void lui_trai() {

  digitalWrite(IN1, 0x0);
  digitalWrite(IN2, 0x0);
  digitalWrite(IN3, 0x0);
  digitalWrite(IN4, 0x1);
}

void lui_phai() {

  digitalWrite(IN1, 0x0);
  digitalWrite(IN2, 0x1);
  digitalWrite(IN3, 0x0);
  digitalWrite(IN4, 0x0);
}

void Stop() {
  Serial.println("dung lai");
  digitalWrite(IN1, 0x0);
  digitalWrite(IN2, 0x0);
  digitalWrite(IN3, 0x0);
  digitalWrite(IN4, 0x0);
}
