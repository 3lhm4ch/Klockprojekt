// Include library
#include "RTClib.h"
#include <Servo.h>

// Make object
RTC_DS3231 rtc;
Servo myservo;

void setup() {
  Serial.begin(9600);

  // Servo är på pin 9
  myservo.attach(9);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

}

void loop() {
  // Lägger tiden i variabeln "now"
  DateTime now = rtc.now();

  // Skriver ut sekunder, minuter, timmar, dagar, månader och år
  int second = now.second();
  Serial.println(second);
  servsec(now.second());
  // Delay på 1 sek
  delay(1000);
}

void servsec(int second){
  int mapsec = map(second, 0, 60, 0, 180);
  Serial.println(mapsec);
  myservo.write(80);
}