#include <Servo.h>
Servo myservo;

int potpin = 0;
int val;
void setup() {
  // put your setup code here, to run once:
myservo.attach(9);
myservo.write(20);
delay(2000);
myservo.write(100);
delay(1000);
myservo.write(20);
delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
val = analogRead(potpin);
val = map(val, 0, 1023, 0, 180);
myservo.write(val);
delay(15);
}
