#include <ESP32Servo.h>
int entradaPotenciometro = 0;
int anguloServo = 0;
Servo servo;
int pinPotenciometro=2;
int pinServo=4;
void setup() {
  Serial.begin(115200);
  pinMode(pinPotenciometro, INPUT);
  servo.attach(pinServo, 500, 2500);
}

void loop() {
  entradaPotenciometro = analogRead(pinPotenciometro);
  anguloServo = (entradaPotenciometro * 0.04395604395);
  Serial.print("entradaPotenciometro:");
  Serial.println(entradaPotenciometro);
  Serial.print("anguloServo:");
  Serial.println(anguloServo);
  Serial.println("------------------");
  servo.write(anguloServo);
  delay(10); 
}
