#include <WEMOS_SHT3X.h>
#define BUZZER 7
#define BATTERY_PIN 3
float batteryVoltage;
SHT3X sht30(0x45);

int frequency = 1000;
int duration = 500;
int alertTemperature = 30;

void setup_app() {
     analogSetPinAttenuation(BATTERY_PIN, ADC_11db);
}

void loop_app(){
   if (sht30.get() == 0) {
    Serial.print("Temperature in Celsius : ");
    Serial.println(sht30.cTemp);
    Serial.print("Relative Humidity : ");
    Serial.println(sht30.humidity);
    Serial.print("Battery : ");
    Serial.println(analogRead(3) * 2.3 / 1024.0);
    Serial.println();
  }
  else
  {
    Serial.println("Error!");
  }
  if (sht30.cTemp >= alertTemperature) {
    tone(BUZZER, frequency, duration);  //emette una nota di 1000Hz x 3000 ms
    delay(1000);              //Attende 1 secondi
  }
}
bool read_sensors() {
  return (true);
}
