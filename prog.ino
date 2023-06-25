#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "iPhone de Rafael";
const char* password = "pedrinviado";
const char* mqttServer = "rgrillao.duckdns.org";
const int mqttPort = 1883;
const char* mqttUser = "rgrillao";
const char* mqttPassword = "rgrillao";
const char* mqttTopicph = "esp32/ph";
const char* mqttTopicteordecloro = "esp32/teordecloro";

const int sensorPinph = 36; // Potenciômetro da ph
const int sensorPinteordecloro = 35; // Potenciômetro da cloro

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado ao WiFi com endereço IP: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Conectado ao servidor MQTT!");
    } else {
      Serial.print("Falha na conexão - Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  int sensorValueph = analogRead(sensorPinph);
  int sensorValueteordecloro = analogRead(sensorPinteordecloro);
//float ph = map(sensorValueph, 0, 100, 0, 14);
//float teordecloro = map(sensorValueteordecloro, 0, 100, 0, 28) / 100;
float ph=((float)sensorValueph/4095)*14;
float teordecloro=((float)sensorValueteordecloro/4095)*1;

  String phPayload = String(ph);
  String teordecloroPayload = String(teordecloro);

  Serial.print("Enviando dados para o Node-RED - ph: ");
  Serial.println(ph);
  client.publish(mqttTopicph, phPayload.c_str());

  Serial.print("Enviando dados para o Node-RED - teordecloro: ");
  Serial.println(teordecloro);
  client.publish(mqttTopicteordecloro, teordecloroPayload.c_str());

  delay(1000);
}
