|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
|| Elevador V1.0 ||||||||||||||||||||||||||||||||||||||||||||||
|| Autor: Erick Renato Vega Ceron |||||||||||||||||||||||||||||
|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

#include <LiquidCrystal.h> // Incluye la librería para el control de LCD
#include <ESP32Servo.h>    // Incluye la librería para el control de servos en ESP32
#include <Wire.h>          // Incluye la librería para comunicación I2C
#include <Adafruit_GFX.h>  // Incluye la librería para gráficos Adafruit
#include <Adafruit_SSD1306.h> // Incluye la librería para pantallas OLED SSD1306

#define SCREEN_WIDTH 128  // Ancho de la pantalla OLED, en píxeles
#define SCREEN_HEIGHT 64  // Alto de la pantalla OLED, en píxeles
#define LED_BUILTIN 2      // Pin del LED incorporado en el ESP32

// Declaración para una pantalla SSD1306 conectada a través de I2C (pines SDA, SCL)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int btnPiso1 = 35;    // Pin para el botón del piso 1
int btnPiso2 = 34;    // Pin para el botón del piso 2
int pisoAnterior = 0; // Variable para almacenar el piso anterior
int pisoActual = 1;   // Variable para almacenar el piso actual
int pisoDestino = 1;  // Variable para almacenar el piso de destino
const int servoPin = 18; // Pin para el servo motor
int gradosObjetivo = 0;   // Ángulo objetivo del servo
int gradosActuales = 0;   // Ángulo actual del servo
int TOUCH_SENSOR_VALUE = 0; // Valor del sensor táctil
bool LED = false;   // Estado del LED
Servo servo;         // Instancia del servo

void setup() {
  Serial.begin(2000000);   // Inicializa la comunicación serial
  pinMode(LED_BUILTIN, OUTPUT);   // Configura el pin del LED incorporado como salida
  pinMode(btnPiso1, INPUT_PULLUP); // Configura el pin del botón del piso 1 con resistencia pull-up
  pinMode(btnPiso2, INPUT_PULLUP); // Configura el pin del botón del piso 2 con resistencia pull-up
  
  // Inicializar la pantalla SSD1306
  Serial.print("Inicializando pantalla ");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Fallo en la asignación de SSD1306"));
    for (;;) {
      // Se repite indefinidamente si falla la inicialización de la pantalla
    }
  }
  delay(300);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.clearDisplay();
  display.setRotation(2);
  dibujarGUI(0);
  Serial.println("Inicializado elevador");
  servo.attach(servoPin, 544, 2400); // Adjunta el servo al pin y define los límites de movimiento
  servo.write(0);  // Reiniciar servo a 0 grados
}

void loop() {
  // Verifica si se presionó el botón del piso 1
  if (digitalRead(btnPiso1) == LOW) {
    pisoDestino = 1;
  } 
  // Verifica si se presionó el botón del piso 2
  else if (digitalRead(btnPiso2) == LOW) {
    pisoDestino = 2;
  } 
  // Verifica si se tocó el sensor táctil T7
  else if (touchRead(T7) < 70) {
    pisoDestino = 3;
  }
  
  // Compara el piso de destino con el piso actual y realiza cambios si son diferentes
  if (pisoDestino != pisoActual) {
    Serial.println("Piso Destino = " + String(pisoDestino));
    cambiarPiso(pisoDestino);
  }
}

// Función para cambiar de piso
void cambiarPiso(int pisoDestino) {
  // Asigna ángulo objetivo del servo según el piso de destino
  switch (pisoDestino) {
    case 1:
      gradosObjetivo = 0;
      break;
    case 2:
      gradosObjetivo = 90;
      break;
    case 3:
      gradosObjetivo = 180;
      break;
  }
  
  // Realiza una transición suave del servo hacia el ángulo objetivo
  while (gradosActuales != gradosObjetivo) {
    if (LED) {
      digitalWrite(LED_BUILTIN, LOW);
      LED = false;
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
      LED = true;
    }
    
    display.clearDisplay();
    if (gradosObjetivo > gradosActuales) {
      gradosActuales++;
      dibujarGUI(1);
    } else if (gradosObjetivo < gradosActuales) {
      gradosActuales--;
      dibujarGUI(2);
    }
    servo.write(gradosActuales);
    
    // Actualiza el piso actual basado en el ángulo del servo
    if (gradosActuales > 0 && gradosActuales < 40) {
      pisoActual = 1;
    } else if (gradosActuales > 70 && gradosActuales < 100) {
      pisoActual = 2;
    } else if (gradosActuales > 110 && gradosActuales <= 120) {
      pisoActual = 3;
    }
    delay(10);
  }
  dibujarGUI(0);
  pisoAnterior = pisoActual;
  pisoActual = pisoDestino;
}

// Función para dibujar la interfaz gráfica en la pantalla OLED
void dibujarGUI(int estado) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("|CyberSys Elevadores|");
  display.setCursor(0, 60 - map(gradosActuales, 0, 180, 5, 50));
  if (estado == 1) {
    display.print("^");
    display.setCursor(15, 20);
    display.print("Subiendo a piso " + String(pisoDestino));
  } else if (estado == 2) {
    display.print("v");
    display.setCursor(15, 20);
    display.print("Bajando a piso " + String(pisoDestino));
  } else {
    display.print("#");
  }
  display.setCursor(15, 50);
  display.print("Piso Actual: " + String(pisoActual));
  display.display();
}
