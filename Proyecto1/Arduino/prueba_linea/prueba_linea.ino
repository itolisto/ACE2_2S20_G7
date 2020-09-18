// Pines de sensores de color MH
const int FRONT2PIN = A2; // Izquierda
const int FRONT1PIN = A3; // Derecha
const int BACK1PIN = A4; // Izquierda
const int BACK2PIN = A5; // Derecha
// Pines de motores
#define Motor11 7
#define Motor12 6
#define Motor21 9
#define Motor22 8
#define PWMmotor1 5
#define PWMmotor2 10
int valuePWM1 = 140; // Velocidad de motores cuando se mueven hacia el frente
int valuePWM2 = 110; // Velocidad de motores cuando giran

bool activo = false;
int orientacion = 1;

void setup() {
  // Configurar data rate en baud (bits por segundo)
  Serial.begin(115200);
  // Configurar pines de motor
  pinMode(Motor11, OUTPUT);
  pinMode(Motor12, OUTPUT);
  pinMode(Motor21, OUTPUT);
  pinMode(Motor22, OUTPUT);
  pinMode(PWMmotor1, OUTPUT);
  pinMode(PWMmotor2, OUTPUT);
  // Configurar sensores de color
  pinMode(FRONT1PIN, INPUT); // Sensor frontal derecho
  pinMode(FRONT2PIN, INPUT); // Sensor frontal izquierdo
  pinMode(BACK1PIN, INPUT); // Sensor trasero derecho
  pinMode(BACK2PIN, INPUT); // Sensor trasero izquierdo
}

void loop() {
  // 1. Leer consola serial para saber si debemos cambiar el estado de activo
  // 2. Verificar estado de respuesta actual del carro, si es falso omitir cualquier accion
  if(activo == false) {
    activo = true;
    Serial.println("Activando...");
    delay(10000);
    Serial.println("Activo");
    return;
  }
  //------------------------------------------------------
  // Acciones
  //------------------------------------------------------
  // 4. Verificar orientacion actual
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

  if (orientacion == 1) {
    LEFT_SENSOR = analogRead(FRONT2PIN);
    RIGHT_SENSOR = analogRead(FRONT1PIN);
    Serial.println("Front: " + String(LEFT_SENSOR) + String(",") + String(RIGHT_SENSOR));
  } else {
    LEFT_SENSOR = analogRead(BACK1PIN);
    RIGHT_SENSOR = analogRead(BACK2PIN);
    Serial.println("Back: " + String(LEFT_SENSOR) + String(",") + String(RIGHT_SENSOR));
  }

  

  bool detenerse = false;

  int base_limit = 200;

  if (RIGHT_SENSOR < base_limit && LEFT_SENSOR < base_limit) {
    // Adelante
    if (orientacion == 1) {
      digitalWrite(Motor11, HIGH);
      digitalWrite(Motor12, LOW);
      digitalWrite(Motor21, LOW);
      digitalWrite(Motor22, HIGH);
    } else {
      digitalWrite(Motor11, LOW);
      digitalWrite(Motor12, HIGH);
      digitalWrite(Motor21, HIGH);
      digitalWrite(Motor22, LOW);
    }
    analogWrite(PWMmotor1, valuePWM1);
    analogWrite(PWMmotor2, valuePWM1);
  } else if (RIGHT_SENSOR > base_limit && LEFT_SENSOR < base_limit) {
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
  } else if (RIGHT_SENSOR < base_limit && LEFT_SENSOR > base_limit) {
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
  } else if (RIGHT_SENSOR > base_limit && LEFT_SENSOR > base_limit) {
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
