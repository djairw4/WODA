#define SensorPin A0          
int temp;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);     //pin drugi jako wyjście
  Serial.begin(9600);  
  Serial.println("Ready");    //Test the serial monitor
}
void loop()
{

  temp=analogRead(SensorPin);
  //float phValue=(float)(temp-50)*5.0/662+4.0;
  float phValue=(float)(temp-412)*2.0/300+7.0;  
  Serial.print("    goly odczyt:");  
  Serial.print(temp);
  Serial.println(" ");
  Serial.print("    pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");
  digitalWrite(LED_BUILTIN, LOW);   //zapalamy diodę
  delay(1000);            //czekamy sekundę
  digitalWrite(LED_BUILTIN, HIGH);  //gasimy diodę 
}
