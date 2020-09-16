void setup() {
  Serial.begin(9600);
 
  pinMode(A0, INPUT); // initialize Right sensor as an inut
}
 
void loop() {
 
  int SENSOR = analogRead(A0);
 
  Serial.println("Sensor:");
  Serial.println(SENSOR);
  
  delay(200);  
}
