#define SensorPin 34
int Signal;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  while (!Serial);
  pinMode(SensorPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Signal = analogRead(SensorPin);
  Serial.print(Signal);
}
