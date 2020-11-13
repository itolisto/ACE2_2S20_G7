#include <Wire.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  Serial.println("Prueba sensor de temperatura GY-906");
  mlx.begin();
}

void loop() {
  Serial.print("Ambiente = "); Serial.print(mlx.readAmbientTempC());
  Serial.print("*C\tObjeto = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.print("Ambiente = "); Serial.print(mlx.readAmbientTempF());
  Serial.print("*F\tObjeto = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");
  Serial.println();
  delay(500);
}
