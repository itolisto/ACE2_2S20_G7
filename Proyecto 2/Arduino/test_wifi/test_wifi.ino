void setup() {
  // Configurar data rate en baud (bits por segundo)
  Serial.begin(115200);
}

void loop() {
  Serial.write("#regreso#25#");
  delay(5000);
}
