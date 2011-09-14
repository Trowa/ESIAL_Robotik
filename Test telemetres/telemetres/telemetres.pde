int TelemAv1 = A5;
int ctrlAv1 = 2;

int TelemAv2 = A3;
int ctrlAv2 = 4;

int TelemAr = A0;
int ctrlAr = 7;

void setup() {
  Serial.begin(115200);
  // declare the ledPin as an OUTPUT:
  pinMode(ctrlAv1, OUTPUT);
  digitalWrite(ctrlAv1, LOW);
  pinMode(ctrlAv2, OUTPUT);
  digitalWrite(ctrlAv2, LOW);
  pinMode(ctrlAr, OUTPUT);
  digitalWrite(ctrlAr, LOW);
}

void loop() {
  // read the value from the sensor:
  digitalWrite(ctrlAv1, HIGH);
  delayMicroseconds(20);
  digitalWrite(ctrlAv1, LOW);
  delay(100);    
  Serial.print(analogRead(TelemAv1));
  
  digitalWrite(ctrlAv2, HIGH);
  delayMicroseconds(20);
  digitalWrite(ctrlAv2, LOW);
  delay(100);
  Serial.print("  ");  
  Serial.print(analogRead(TelemAv2));
  
  digitalWrite(ctrlAr, HIGH);
  delayMicroseconds(20);
  digitalWrite(ctrlAr, LOW);
  delay(100);
  Serial.print("  ");  
  Serial.println(analogRead(TelemAr));
  delay(50);                 
}
