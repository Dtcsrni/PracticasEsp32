#include <esp_now.h>
#include <WiFi.h>

// REEMPLAZAR CON LA DIRECCIÓN MAC DE TU RECEPTOR
uint8_t MAC_Receptor[] = {0xEC, 0x62, 0x60, 0x1E, 0xBE, 0x7C};

// Ejemplo de estructura para enviar datos
// Debe coincidir con la estructura del receptor
typedef struct estructura_mensaje {
  int numero;
   char mensaje[32];
} estructura_mensaje;

// Crear una estructura estructura_mensaje llamada misDatos
estructura_mensaje misDatos;

esp_now_peer_info_t infoCompanero;

// Callback cuando se envían datos
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nEstado del último mensaje:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Mensaje entregado" : "Fallo al entregar mensaje");
}

void setup() {
  // Iniciar el Monitor Serial
  Serial.begin(115200);

  // Configurar el dispositivo como una Estación Wi-Fi
  WiFi.mode(WIFI_STA);

  // Inicializar ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al inicializar ESP NOW");
    return;
  }

  // Una vez que ESPNow se ha inicializado correctamente, nos registraremos para recibir la confirmación
  // del estado del paquete transmitido
  esp_now_register_send_cb(OnDataSent);

  // Registrar compañero
  memcpy(infoCompanero.peer_addr, MAC_Receptor, 6);
  infoCompanero.channel = 0;
  infoCompanero.encrypt = false;

  // Agregar compañero
  if (esp_now_add_peer(&infoCompanero) != ESP_OK) {
    Serial.println("Fallo al agregar compañero");
    return;
  }
}

void loop() {
  // Establecer valores para enviar

  misDatos.numero = 10;
  strcpy(misDatos.mensaje, "La calificación es: ");

  // Enviar mensaje a través de ESP-NOW
  esp_err_t result = esp_now_send(MAC_Receptor, (uint8_t *)&misDatos, sizeof(misDatos));

  if (result == ESP_OK) {
    Serial.println("Enviado correctamente");
  } else {
    Serial.println("Error al enviar datos");
  }
  delay(2000);
}
