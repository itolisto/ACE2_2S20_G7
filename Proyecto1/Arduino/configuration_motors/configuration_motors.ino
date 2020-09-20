// Pines de motores
int Motor11 = 7;
int Motor12 = 6;
int Motor21 = 8;
int Motor22 = 9;
int PWMmotor1 = 5;
int PWMmotor2 = 10;
int valuePWM1 = 10; // Velocidad de motores cuando se mueven hacia el frente
int valuePWM2 = 0; // Velocidad de motores cuando giran

void setup() {
  pinMode(Motor11, OUTPUT);
  pinMode(Motor12, OUTPUT);
  pinMode(Motor21, OUTPUT);
  pinMode(Motor22, OUTPUT);
  pinMode(PWMmotor1, OUTPUT);
  pinMode(PWMmotor2, OUTPUT);
  // Configurar data rate en baud (bits por segundo)
  Serial.begin(9600);
}

void loop() {
  desplazarse(1);
}

void desplazarse(int orientacion) {
  bool detenerse = false;

  //   adelante
  //  digitalWrite(Motor11, LOW);
  //  digitalWrite(Motor12, HIGH);
  //  digitalWrite(Motor21, HIGH);
  //  digitalWrite(Motor22, LOW);

  // atras
  //  digitalWrite(Motor11, HIGH);
  //  digitalWrite(Motor12, LOW);
  //  digitalWrite(Motor21, LOW);
  //  digitalWrite(Motor22, HIGH);


  // Izquierda adelante
  //  digitalWrite(Motor11, LOW);
  //  digitalWrite(Motor12, LOW);
  //  digitalWrite(Motor21, HIGH);
  //  digitalWrite(Motor22, LOW);

  // Izquierda atras
  //  digitalWrite(Motor11, HIGH);
  //  digitalWrite(Motor12, LOW);
  //  digitalWrite(Motor21, LOW);
  //  digitalWrite(Motor22, LOW);



  // Derecha adelante
  //  digitalWrite(Motor11, LOW);
  //  digitalWrite(Motor12, HIGH);
  //  digitalWrite(Motor21, LOW);
  //  digitalWrite(Motor22, LOW);

  // Derecha atras
  //  digitalWrite(Motor11, LOW);
  //  digitalWrite(Motor12, LOW);
  //  digitalWrite(Motor21, LOW);
  //  digitalWrite(Motor22, HIGH);




  // Detenerse
  digitalWrite(Motor11, LOW);
  digitalWrite(Motor12, LOW);
  digitalWrite(Motor21, LOW);
  digitalWrite(Motor22, LOW);

  delay(500);

  return detenerse;
}
