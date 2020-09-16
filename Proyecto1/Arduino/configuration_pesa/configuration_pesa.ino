#include <HX711.h>

// Pines de sensor de peso
const int CLKPIN = A0;
const int DOUTPIN = A1;
const int PESO_CONOCIDO = 401; // Gramos
HX711 balanza;

float escala = 0.0;

void setup() {
  // Configurar data rate en baud (bits por segundo)
  Serial.begin(115200);
  // Configurar sensor de peso
  balanza.begin(DOUTPIN, CLKPIN);
  calibrarBalanza();
}

void loop() {
  float peso = balanza.get_units(10);

  if (peso < 0) {
    peso = peso * -1;
  }

  Serial.println("Escala: " + String(escala, 10) + String(", Peso: ") + String(peso, 10));
}

void calibrarBalanza() {
  // Configurar balanza
  //  float medicion = 0.0;
  //
  //  balanza.set_scale(); // La escala por defecto es 1
  //  balanza.tare(20);  // Peso muerto de la balanza
  //
  //  Serial.println("7 segundos para colocar peso");
  //  delay(7000);
  //  // Calibrar al hacer promedio de tres mediciones
  //  for (int i = 0; i < 3; i++) {
  //    Serial.println("Midiendo");
  //    medicion = medicion + balanza.get_value(20); // Mediciones antes de regresar peso
  //    delay(1000);
  //  }
  //  // Configurar
  //  Serial.println("4 segundos para retirar peso");
  //  delay(4000);
  //  Serial.println("Peso deberia estar retirado para este momento");
  //  escala = (medicion / 3) / PESO_CONOCIDO;
  //  balanza.set_scale(escala);
  //  delay(500);
  //  Serial.println("Balanza configurada");

  // Balanza ya configurada
  escala = -379.56027;
  balanza.set_scale(); // La escala por defecto es 1
  balanza.tare(20);  // Peso muerto de la balanza
  balanza.get_value(20);
  delay(250);
  balanza.set_scale(escala);
}
