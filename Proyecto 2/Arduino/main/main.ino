#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>

// Iniciar librerias para pantalla y sensor de temperatura
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Pines de indicador de resultado
const int RED_PIN = 3;
const int GREEN_PIN = 4;
// Pines de sensor HC-SR04
const int TRIG_PIN = 5;
const int ECHO_PIN = 6;
// Pin de bomba
const int PUM_PIN = 12;

// Variables de estado
bool isFirstLaunch = true;
bool isWaiting = false;

const int MEDICIONES = 3;

void setup() {
  // Configurar data rate en baud (bits por segundo)
  Serial.begin(115200);
  // Configurar pines de resultado
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  // Configurar sensores de obstaculos
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  // Configurar pantalla LCD
  //    Indicar a la libreria que tenemos conectada una pantalla de 16x2 e iniciar
  lcd.begin(16, 2);
  lcd.init ();
  //    Encender la luz de fondo.
  lcd.backlight();
  // Configurar sensor de temperatura
  mlx.begin();
  // Configurar pin de bomba de agua
  pinMode(PUM_PIN, OUTPUT);
}

void loop() {
  if (isFirstLaunch) {
    lcd.setCursor (0, 0);
    lcd.print("SafeBox");
    lcd.setCursor (0, 1);
    lcd.print("ARQ2 - Grupo #7");
    // Dejar mensaje por 7 seg
    delay(7000);
    // Limpiar variable y pantalla
    isFirstLaunch = false;
    lcd.clear();
    return;
  }

  float distanciaASujeto = medirDistanciaASujeto();

  if (distanciaASujeto < 20) {
    lcd.clear();
    lcd.setCursor (0, 0);
    lcd.print("Midiendo temp.");
    lcd.setCursor (0, 1);
    lcd.print("por favor espere");
    // Tomar varias mediciones y promediar
    float sumatoriaTemperatura = 0.0;

    for (int i = 0; i < MEDICIONES; i++) {
      sumatoriaTemperatura = sumatoriaTemperatura + medirTemperatura();
      delay(300);
    }

    float promedioTemperatura = sumatoriaTemperatura / MEDICIONES;

    lcd.clear();
    lcd.setCursor (0, 0);

    int colorPin = 0;
    bool accessoPermitido = true;

    if (promedioTemperatura <= 37.5) {
      lcd.print("Acceso concedido");
      colorPin = GREEN_PIN;
    } else {
      lcd.print("Acceso denegado");
      colorPin = RED_PIN;
      accessoPermitido = false;
    }

    // Mostrar temperatura
    lcd.setCursor (0, 1);
    lcd.print("Temp: " + String(promedioTemperatura, 2) + String("*C"));

    publicarEnConsola("#send#" + String(promedioTemperatura, 2) + "#");

    // Mostrar pin de color
    digitalWrite(colorPin, HIGH);
    delay(3000);
    digitalWrite(colorPin, LOW);

    // Dispensar agua
    if (accessoPermitido) {
      digitalWrite(PUM_PIN, HIGH);
      delay(1000);
      digitalWrite(PUM_PIN, LOW);
    }

    // Reiniciar estado de espera
    isWaiting = false;
  } else {
    if (!isWaiting) {
      isWaiting = true;
      lcd.clear();
      lcd.setCursor (0, 0);
      lcd.print("Esperando medir");
      lcd.setCursor (0, 1);
      lcd.print("temperatura...");
    }
  }
}

float medirDistanciaASujeto() {
  // Limpiar trigPin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // Enviar HIGH a trigPin durante 10 microsegundos
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Leer echoPin, devuelve el tiempo que tomo la onda de sonida regresar en microsegundos
  int duration = pulseIn(ECHO_PIN, HIGH);
  // Calcular la distancia
  return duration * 0.034 / 2;
}

float medirTemperatura() {
  return mlx.readObjectTempC();
}

void publicarEnConsola(String cadena) {
  int str_len = cadena.length() + 1;
  char CharString[str_len];
  cadena.toCharArray(CharString, str_len);
  Serial.write(CharString);
}
