#define SensorPin A0          
int temp;
void setup() {

  pinMode(12, OUTPUT); 
  pinMode(14, OUTPUT); 

  pinMode(LED_BUILTIN, OUTPUT);   
  Serial.begin(9600);  
  Serial.println("Ready");    
}

void loop() {
  digitalWrite(12, LOW);   
  digitalWrite(14, HIGH);
  delay(5000); 
  digitalWrite(12, HIGH);
  digitalWrite(14, LOW);  
  delay(5000);  

}
