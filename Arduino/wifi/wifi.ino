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
      int expectedRead = 0; // 0: Objeto 1: Peso 2: Liquido
      String val = "";

      int hayObjeto = 0;
      float peso = 0.0;
      String liquido = "no_hay";

      bool sendPost = true;

      for (int i = 0; i < datos.length(); i++) {
        if (i == 0) {
          // Verificar que inicie en el formato que deseamos, sino salir de este ciclo
          if (datos[i] == '#') {
            continue;
          } else {
            sendPost = false;
            break;
          }
        }

        // Si volvemos a encontrar # significa el final de un valor
        if (datos[i] == '#') {
          switch (expectedRead) {
            case 0: {
                hayObjeto = val.toInt();
                break;
              }
            case 1: {
                peso = stof(val);
                break;
              }
            case 2: {
                liquido = val;
                break;
              }
            default:
              break;
          }

          // Limpiar valor y comenzar de nuevo
          val = "";
          expectedRead++;
        } else {
          // Concatenar valor
          val += datos[i];
        }
      }

      if (sendPost) {
        enviar(hayObjeto, peso, liquido);
      }
    }
  } else {
    Serial.println("Error en conexión WiFi");
  }
}

void enviar(int hayObjeto, float peso, String liquido) {
  // Declarar JSON buffer estático
  StaticJsonDocument<80> JSONbuffer;

  JSONbuffer["existe"] = hayObjeto;
  JSONbuffer["peso"] = peso;

  char JSONmessageBuffer[80];
  serializeJson(JSONbuffer, JSONmessageBuffer, 80);



  StaticJsonDocument<50> JSONbuffer2;
  JSONbuffer2["nivel"] = liquido;
  char JSONmessageBuffer2[50];
  serializeJson(JSONbuffer2, JSONmessageBuffer2, 50);


  // Imprimir mensaje en consola para verificar
  Serial.println(JSONmessageBuffer);
  Serial.println(JSONmessageBuffer2);

  // Declarar objeto de la clase HTTPClient
  HTTPClient http;
  // Indicar ruta de servidor
  http.begin("http://167.99.237.132:3600/bitacora");
  http.addHeader("Content-Type", "application/json");
  // Enviar petición POST
  int httpCode = http.POST(JSONmessageBuffer);
  // Obtener respuesta e imprimir información
  // String payload = http.getString();
  // Serial.println(httpCode);
  // Serial.println(payload);
  // Cerrar conexión
  http.end();

  HTTPClient http2;
  http2.begin("http://167.99.237.132:3600/liquido");
  http2.addHeader("Content-Type", "application/json");
  http2.POST(JSONmessageBuffer2);
  http2.end();
}

float stof(String Texto) {
  char cadena[Texto.length() + 1];
  Texto.toCharArray(cadena, Texto.length() + 1);
  float myFloat = atof(cadena);
  return myFloat;
}
