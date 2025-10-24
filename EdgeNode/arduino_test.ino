const int pirPin = 7;

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200); // Pokreće serijsku komunikaciju na 115200 bps
  Serial.println("Sistem spreman, cekam pokret...");
}

void loop() {
  int motion = digitalRead(pirPin);

  if (motion) {
    digitalWrite(LED_BUILTIN, HIGH);   
    Serial.println("Pokret detektovan!");
  } 
  else {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Nema pokreta.");
  }

  delay(1000); // proverava svake sekunde
}
