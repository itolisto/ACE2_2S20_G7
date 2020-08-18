#include <HX711.h>

const int magneticPin = 2;

const int trigPin = 4;
const int echoPin = 5;

const int waterPumPin = 7;

const int dataPin = A1;
const int CLK = A0;
double peso;

HX711 balanza;

void setup() {
  // Configurar switch magnético
  pinMode(magneticPin, INPUT);
  // Configurar sensor de distancia
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Configurar switch de bomba de agua
  pinMode(waterPumPin, OUTPUT);

  // Configurar sensor de peso
  //  balanza.begin(DOUT, CLK);
  //  Serial.print("Lectura del valor del ADC:t");
  //  Serial.println(balanza.read());
  //  Serial.println("No ponga ningún objeto sobre la balanza");
  //  Serial.println("Destarando...");
  //  balanza.set_scale(); //La escala por defecto es 1
  //  balanza.tare(20);  //El peso actual es considerado Tara.
  //  Serial.println("Coloque un peso conocido:");

  // Configurar data rate en baud (bits por segundo)
  Serial.begin(9600);
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
  int peso = 0;
  //  Serial.print("Valor de lectura: t");
  //  Serial.println(balanza.get_value(10),0);
  //  delay(100);

  //------------------------------------------------------
  // Acciones
  //------------------------------------------------------
  // 1. Si el switch está HIGH, la puerta está cerrada
  if (digitalRead(magneticPin) == HIGH) {
    Serial.println("Cerrada");
    // 2. Si hay peso, hay paquete
    if (peso > 0) {
      // 3. Rociar (If separado por si acaso necesitamos hacer algo distinto dependiendo del nivel de liquido)
      if (waterDistance < 10) {
        // Alto
        digitalWrite(waterPumPin, HIGH);
        // Esperar y rociar por 1 segundo, luego apagar la bomba
        delay(10000);
        digitalWrite(waterPumPin, LOW);
      } else if (waterDistance >= 10 && waterDistance < 20) {
        // Media
        digitalWrite(waterPumPin, HIGH);
        // Esperar y rociar por 1 segundo, luego apagar la bomba
        delay(10000);
        digitalWrite(waterPumPin, LOW);
      } else if (waterDistance >= 30 && waterDistance < 30) {
        // Bajo
        digitalWrite(waterPumPin, HIGH);
        // Esperar y rociar por 1 segundo, luego apagar la bomba
        delay(10000);
        digitalWrite(waterPumPin, LOW);
      } else {
        // No hay liquido, no hacer nada
        digitalWrite(waterPumPin, LOW);
      }
    }
  }
  //  else {
  //     Serial.println("Abierto");
  //  }

  delay(500);
}
