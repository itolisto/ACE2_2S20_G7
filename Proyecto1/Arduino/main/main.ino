#include <HX711.h>

// Pines de sensor HC-SR04
const int TRIG1PIN = 2;
const int ECHO1PIN = 3;
const int TRIG2PIN = 4;
const int ECHO2PIN = 5;
// Pines de sensor de peso
const int CLKPIN = A0;
const int DOUTPIN = A1;
const int PESO_CONOCIDO = 401; // Gramos
HX711 balanza;
// Pines de sensores de color MH
const int FRONT1PIN = A2;
const int FRONT2PIN = A3;
const int BACK1PIN = A4;
const int BACK2PIN = A5;
// Pines de motores
#define Motor11 7
#define Motor12 6
#define Motor21 9
#define Motor22 8
#define PWMmotor1 5
#define PWMmotor2 10
int valuePWM1 = 140; // Velocidad de motores cuando se mueven hacia el frente
int valuePWM2 = 110; // Velocidad de motores cuando giran
// Variables de configuración
bool activo = true; // Si es falso no ejecuta ninguna accion, de lo contrario, camina.
bool obstaculo_detectado = false; // Si es falso, permite seguir el desplazamiento del carro
bool yaSeMovio = false;

int orientacion = 1; // 1: Hacia adelante (ida) 2: Hacia atras (regreso)
int numero_obstaculos_ida = 0; // Cuenta los obstaculos encontrados en la ida
int numero_obstaculos_regreso = 0; // Cuenta los obstaculos encontrados en el regreso

float peso_actual = 0;

void setup() {
  // Configurar data rate en baud (bits por segundo)
  Serial.begin(115200);
  // Configurar sensores de obstaculos
  pinMode(TRIG1PIN, OUTPUT);
  pinMode(ECHO1PIN, INPUT);
  pinMode(TRIG2PIN, OUTPUT);
  pinMode(ECHO2PIN, INPUT);
  // Configurar sensores de color
  pinMode(FRONT1PIN, INPUT); // Sensor frontal derecho
  pinMode(FRONT2PIN, INPUT); // Sensor frontal izquierdo
  pinMode(BACK1PIN, INPUT); // Sensor trasero derecho
  pinMode(BACK2PIN, INPUT); // Sensor trasero izquierdo
  // Configurar sensor de peso
  balanza.begin(DOUTPIN, CLKPIN);
  calibrarBalanza();
}

void loop() {
  // 1. Leer consola serial para saber si debemos cambiar el estado de activo
  if (Serial.available()) {
    String valorActivo = "";
    String datos = Serial.readString();

    for (int i = 0; i < datos.length(); i++) {
      if (i == 0) {
        // Verificar que inicie en el formato que deseamos, sino salir de este ciclo
        if (datos[i] == '=') {
          continue;
        } else {
          break;
        }
      }

      // Si volvemos a encontrar = significa el final de un valor
      if (datos[i] == '=') {
        // Verificar valor de lectura
        if (valorActivo == "falso") {
          activo = false;
        } else {
          activo = true;
        }
      } else {
        // Concatenar valor
        valorActivo += datos[i];
      }
    }
  }

  // 2. Verificar estado de respuesta actual del carro, si es falso omitir cualquier accion
  if (activo == false) {
    delay(1000);
    return;
  }

  // 3. Verificar si hay algun obstaculo que impida el movimiento del carro
  float distanciaAObstaculo = distanciaObstaculo(orientacion);

  if (distanciaAObstaculo < 15) {
    // Actualizar solamente si hubo un cambio de estado
    if (obstaculo_detectado == false) {
      // Subir contador
      sumarObstaculos(orientacion);
      // Actualizar estado
      obstaculo_detectado = true;
      // Publicar nuevo estado
      publicarEnConsola("#obstaculo#");
    }
    // Impedir cualquier movimiento por medio de retorno
    delay(1000);
    return;
  } else if (obstaculo_detectado) {
    // Actualizar estado solamente si el anterior era verdadero
    obstaculo_detectado = false;
  }

  //------------------------------------------------------
  // Acciones
  //------------------------------------------------------
  // 4. Verificar orientacion actual
  if (orientacion == 1) {
    // Si hay peso, empezar movimiento
    bool mover = false; // Variable temporal para detectar primer movimiento

    if (yaSeMovio == false) {
      peso_actual = obtenerPeso();
      if (peso_actual > 20) {
        // Enviar notificacion
        publicarEnConsola("#salida#" + String(peso_actual, 1) + String("#"));
        // Actualizar
        mover = true;
      }
    }

    if (mover || yaSeMovio) {
      yaSeMovio = true;
      // Publicar nuevo estado
      publicarEnConsola("#ida#");
      // Desplazarse
      bool detenerse = desplazarse(orientacion);
      // Si se activa detener, cambiar orientacion
      if (detenerse) {
        orientacion = 2;
        yaSeMovio = false;
      }
    }
  } else {
    // Si hay peso, empezar movimiento
    bool mover = false; // Variable temporal para detectar primer movimiento

    if (yaSeMovio == false) {
      if (obtenerPeso() < 20) {
        // Enviar notificacion
        publicarEnConsola("#entrega#" + String(peso_actual, 1) + String("#") + String(numero_obstaculos_ida) + String("#"));
        // Actualizar
        mover = true;
      }
    }

    if (mover || yaSeMovio) {
      yaSeMovio = true;
      // Publicar nuevo estado
      publicarEnConsola("#retorno#");
      // Desplazarse
      bool detenerse = desplazarse(orientacion);
      // Si se activa detener, cambiar orientacion
      if (detenerse) {
        // Enviar notificacion
        publicarEnConsola("#regreso#" + String(numero_obstaculos_regreso) + String("#"));
        // Resetear todo
        orientacion = 1;
        yaSeMovio = false;
        peso_actual = 0;
        numero_obstaculos_ida = 0;
        numero_obstaculos_regreso = 0;
      }
    }
  }
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

void sumarObstaculos(int orientacion) {
  // Basado en la orientacion, sumar a contador
  if (orientacion == 1) {
    numero_obstaculos_ida = numero_obstaculos_ida + 1;
  } else {
    numero_obstaculos_regreso = numero_obstaculos_regreso + 1;
  }
}

float obtenerPeso() {
  float peso = balanza.get_units(10);

  if (peso < 0) {
    peso = peso * -1;
  }

  return peso;
}

bool desplazarse(int orientacion) {
  int LEFT_SENSOR = 0;
  int RIGHT_SENSOR = 0;

  if (orientacion == 1) {
    LEFT_SENSOR = analogRead(FRONT1PIN);
    RIGHT_SENSOR = analogRead(FRONT2PIN);
  } else {
    LEFT_SENSOR = analogRead(BACK2PIN);
    RIGHT_SENSOR = analogRead(BACK1PIN);
  }

  bool detenerse = false;

  if (RIGHT_SENSOR < 36 && LEFT_SENSOR < 36) {
    // Adelante
    if (orientacion == 1) {
      digitalWrite(Motor11, HIGH);
      digitalWrite(Motor12, LOW);
      digitalWrite(Motor21, HIGH);
      digitalWrite(Motor22, LOW);
    } else {
      digitalWrite(Motor11, LOW);
      digitalWrite(Motor12, HIGH);
      digitalWrite(Motor21, LOW);
      digitalWrite(Motor22, HIGH);
    }
    analogWrite(PWMmotor1, valuePWM1);
    analogWrite(PWMmotor2, valuePWM1);
  } else if (RIGHT_SENSOR > 36 && LEFT_SENSOR < 36) {
    // Izquierda
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

    analogWrite(PWMmotor1, valuePWM2);
    analogWrite(PWMmotor2, valuePWM2);
  } else if (RIGHT_SENSOR < 36 && LEFT_SENSOR > 35) {
    // Derecha
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

    analogWrite(PWMmotor1, valuePWM2);
    analogWrite(PWMmotor2, valuePWM2);
  } else if (RIGHT_SENSOR > 35 && LEFT_SENSOR > 35) {
    // Detenerse
    detenerse = true;
    digitalWrite(Motor11, LOW);
    digitalWrite(Motor12, LOW);
    digitalWrite(Motor21, LOW);
    digitalWrite(Motor22, LOW);
  }

  return detenerse;
}

void publicarEnConsola(String cadena) {
  int str_len = cadena.length() + 1;
  char CharString[str_len];
  cadena.toCharArray(CharString, str_len);
  Serial.write(CharString);
}

void calibrarBalanza(void)
{
  float medicion = 0;
  balanza.set_scale(); // La escala por defecto es 1
  balanza.tare(20);  // Peso muerto de la balanza

  Serial.println("7 segundos para colocar peso");
  delay(7000);
  // Calibrar al hacer promedio de tres mediciones
  for (int i = 0; i < 3; i++) {
    Serial.println("Midiendo");
    medicion = medicion + balanza.get_value(20); // Mediciones antes de regresar peso
    delay(1000);
  }
  // Configurar
  Serial.println("4 segundos para retirar peso");
  delay(4000);
  Serial.println("Peso deberia estar retirado para este momento");
  float escala = (medicion / 3) / PESO_CONOCIDO;
  balanza.set_scale(escala);
  delay(500);
  Serial.println("Balanza configurada");
}
