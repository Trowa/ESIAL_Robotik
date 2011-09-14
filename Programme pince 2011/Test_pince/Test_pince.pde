#include <Servo.h> 

Servo verin; // Pin 5
Servo morG;  // Pin 3
Servo morD;  // Pin 6

int posG = 1500;
int posD = 1500;
int posVerin = 1500;

void setup() {
    Serial.begin(115200);
    verin.attach(5);
    morG.attach(3);
    morD.attach(6);
}

void loop() {
  int c;
  while ((c = Serial.read()) == -1);
  Serial.println(c);
  switch(c) {
     case 'q':
       posG -= 50;
       break;
     case 'w':
       posG += 50;
       break;
     case 'a':
       posD -= 50;
       break;
     case 's':
       posD += 50;
       break;
     case 'z':
       posVerin -= 50;
       break;
     case 'x':
       posVerin += 50;
       break;
  }
  morG.writeMicroseconds(posG);
  morD.writeMicroseconds(posD);
  verin.write(posVerin);
  Serial.print("posG :");
  Serial.print(posG);
  Serial.print(" posD :");
  Serial.print(posD);
  Serial.print(" posVerin :");
  Serial.println(posVerin);
}
