#include "SoftwareSerial.h"

SoftwareSerial HC05(4,3); //TX, RX
void setup() {
  // put your setup code here, to run once:
   HC05.begin(9600);
  Serial.begin(9600);
}
void loop() {
  // put your main code here, to run repeatedly:
  if (HC05.available()) {
    Serial.println("ok");
  }
}
