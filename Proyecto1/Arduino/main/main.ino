#include <HX711.h>

const int magneticPin = 2;

const int trigPin = 4;
const int echoPin = 5;

const int waterPumPin = 7;

const int DOUT = A1;
const int CLK = A0;
const int Peso_conocido = 401; //401 gramos

HX711 balanza;

void setup() {
  // Configurar data rate en baud (bits por segundo)
  Serial.begin(115200);
  // Configurar switch magnético
  pinMode(magneticPin, INPUT);
  // Configurar sensor de distancia
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Configurar switch de bomba de agua
  pinMode(waterPumPin, OUTPUT);
  // Configurar sensor de peso
  balanza.begin(DOUT, CLK);
  Calibrar();
}

void loop() {
  //------------------------------------------------------
  // Obtener distancia de niquel de liquido
  //------------------------------------------------------
  // Limpiar trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Enviar HIGH a trigPin durante 10 microsegundos
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Leer echoPin, devuelve el tiempo que tomo la onda de sonida regresar en microsegundos
  int duration = pulseIn(echoPin, HIGH);
  // Calcular la distancia
  int waterDistance = duration * 0.034 / 2;
  // Imprimir valor en consola
  // Serial.println(waterDistance);

  //------------------------------------------------------
  // Obtener peso de balanza
  //------------------------------------------------------
  float peso = balanza.get_units(10);

  if (peso < 0) {
    peso = peso * -1;
  }

  //------------------------------------------------------
  // Acciones
  //------------------------------------------------------
  // 1. Si el switch está HIGH, la puerta está cerrada
  if (digitalRead(magneticPin) == HIGH) {
    // Imprimir valor en consola
    bool rociar = true;
    // 2. Si hay peso, hay paquete
    if (peso > 20) {
      String chain = "";
      // 3. Rociar
      if (waterDistance < 4) {
        chain = "#1#" + String(peso, 1) + String("#alto#");
      } else if (waterDistance >= 4 && waterDistance < 7) {
        chain = "#1#" + String(peso, 1) + String("#medio#");
      } else if (waterDistance >= 7 && waterDistance < 11) {
        chain = "#1#" + String(peso, 1) + String("#bajo#");
      } else {
        chain = "#1#" + String(peso, 1) + String("#no_hay#");
        rociar = false;
      }

      int str_len = chain.length() + 1;
      char CharString[str_len];
      chain.toCharArray(CharString, str_len);
      Serial.write(CharString);
      if (rociar) {
        digitalWrite(waterPumPin, HIGH);
        // Esperar y rociar por 1 segundo, luego apagar la bomba
        delay(1000);
        digitalWrite(waterPumPin, LOW);
      }
    } else {
      Serial.write("#0#");
    }
  }

  delay(2000);
}

void Calibrar(void)
{
  float escala;
  float medicion = 0;
  balanza.set_scale(); // La escala por defecto es 1
  balanza.tare(20);  //20 mediciones

  Serial.println("5 segundos para colocar peso");
  delay(5000);

  for (int i = 0; i < 3; i++) {
    Serial.println("Midiendo");
    medicion = medicion + balanza.get_value(20); // 20 mediciones
    delay(1000);
  }

  Serial.println("Retirar peso");

  for (int i = 0; i < 3; i++) {
    delay(1000);
  }

  Serial.println("Ya deberia estar retirado");

  escala = (medicion / 3) / Peso_conocido;
  balanza.set_scale(escala);
  delay(500);
}
