#include <WiFi.h>

String ssid = "Siam-ReteAule";
String password = "Khimeya2018";

void setup_wifi() {
  Serial.print("Connecting to " + String(ssid));
  delay(10);
  WiFi.mode(WIFI_STA);

  int t0 = millis();
  WiFi.begin(ssid.c_str(), password.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected in: " + String(millis() - t0) + " ms");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
