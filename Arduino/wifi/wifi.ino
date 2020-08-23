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
  Serial.println("Connection begin");

  while (WiFi.status() != WL_CONNECTED) { // Esperar a la conexion WiFi
    delay(500);
    Serial.println("Waiting for connection");
  }
}

float peso = 0;
float temperatura = 0;
int aire;
int liquido;
int contador = 0;
float ultimo;

void loop() {
  // Verificar si estamos conectados
  if (WiFi.status() == WL_CONNECTED) {
    if (Serial.available()) {
      String datos = Serial.readString();
      int cont = 0;
      String val = "";
      for (int i = 0; i < datos.length(); i++) {
        if (datos[i] == '#') {
          switch (cont) {
            case 0: {
                peso = val.toInt();
                val = "";
                break;
              }
            case 1: {
                temperatura = stof(val);
                val = "";
                break;
              }
            case 2: {
                aire = val.toInt();
                val = "";
                break;
              }
            case 3: {
                liquido = val.toInt();
                if (liquido == 1) {
                  liquido = 0;
                } else {
                  liquido = 1;
                }
                val == "";
                break;
              }
            case 4: {
                contador = val.toInt();
                val = "";
                break;
              }
            case 5:
              ultimo = stof(val);
          }
          cont++;
        } else {
          val += datos[i];
        }
      }
      enviar(1, peso);
      enviar(2, temperatura);
      enviar(3, aire);
      enviar(4, liquido);
    }
  } else {
    Serial.println("Error en conexión WiFi");
  }
}

void enviar(int tipo, double valor) {
  // Declarar JSON buffer estático
  StaticJsonDocument<200> JSONbuffer;
  JSONbuffer["type"] = tipo;
  JSONbuffer["user"] = 5;
  JSONbuffer["value"] = valor;
  char JSONmessageBuffer[200];
  serializeJson(JSONbuffer, JSONmessageBuffer, 200);
  // Imprimir mensaje en consola para verificar
  Serial.println(JSONmessageBuffer);
  // Declarar objeto de la clase HTTPClient
  HTTPClient http;
  // Indicar ruta de servidor
  http.begin("http://134.209.4.117:3600/store");
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
