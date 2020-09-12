#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "FamGoLeonGuest";
const char* password = "GroovesRushDebris57";

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
      String val = "";

      String accion = "";
      int expectedRead = 0;

      float peso = 0.0;

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
            if (accion == "update") {
              enviarUpdate(val);
              // Salir de ciclo, ya no nos interesa nada mas
              break;
            } else if (accion == "salida") {
              enviarSalida(stof(val));
              // Salir de ciclo, ya no nos interesa nada mas
              break;
            } else if (accion == "entrega") {
              switch (expectedRead) {
                case 1: {
                    peso = stof(val);
                    break;
                  }
                default: {
                    enviarEntrega(peso, val.toInt());
                    break;
                  }
              }
            } else if (accion == "regreso") {
              enviarRegreso(val.toInt());
            }
          }

          // Limpiar valor y comenzar de nuevo
          val = "";
          expectedRead++;
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

void enviarUpdate(String estado) {
  // Declarar JSON buffer estático
  StaticJsonDocument<30> JSONbuffer;
  JSONbuffer["estado"] = estado;
  char JSONmessageBuffer[30];
  serializeJson(JSONbuffer, JSONmessageBuffer, 30);
  // Imprimir mensaje en consola para verificar
  Serial.println(JSONmessageBuffer);
  // Declarar objeto de la clase HTTPClient
  HTTPClient http;
  // Indicar ruta de servidor
  http.begin("http://167.99.237.132:3600/vehiculo/progreso");
  http.addHeader("Content-Type", "application/json");
  // Enviar petición POST
  int httpCode = http.POST(JSONmessageBuffer);
  // Obtener respuesta e imprimir información
  // String payload = http.getString();
  // Serial.println(httpCode);
  // Serial.println(payload);
  // Cerrar conexión
  http.end();
}

void enviarSalida(float peso) {
  StaticJsonDocument<30> JSONbuffer;
  JSONbuffer["peso"] = peso;
  char JSONmessageBuffer[30];
  serializeJson(JSONbuffer, JSONmessageBuffer, 30);
  Serial.println(JSONmessageBuffer);
  HTTPClient http;
  http.begin("http://167.99.237.132:3600/envio/salida");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(JSONmessageBuffer);
  http.end();
}

void enviarRegreso(int obstaculos) {
  StaticJsonDocument<25> JSONbuffer;
  JSONbuffer["obstaculos"] = obstaculos;
  char JSONmessageBuffer[25];
  serializeJson(JSONbuffer, JSONmessageBuffer, 25);
  Serial.println(JSONmessageBuffer);
  HTTPClient http;
  http.begin("http://167.99.237.132:3600/envio/regreso");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(JSONmessageBuffer);
  http.end();
}

void enviarEntrega(float peso, int obstaculos) {
  StaticJsonDocument<65> JSONbuffer;
  JSONbuffer["peso"] = peso;
  JSONbuffer["obstaculos"] = obstaculos;
  char JSONmessageBuffer[65];
  serializeJson(JSONbuffer, JSONmessageBuffer, 65);
  Serial.println(JSONmessageBuffer);
  HTTPClient http;
  http.begin("http://167.99.237.132:3600/envio/entrega");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(JSONmessageBuffer);
  http.end();
}

float stof(String Texto) {
  char cadena[Texto.length() + 1];
  Texto.toCharArray(cadena, Texto.length() + 1);
  float myFloat = atof(cadena);
  return myFloat;
}
