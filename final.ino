/*
* Name: övningsprojekt
* Author: Elias Christensen
* Date: 2025-10-9
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display, 
* Further, it measures temprature and displays a mapped value to a 9g-servo-motor
*/

// Include Libraries
#include <RTClib.h>
#include <Wire.h>
#include <U8glib.h>
#include <Servo.h>
#include "LedControl.h"

// Init constants
const int radius = 31;
const int centerX = 80;
const int centerY = 32;

// Init global variables

// Construct objects
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC
RTC_DS3231 rtc;
Servo myservo; 
LedControl lc=LedControl(12,11,10,1);

void setup() {
  // init communication
  Serial.begin(9600);
  Wire.begin();

  // Init Hardware
  rtc.begin();
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);

  // Settings
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  u8g.setFont(u8g_font_unifont);
}

void loop() {
  oledDraw(getSecond(), getTemp(), getMinute(), getHours());  // draws the seconds as a clock and the temperature as a box
  servoWrite(getTemp()); //writes the temp to the servo
  segmentWrite(getTime());
  Serial.println(getTime());
}

/*
*This function reads time from an ds3231 module and package the time as a String
*Parameters: Void
*Returns: time in hh:mm:ss as String
*/
String getTime() {
  DateTime now = rtc.now();
  return String(now.hour()) + "-" + String(now.minute()) + "-" + String(now.second());
}

/*
*This function reads the seconds from the ds3231 and packages the seconds as a integer.
*Parameters: Void
*Returns: Seconds as an integer
*/
int getSecond() {
  DateTime now = rtc.now();
  int second = now.second();
  return second;
}

int getMinute() {
  DateTime now = rtc.now();
  int second = now.minute();
  return second;
}

int getHours() {
  DateTime now = rtc.now();
  int second = now.hour();
  return second;
}

/*
* This function takes a int and draws a clock based on the seconds. 
*Parameters: - text: String to write to display
*Returns: void
*/
void oledDraw(int seconds, float temp, int minute, int hours) {
  int lineRadius = radius - 5;
  int newtemp = map(temp, 15, 30, 0, 64);

  float angle_rad = (minute*6 - 90) * M_PI / 180.0;  // 0° points up
  int minx = centerX + cos(angle_rad) * lineRadius;
  int miny = centerY + sin(angle_rad) * lineRadius;

  angle_rad = (seconds*6 - 90) * M_PI / 180.0;  // 0° points up
  int secx = centerX + cos(angle_rad) * lineRadius;
  int secy = centerY + sin(angle_rad) * lineRadius;

  lineRadius = lineRadius - 10;

  angle_rad = (hours*30 - 90) * M_PI / 180.0;  // 0° points up
  int hourx = centerX + cos(angle_rad) * lineRadius;
  int houry = centerY + sin(angle_rad) * lineRadius;


  u8g.firstPage();
  do {
    u8g.drawLine(centerX, centerY, secx, secy);
    u8g.drawLine(centerX, centerY, minx, miny);
    u8g.drawLine(centerX, centerY, hourx, houry);
    u8g.drawCircle(centerX, centerY, radius);
    u8g.drawBox(0, 0, 5, newtemp);
    u8g.drawStr(0, 55, "Temp");
  } while (u8g.nextPage());
}


/*
* takes a temprature value and maps it to corresppnding degree on a servo
*Parameters: - value: temprature
*Returns: void
*/
void servoWrite(float value) {
  int newvalue = map(value, 15, 30, 0, 180);
  myservo.write(newvalue);
}

/*
* This function takes temprature from ds3231 and returns as a float
*Parameters: Void
*Returns: temperature as float
*/
float getTemp() {
  float temp = rtc.getTemperature();
  return temp;
}

/*
*This function writes the time to a seven segment display
*/
void segmentWrite(String time) {
  for (int i = 0; i <= 7; i++){
    lc.setChar(0,i, time[7-i], false);
  }
}
