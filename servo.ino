#include <ESP32Servo.h>
int entradaPotenciometro = 0;
int anguloServo = 0;
Servo servo;
int pinPotenciometro=2;
int pinServo=4;
void setup() {
  Serial.begin(9600);
  pinMode(pinPotenciometro, INPUT);
  servo.attach(pinServo, 500, 2500);
}

void loop() {
  servo.write(map(analogRead(pinPotenciometro), 0, 4095, 0, 190));
  delay(3000); 
}
