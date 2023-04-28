#include <PubSubClient.h>
#include <ArduinoJson.h>

String myName = "Sensora2";
String mqttDomain = "iort2023";

String mqtt_server = "172.16.206.91";

String mqtt_user = "mosquitto";
String mqtt_password = "dietpi";

String thingId = "DMO_" + String(WiFi.macAddress().c_str());

String mqttTopicPubish_sensor = mqttDomain + "/" + myName + "/sensor";
String mqttTopicPubish_event = mqttDomain + "/" + myName + "/event";

String mqttTopicSubscibe_buzzer = mqttDomain + "/" + myName + "/buzzer";
String mqttTopicSubscibe_config = mqttDomain + "/" + myName + "/config";

WiFiClient espClient;
PubSubClient mqttClient(espClient);
unsigned long lastPeriodicMsg = 0;

void sendSensorData() {
  DynamicJsonDocument jmsg(1024);
  jmsg["humidity"] = sht30.humidity;
  jmsg["temperature"] = sht30.cTemp;
  jmsg["Vbattery"] = analogRead(3) * 4.5 / 1024.0;//valori inviati il Json su mqtt
  jmsg["name"] = myName;
  jmsg["bootCount"] = bootCount;
  jmsg["eventCount"] = eventCount;
  String mqttPayload = "";
  serializeJson(jmsg, mqttPayload);
  Serial.println("Publish message: " + mqttTopicPubish_sensor + " " + mqttPayload);
  mqttClient.publish(mqttTopicPubish_sensor.c_str(), mqttPayload.c_str());
}

void sendEventData(int b) {
  DynamicJsonDocument jmsg(1024);
  jmsg["Vbattery"] = analogRead(3) * 2.3 / 1024.0;//valori inviati il Json su mqtt
  jmsg["name"] = myName;
  jmsg["eventCount"] = eventCount;
  String mqttPayload = String(b);
  serializeJson(jmsg, mqttPayload);
  Serial.println("Publish message: " + mqttTopicPubish_event + " " + mqttPayload);
  mqttClient.publish(mqttTopicPubish_event.c_str(), mqttPayload.c_str());
}

void mqttReceive(char* topic, byte* payload, unsigned int length) {
  payload[length] = 0;
  String Payload = String((char*)payload);
  String Topic = String(topic);
  Serial.println("Message arrived " + Topic + " " + Payload );

  if (Topic == mqttTopicSubscibe_config) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, Payload);
    //impostare i parametri della temperatura
    alertTemperature = doc["temperature"];
  }

  DynamicJsonDocument jmsg(1024);
  if (deserializeJson(jmsg, Payload) == DeserializationError::Ok) {
    if (Topic == mqttTopicSubscibe_buzzer) {
      int frequency = jmsg["frequency"];
      int duration = jmsg["duration"];
      frequency = constrain(frequency, 100, 10000);
      duration = constrain(frequency, 10, 5000);
      tone(BUZZER_PIN, frequency, duration);
    } else {
      Serial.println("invalid json");
    }
  }
}
  void mqttLoop() {
    if (!mqttClient.connected()) {
      Serial.print("Attempting MQTT connection...");
      String clientId = WiFi.macAddress().c_str();
      mqttClient.setServer(mqtt_server.c_str(), 1883);
      mqttClient.setCallback(mqttReceive);
      if (mqttClient.connect(clientId.c_str(), mqtt_user.c_str(), mqtt_password.c_str())) {
        Serial.println("mqtt connected");
        mqttClient.subscribe(mqttTopicSubscibe_buzzer.c_str());
        Serial.println("subscribed to:");
        Serial.println(mqttTopicSubscibe_buzzer);
        mqttClient.subscribe(mqttTopicSubscibe_config.c_str());
        Serial.println("subscribed to:");
        Serial.println(mqttTopicSubscibe_buzzer);
        lastPeriodicMsg = millis();
      } else {
        Serial.print("failed, code:");
        Serial.print(mqttClient.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
    }
    mqttClient.loop();
  }

  void disconnectMQTT() {
    mqttClient.disconnect();
  }
  
