#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// WiFi configuration
const char* ssid = "FunBox2-D32D";
const char* password = "aleksandra2";

// MQTT configuration
const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* mqtt_username = "";
const char* mqtt_password = "";
const char* mqtt_topic = "domoticz/in";

WiFiClient espClient;
PubSubClient client(espClient);

// Temperature variables
const int idx1 = 1;
char mqttbuffer[60];
float temperature1 = 0.0;

// Handle recieved MQTT message, just print it
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Reconnect to MQTT broker
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("broker")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  
  // Start the DS18B20 sensor
  sensors.begin();
  Serial.println("1-Wire library started");
  
  // Start WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Reconecting to wifi\n");
    delay(1000);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Setup MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  // Try to reconnect if connection is lost
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Get temperature
  sensors.requestTemperatures(); 
  temperature1 = sensors.getTempCByIndex(0);
  Serial.print("-------------------");
  Serial.print("Temperatura: ");
  Serial.print(temperature1);
  Serial.println("ºC");
  
  // Publish temperature
  sprintf(mqttbuffer, "{ \"idx\" : %d, \"nvalue\" : 0, \"svalue\" : \"%3.1f\" }", idx1, temperature1);
  Serial.print(mqttbuffer);
  Serial.print("\n");
  client.publish(mqtt_topic, mqttbuffer);
  
  // Delay
  delay(1000);
}
