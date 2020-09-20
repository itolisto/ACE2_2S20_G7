// Pines de sensores de color MH
const int FRONT2PIN = A2; // Izquierda
const int FRONT1PIN = A3; // Derecha
const int BACK1PIN = A4; // Izquierda
const int BACK2PIN = A5; // Derecha

const int frontLeftLimit = 480;
const int frontRightLimit = 480;
const int backLeftLimit = 400;
const int backRightLimit = 400;
// Pines de motores
#define Motor11 7
#define Motor12 6
#define Motor21 8
#define Motor22 9
#define PWMmotor1 5
#define PWMmotor2 10
int valuePWM1 = 140; // Velocidad de motores cuando se mueven hacia el frente
int valuePWM2 = 110; // Velocidad de motores cuando giran
// Variables de configuración
bool activo = false; // Si es falso no ejecuta ninguna accion, de lo contrario, camina.

int orientacion = 1; // 1: Hacia adelante (ida) 2: Hacia atras (regreso)

void setup() {
  // Configurar data rate en baud (bits por segundo)
  Serial.begin(115200);
  // Configurar sensores de color
  pinMode(FRONT1PIN, INPUT); // Sensor frontal derecho
  pinMode(FRONT2PIN, INPUT); // Sensor frontal izquierdo
  pinMode(BACK1PIN, INPUT); // Sensor trasero derecho
  pinMode(BACK2PIN, INPUT); // Sensor trasero izquierdo
  // Configurar pines de motor
  pinMode(Motor11, OUTPUT);
  pinMode(Motor12, OUTPUT);
  pinMode(Motor21, OUTPUT);
  pinMode(Motor22, OUTPUT);
  pinMode(PWMmotor1, OUTPUT);
  pinMode(PWMmotor2, OUTPUT);
}

void loop() {
  if (activo == false) {
    activo = true;
    delay(10000);
    return;
  }

  if (orientacion == 1) {
    // Desplazarse
    bool detenerse = desplazarse(orientacion);
    // Si se activa detener, cambiar orientacion
    if (detenerse) {
      orientacion = 2;
    }
  } else {
    // Desplazarse
    bool detenerse = desplazarse(orientacion);
    // Si se activa detener, cambiar orientacion
    if (detenerse) {
      // Resetear todo
      orientacion = 1;
    }
  }
}

bool desplazarse(int orientacion) {
  int LEFT_SENSOR = 0;
  int RIGHT_SENSOR = 0;
  int LEFT_LIMIT = 0;
  int RIGHT_LIMIT = 0;

  if (orientacion == 1) {
    LEFT_SENSOR = analogRead(FRONT2PIN);
    RIGHT_SENSOR = analogRead(FRONT1PIN);
    LEFT_LIMIT = frontLeftLimit;
    RIGHT_LIMIT = frontRightLimit;
  } else {
    LEFT_SENSOR = analogRead(BACK1PIN);
    RIGHT_SENSOR = analogRead(BACK2PIN);
    LEFT_LIMIT = backLeftLimit;
    RIGHT_LIMIT = backRightLimit;
  }

  Serial.println(String(LEFT_SENSOR) + String(",") + String(RIGHT_SENSOR));

  bool detenerse = false;

  if (RIGHT_SENSOR < RIGHT_LIMIT && LEFT_SENSOR < LEFT_LIMIT ) {
    // Adelante
    if (orientacion == 1) {
      digitalWrite(Motor11, LOW);
      digitalWrite(Motor12, HIGH);
      digitalWrite(Motor21, HIGH);
      digitalWrite(Motor22, LOW);
    } else {
      digitalWrite(Motor11, HIGH);
      digitalWrite(Motor12, LOW);
      digitalWrite(Motor21, LOW);
      digitalWrite(Motor22, HIGH);
    }
    analogWrite(PWMmotor1, valuePWM1);
    analogWrite(PWMmotor2, valuePWM1);
  } else if (RIGHT_SENSOR > RIGHT_LIMIT && LEFT_SENSOR < LEFT_LIMIT) {
    // Derecha
    if (orientacion == 1) {
      digitalWrite(Motor11, LOW);
      digitalWrite(Motor12, HIGH);
      digitalWrite(Motor21, LOW);
      digitalWrite(Motor22, LOW);
    } else {
      digitalWrite(Motor11, LOW);
      digitalWrite(Motor12, LOW);
      digitalWrite(Motor21, LOW);
      digitalWrite(Motor22, HIGH);
    }

    analogWrite(PWMmotor1, valuePWM2);
    analogWrite(PWMmotor2, valuePWM2);
  } else if (RIGHT_SENSOR < RIGHT_LIMIT && LEFT_SENSOR > LEFT_LIMIT) {
    // Izquierda
    if (orientacion == 1) {
      digitalWrite(Motor11, LOW);
      digitalWrite(Motor12, LOW);
      digitalWrite(Motor21, HIGH);
      digitalWrite(Motor22, LOW);
    } else {
      digitalWrite(Motor11, HIGH);
      digitalWrite(Motor12, LOW);
      digitalWrite(Motor21, LOW);
      digitalWrite(Motor22, LOW);
    }

    analogWrite(PWMmotor1, valuePWM2);
    analogWrite(PWMmotor2, valuePWM2);
  } else if (RIGHT_SENSOR > RIGHT_LIMIT && LEFT_SENSOR > LEFT_LIMIT) {
    // Detenerse
    detenerse = true;
    digitalWrite(Motor11, LOW);
    digitalWrite(Motor12, LOW);
    digitalWrite(Motor21, LOW);
    digitalWrite(Motor22, LOW);
    analogWrite(PWMmotor1, 0);
    analogWrite(PWMmotor2, 0);
  }

  return detenerse;
}
