
int SENSOR_TIME = 30000; //milliseconds

#include "_board.h"
#include "_ESP32-S2_Sleep.h"
#include "_wifi.h"
#include "_myApp.h"
#include "_mqtt.h"

int oldButtonState = 1; //high by default

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT); //buzzer
  Serial.begin(115200);
  while ((!Serial) && (millis() < 5000)) {
    ;
  }
  Serial.print("\nBenvenuti in ");
  Serial.println(myName);
  setup_wifi();
}

void loop() {
  mqttLoop();
  unsigned long now = millis();
  if (now - lastPeriodicMsg > SENSOR_TIME) {
    lastPeriodicMsg = now;
    bootCount++;
    print_wakeup_reason();
    Serial.print("\nMi sono svegliato ");
    loop_app();
    tone(BUZZER_PIN, frequency, duration);
    sendSensorData();

    if (TIME_TO_SLEEP) {
      Serial.print("\nMi sto addormentando ");
      disconnectMQTT();
      WiFi.disconnect();
      gotoSleep(TIME_TO_SLEEP);
    }
  }
  int b = digitalRead(BUTTON_PIN);
  if (b != oldButtonState) {
    oldButtonState = b;
    sendEventData(b);
    delay(100);
  }
}
