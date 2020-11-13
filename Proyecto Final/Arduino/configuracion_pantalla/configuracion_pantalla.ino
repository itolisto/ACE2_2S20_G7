#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  Serial.begin(115200);
  // Indicar a la libreria que tenemos conectada una pantalla de 16x2
  // lcd.begin(16, 2);
  // Mover el cursor a la primera posición de la pantalla (0, 0)
  lcd.init ();
  //Encender la luz de fondo.
  lcd.backlight();
  // Mover el cursor a la segunda linea (1) primera columna
  lcd.setCursor ( 0, 0 );
  // Imprimir "Hola Mundo" en la primera linea
  lcd.print("Hola Mundo");
  // Mover el cursor a la segunda linea (1) primera columna
  lcd.setCursor ( 0, 1 );
  // Imprimir otra cadena en esta posicion
  lcd.print("GEEKFACTORYSABC");
  // Esperar un segundo
  delay(1000);
}

void loop() {
  // EN EL CICLO PRINCIPAL SOLAMENTE RECORREMOS EL MENSAJE DE UN LADO A OTRO
  // Variable para conteo de ciclos
  int i;

  // Desplazar la pantalla a la derecha 2 veces
  for ( int i = 0; i < 5; i++ ) {
    lcd.scrollDisplayRight();
    delay (1000);
  }

  // Desplazar la pantalla a la izquierda 2 veces
  for ( int i = 0; i < 5; i++ ) {
    lcd.scrollDisplayLeft();
    delay (1000);
  }
}
