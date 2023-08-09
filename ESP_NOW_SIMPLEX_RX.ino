#include <esp_now.h>
#include <WiFi.h>

// Estructura de ejemplo para recibir datos
// Debe coincidir con la estructura del remitente
typedef struct estructura_mensaje {
  int numero;
  char mensaje[200];
} estructura_mensaje;

// Crear una estructura llamada misDatos
estructura_mensaje misDatos;

// Función de devolución de llamada que se ejecutará cuando se reciban datos
void OnDataRecv(const uint8_t *mac, const uint8_t *datosRecibidos, int tamano) {
  memcpy(&misDatos, datosRecibidos, sizeof(misDatos));
  Serial.print("Bytes recibidos: ");
  Serial.println(tamano);
  Serial.println(misDatos.mensaje);
  Serial.print(":");
  Serial.println(misDatos.numero);
}
 
void setup() {
  // Inicializar el Monitor Serial
  Serial.begin(115200);
  
  // Configurar el dispositivo como una Estación Wi-Fi
  WiFi.mode(WIFI_STA);

  // Inicializar ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando protocolo ESP NOW");
    return;
  }
  
  // Una vez que ESPNow se ha inicializado correctamente, nos registraremos para recibir
  // información sobre paquetes recibidos
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}
