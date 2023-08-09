#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t MAC_Receptor[] = {0xEC,0x62,0x60,0x1E,0xBE,0x7C};

// Structure example to send data
// Must match the receiver structure
typedef struct estructura_mensaje {
  int mensaje;

} estructura_mensaje;

// Create a struct_message called myData
estructura_mensaje misDatos;

esp_now_peer_info_t infoCompanero;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nEstatus de último mensaje:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Mensaje entregado" : "Fallo al entregar mensaje");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al inicializar ESP NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(infoCompanero.peer_addr, MAC_Receptor, 6);
  infoCompanero.channel = 0;  
  infoCompanero.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&infoCompanero) != ESP_OK){
    Serial.println("Fallo al agregar companero");
    return;
  }
}
 
void loop() {
  // Set values to send

  misDatos.mensaje = false;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(MAC_Receptor, (uint8_t *) &misDatos, sizeof(misDatos));
   
  if (result == ESP_OK) {
    Serial.println("Envíado correctamente");
  }
  else {
    Serial.println("Error al enviar datos");
  }
  delay(2000);
}
