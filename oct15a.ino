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

// Init constants

// Init global variables

// Construct objects
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC
RTC_DS3231 rtc;
Servo myservo; 

void setup() {
  // init communication
  Serial.begin(9600);
  Wire.begin();

  // Init Hardware
  rtc.begin();
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object

  // Settings
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  u8g.setFont(u8g_font_unifont);
}

void loop() {
  
  oledWrite( getTime(), getTemp()); // writes the temp to the oled display
  servoWrite( getTemp() ); // turns the temperature to a reabable value for the servo and sets the servo to that value. 

  Serial.println(getTime());
  Serial.println(getTemp());
  delay(200);
}


/*
*This function reads time from an ds3231 module and package the time as a String
*Parameters: Void
*Returns: time in hh:mm:ss as String
*/
String getTime() {
  DateTime now = rtc.now();
  return String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
}

/*
* This function takes a string and draws it to an oled display
596061626364656667686970
*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(String text, float temp) {
  String mellantemp = String(temp);
  const char* nytemp = mellantemp.c_str();
  const char* nytext = text.c_str();


  u8g.firstPage();


*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(String text, float temp) {
  String mellantemp = String(temp);
  const char* nytemp = mellantemp.c_str();
  const char* nytext = text.c_str();


  u8g.firstPage();
  do {
    u8g.drawStr(0, 20, nytext);
    u8g.drawStr(0, 50, nytemp);
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
*Returns: temprature as float
*/
float getTemp() {
  float temp = rtc.getTemperature();
  return temp;
}