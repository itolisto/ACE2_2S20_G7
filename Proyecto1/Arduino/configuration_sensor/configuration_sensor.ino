// Pines de sensores de color MH
const int FRONT2PIN = A2; // Izquierda
const int FRONT1PIN = A3; // Derecha
const int BACK1PIN = A4; // Izquierda
const int BACK2PIN = A5; // Derecha

void setup() {
  Serial.begin(9600);
  // Configurar sensores de color
  pinMode(FRONT1PIN, INPUT); // Sensor frontal derecho
  pinMode(FRONT2PIN, INPUT); // Sensor frontal izquierdo
  pinMode(BACK1PIN, INPUT); // Sensor trasero derecho
  pinMode(BACK2PIN, INPUT); // Sensor trasero izquierdo
}

void loop() {
  int FLEFT_SENSOR = analogRead(FRONT2PIN);
  int FRIGHT_SENSOR = analogRead(FRONT1PIN);

  int BLEFT_SENSOR = analogRead(BACK1PIN);
  int BRIGHT_SENSOR = analogRead(BACK2PIN);

  Serial.println(String(FLEFT_SENSOR) + String(",") + String(FRIGHT_SENSOR) + String("--") + String(BLEFT_SENSOR) + String(",") + String(BRIGHT_SENSOR));
}
