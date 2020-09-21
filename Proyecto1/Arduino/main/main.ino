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
const int FRONT2PIN = A2; // Izquierda
const int FRONT1PIN = A3; // Derecha
const int BACK1PIN = A4; // Izquierda
const int BACK2PIN = A5; // Derecha

const int frontLeftLimit = 480;
const int frontRightLimit = 480;
const int backLeftLimit = 400;
const int backRightLimit = 400;
// Pines de motores
#define Motor11 8
#define Motor12 9
#define Motor21 10
#define Motor22 11
int valuePWM1 = 140; // Velocidad de motores cuando se mueven hacia el frente
int valuePWM2 = 110; // Velocidad de motores cuando giran
// Variables de configuración
bool activo = false; // Si es falso no ejecuta ninguna accion, de lo contrario, camina.
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
  // Configurar pines de motor
  pinMode(Motor11, OUTPUT);
  pinMode(Motor12, OUTPUT);
  pinMode(Motor21, OUTPUT);
  pinMode(Motor22, OUTPUT);
  // Configurar sensor de peso
  balanza.begin(DOUTPIN, CLKPIN);
  balanza.set_scale(); // La escala por defecto es 1
  balanza.tare(20);  // Peso muerto de la balanza
  balanza.get_value(20);
  delay(250);
  balanza.set_scale(-379.56027);
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
    activo = true;
    delay(10000);
    return;
  }

  // 3. Verificar si hay algun obstaculo que impida el movimiento del carro
  float distanciaAObstaculo = distanciaObstaculo(orientacion);

  if (orientacion == 1) {
    Serial.println("Obstaculo ida: " + String(distanciaAObstaculo, 2));
  } else {
    Serial.println("Obstaculo regreso: " + String(distanciaAObstaculo, 2));
  }

  if (distanciaAObstaculo < 7) {
    // Actualizar solamente si hubo un cambio de estado
    if (obstaculo_detectado == false) {
      // Forzar los motores a parar
      digitalWrite(Motor11, LOW);
      digitalWrite(Motor12, LOW);
      digitalWrite(Motor21, LOW);
      digitalWrite(Motor22, LOW);
      // Subir contador
      sumarObstaculos(orientacion);
      // Actualizar estado
      obstaculo_detectado = true;
      // Publicar nuevo estado
      publicarEnConsola("#update#obstaculo#");
    }
    // Impedir cualquier movimiento por medio de retorno
    delay(1000);
    return;
  } else if (obstaculo_detectado) {
    // Publicar nuevo estado
    if (orientacion == 1) {
      publicarEnConsola("#update#ida#");
    } else {
      publicarEnConsola("#update#retorno#");
    }
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
      Serial.println("Peso ida: " + String(peso_actual, 2));
      if (peso_actual > 20) {
        // Enviar notificacion
        publicarEnConsola("#salida#" + String(peso_actual, 1) + String("#"));
        // Actualizar
        mover = true;
      }
    }

    if (mover || yaSeMovio) {
      yaSeMovio = true;
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
      peso_actual = obtenerPeso();
      Serial.println("Peso regreso: " + String(peso_actual, 2));
      if (peso_actual < 20) {
        // Enviar notificacion
        publicarEnConsola("#entrega#" + String(peso_actual, 1) + String("#") + String(numero_obstaculos_ida) + String("#"));
        // Actualizar
        mover = true;
      }
    }

    if (mover || yaSeMovio) {
      yaSeMovio = true;
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

  bool detenerse = false;

  if (RIGHT_SENSOR < RIGHT_LIMIT && LEFT_SENSOR < LEFT_LIMIT) {
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
  } else if (RIGHT_SENSOR > RIGHT_LIMIT && LEFT_SENSOR < LEFT_LIMIT) {
    // Derecha
    if (orientacion == 1) {
      digitalWrite(Motor11, LOW);
      digitalWrite(Motor12, LOW);
      digitalWrite(Motor21, HIGH);
      digitalWrite(Motor22, LOW);
    } else {
      digitalWrite(Motor11, LOW);
      digitalWrite(Motor12, HIGH);
      digitalWrite(Motor21, LOW);
      digitalWrite(Motor22, LOW);
    }
  } else if (RIGHT_SENSOR < RIGHT_LIMIT && LEFT_SENSOR > LEFT_LIMIT) {
    // Izquierda
    if (orientacion == 1) {
      digitalWrite(Motor11, HIGH);
      digitalWrite(Motor12, LOW);
      digitalWrite(Motor21, LOW);
      digitalWrite(Motor22, LOW);
    } else {
      digitalWrite(Motor11, LOW);
      digitalWrite(Motor12, LOW);
      digitalWrite(Motor21, HIGH);
      digitalWrite(Motor22, LOW);
    }
  } else if (RIGHT_SENSOR > RIGHT_LIMIT && LEFT_SENSOR > LEFT_LIMIT) {
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
