// Pines de sensor HC-SR04
const int TRIG1PIN = 2;
const int ECHO1PIN = 3;
const int TRIG2PIN = 4;
const int ECHO2PIN = 5;

void setup() {
  // Configurar data rate en baud (bits por segundo)
  Serial.begin(115200);
  // Configurar sensores de obstaculos
  pinMode(TRIG1PIN, OUTPUT);
  pinMode(ECHO1PIN, INPUT);
  pinMode(TRIG2PIN, OUTPUT);
  pinMode(ECHO2PIN, INPUT);
}

void loop() {
  Serial.println("Frente: " + String(distanciaObstaculo(1), 2));
  Serial.println("Detras: " + String(distanciaObstaculo(2), 2));
  delay(4000);
}

float distanciaObstaculo(int orientacion) {
  // Basado en la orientacion, obtener sensor que debemos utilizar
  int trigPin;
  int echoPin;

  if (orientacion == 1) {
    trigPin = TRIG1PIN;
    echoPin = ECHO1PIN;
  } else {
    trigPin = TRIG2PIN;
    echoPin = ECHO2PIN;
  }

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
  return duration * 0.034 / 2;
}
