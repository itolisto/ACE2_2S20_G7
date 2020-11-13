#include <ArduinoJson.h>

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "FamGoLeonGuest";
const char* password = "GroovesRushDebris57";

// Para correr programa en modulo ESP-32 agregar la siguiente URL para el manejador de boards
// https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
// luego herramientas, seleccionar el Board "ESP32 Dev Module"

void setup() {
  // Configurar data rate en baud (bits por segundo)
  Serial.begin(115200);
  // Configurar la informacion para conexion WiFi
  WiFi.begin(ssid, password);
  Serial.println("Comenzando conexión");

  while (WiFi.status() != WL_CONNECTED) { // Esperar a la conexion WiFi
    delay(500);
    Serial.println("Esperando a conexión");
  }

  Serial.println("Conectado");
}

void loop() {
  // Verificar si estamos conectados
  if (WiFi.status() == WL_CONNECTED) {
    if (Serial.available()) {
      String datos = Serial.readString();

      // Variable de control que va concatenando cada valor entre # y # (ej. #valor# donde valor es lo que contiene val)
      String val = "";
      // Variable que contiene la accion que se debe realizar
      String accion = "";
      // Variable de control que nos permite saber si ya se ha definido una accion
      bool accionDefinida = false;

      for (int i = 0; i < datos.length(); i++) {
        if (i == 0) {
          // Verificar que inicie en el formato que deseamos, sino salir de este ciclo
          if (datos[i] == '#') {
            continue;
          } else {
            break;
          }
        }

        // Si volvemos a encontrar # significa el final de un valor
        if (datos[i] == '#') {
          if (accionDefinida == false) {
            accion = val;
            accionDefinida = true;
          } else {
            if (accion == "send") {
              enviarTemperatura(stof(val));
            }
          }

          // Limpiar valor y comenzar de nuevo
          val = "";
        } else {
          // Concatenar valor
          val += datos[i];
        }
      }
    }
  } else {
    Serial.println("Error en conexión WiFi");
  }
}

void enviarTemperatura(float temperatura) {
  // Declarar JSON buffer estático
  StaticJsonDocument<30> JSONbuffer;
  JSONbuffer["temperatura"] = temperatura;
  char JSONmessageBuffer[30];
  serializeJson(JSONbuffer, JSONmessageBuffer, 30);
  // Imprimir mensaje en consola para verificar
  Serial.println(JSONmessageBuffer);
  // Declarar objeto de la clase HTTPClient
  HTTPClient http;
  // Indicar ruta de servidor
  http.begin("http://34.67.81.108:3600/safebox/llegada");
  http.addHeader("Content-Type", "application/json");
  // Enviar petición POST
  int httpCode = http.POST(JSONmessageBuffer);
  // Obtener respuesta e imprimir información
  String payload = http.getString();
  Serial.println(httpCode);
  Serial.println(payload);
  // Cerrar conexión
  http.end();
}

float stof(String Texto) {
  char cadena[Texto.length() + 1];
  Texto.toCharArray(cadena, Texto.length() + 1);
  float myFloat = atof(cadena);
  return myFloat;
}
