#include <HX711.h>

const int dataPin=A1;
const int CLK=A0;
double peso;

const int trigPin = 9;
const int echoPin = 10;
long duration;
int distance;

const int switchPin = 2;

HX711 balanza;

void setup() {
  // Configurar data rate en baud (bits por segundo)
  Serial.begin(9600);
  // Configurar switch magnético
  pinMode(switchPin, INPUT);
  // Configurar sensor de distancia
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Configurar sensor de peso
  balanza.begin(DOUT, CLK);
  Serial.print("Lectura del valor del ADC:t");
  Serial.println(balanza.read());
  Serial.println("No ponga ningún objeto sobre la balanza");
  Serial.println("Destarando...");
  balanza.set_scale(); //La escala por defecto es 1
  balanza.tare(20);  //El peso actual es considerado Tara.
  Serial.println("Coloque un peso conocido:");
}

void loop() {
  //------------------------------------------------------
  // Obtener distancia de niquel de liquido
  //------------------------------------------------------
  // Limpiar trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  

  //------------------------------------------------------
  // Obtener peso de balanza
  //------------------------------------------------------
  Serial.print("Valor de lectura: t");
  Serial.println(balanza.get_value(10),0);
  delay(100);

  //------------------------------------------------------
  // Acciones
  //------------------------------------------------------

  // 1. Si el switch está LOW, la puerta está cerrada
  if (digitalRead(switchPin) == HIGH) {
    // 2. Si hay peso
    if (peso > 0.0) {
      
    }
  }
}
