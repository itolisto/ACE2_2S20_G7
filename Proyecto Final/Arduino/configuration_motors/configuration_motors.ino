// Pines de motores
int Motor11 = 8;
int Motor12 = 9;
int Motor21 = 10;
int Motor22 = 11;
int PWMmotor1 = 14;
int PWMmotor2 = 15;
int valuePWM1 = 100; // Velocidad de motores cuando se mueven hacia el frente
int valuePWM2 = 0; // Velocidad de motores cuando giran

void setup() {
  // Configurar data rate en baud (bits por segundo)
  Serial.begin(9600);
  pinMode(Motor11, OUTPUT);
  pinMode(Motor12, OUTPUT);
  pinMode(Motor21, OUTPUT);
  pinMode(Motor22, OUTPUT);
  //pinMode(PWMmotor1, OUTPUT);
  //pinMode(PWMmotor2, OUTPUT);
}

void loop() {
  desplazarse(1);
}

void desplazarse(int orientacion) {
  bool detenerse = false;

  //   adelante
  //  digitalWrite(Motor11, HIGH);
  //  digitalWrite(Motor12, LOW);
  //  digitalWrite(Motor21, HIGH);
  //  digitalWrite(Motor22, LOW);

  // atras
  //  digitalWrite(Motor11, LOW);
  //  digitalWrite(Motor12, HIGH);
  //  digitalWrite(Motor21, LOW);
  //  digitalWrite(Motor22, HIGH);

  // Izquierda adelante x
  //  digitalWrite(Motor11, HIGH);
  //  digitalWrite(Motor12, LOW);
  //  digitalWrite(Motor21, LOW);
  //  digitalWrite(Motor22, LOW);

  // Izquierda atras
  //  digitalWrite(Motor11, LOW);
  //  digitalWrite(Motor12, LOW);
  //  digitalWrite(Motor21, LOW);
  //  digitalWrite(Motor22, HIGH);

  // Derecha adelante x
  //  digitalWrite(Motor11, LOW);
  //  digitalWrite(Motor12, LOW);
  //  digitalWrite(Motor21, HIGH);
  //  digitalWrite(Motor22, LOW);

  // Derecha atras x
  //  digitalWrite(Motor11, LOW);
  //  digitalWrite(Motor12, HIGH);
  //  digitalWrite(Motor21, LOW);
  //  digitalWrite(Motor22, LOW);

  // Detenerse
  digitalWrite(Motor11, LOW);
  digitalWrite(Motor12, LOW);
  digitalWrite(Motor21, LOW);
  digitalWrite(Motor22, LOW);

  //  analogWrite(PWMmotor1, valuePWM1);
  //  analogWrite(PWMmotor2, valuePWM1);
  delay(500);

  return detenerse;
}
