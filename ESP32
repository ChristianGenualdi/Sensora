#define ESP32 //Lolin s2 mini

int TIME_TO_SLEEP = 30; // seconds to sleep (only for ESP32-S2)

RTC_DATA_ATTR unsigned long bootCount = 0;
RTC_DATA_ATTR unsigned long eventCount = 0;

void gotoSleep(unsigned long ts) {
  //Wake for Touch:  from GPIO1 to GPIO14 (T1 T14) on ESP32-S2 just one pin
//#define BUTTON_PIN 12
  //touchSleepWakeUpEnable(BUTTON_PIN, TOUCH_WAKE_THRESHOLD);  //Setup sleep wakeup on Touch Pad

  //wake for single GPIO, RTC_IO
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_12, 0); //1 = High, 0 = Low  //one pin
  esp_sleep_enable_ext0_wakeup(BUTTON_PIN, HIGH);
  //If you were to use ext1, you would use it like //does noy work yet
  //#define BUTTON_PIN_BITMASK 0b100000000//2^9
  //  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH); //bits for multiple pins

  if (ts > 0) esp_sleep_enable_timer_wakeup(ts * 1E6); //wake by timer

  esp_deep_sleep_start();
}
/*
  Method to print the reason by which ESP32
  has been awaken from sleep
*/
int print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  Serial.print(wakeup_reason); Serial.print(" ");

  switch (wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break; //2
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break; //4
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;//5
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break; //0 external reset
  }
  return (wakeup_reason);
}

//redundant on ESP32-S2 since it can only be one pin
void print_wakeup_touchpad() {
  touch_pad_t BUTTON_PIN = esp_sleep_get_touchpad_wakeup_status();

  if (BUTTON_PIN < TOUCH_PAD_MAX)
  {
    Serial.printf("Touch detected on GPIO %d\n", BUTTON_PIN);
  }
  else
  {
    Serial.println("Wakeup not by touchpad");
  }
}
