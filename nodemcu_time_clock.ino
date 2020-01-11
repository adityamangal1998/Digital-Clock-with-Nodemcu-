#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <LiquidCrystal_I2C.h> //This library you can add via Include Library > Manage Library > 
#include <Firebase.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define FIREBASE_HOST "node-4f80b.firebaseio.com"                          // the project name address from firebase id
#define FIREBASE_AUTH "Am7q13MQ96Zzas0YiMFJ5XOvr0DSAQkSduNXfMVi"            // the secret key generated from firebase



///lcd 16*2 with i2c module
LiquidCrystal_I2C lcd(0x27, 16, 2);
const char* ssid = "Adityayo";
const char* password = "aditya123";
WiFiServer server(80);
const long utcOffsetInSeconds = 19800;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "in.pool.ntp.org", utcOffsetInSeconds);
String formattedDate;
String dayStamp;
String l;
String am_pm;
void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
  Wire.begin(2,0);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
  lcd.print(" Hello Creators "); // Start Printing
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  server.begin();
}
String a1;
int flag =0;
void loop() {
  timeClient.update();
  a1 = Firebase.getString("button3");
  int hour = timeClient.getHours();
  if (a1=="1")
      {
        lcd.backlight(); // Enable or Turn On the backlight
      }
      else if (a1=="0")
      {
        lcd.noBacklight(); // turn off backlight
      }
  if(timeClient.getHours() > 12 && timeClient.getHours() <24)
    {
      am_pm = "PM";
      hour = timeClient.getHours()-12;
    }
  else
  {
      am_pm = "AM";
      hour = timeClient.getHours();
  }
  
  
  l = String(hour)+":"+String(timeClient.getMinutes())+":"+String(timeClient.getSeconds())+" "+am_pm;
//  Serial.print(daysOfTheWeek[timeClient.getDay()]);
//  Serial.print(", ");
//  Serial.print(timeClient.getHours());
//  Serial.print(":");
//  Serial.print(timeClient.getMinutes());
//  Serial.print(":");
//  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());
  lcd.clear();
  lcd.print("Time "+l); // Start Printing
  lcd.setCursor(0,1);
  formattedDate = timeClient.getFormattedDate();
//  Serial.println(formattedDate);

  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
//  Serial.print("DATE: ");
//  Serial.println(dayStamp);
if (flag ==0)
{
  lcd.print("Date "+dayStamp);
  flag=1;
}
else if(flag==1)
  {
    lcd.print(daysOfTheWeek[timeClient.getDay()]);
    flag=0;
  }
  
  delay(1000);
  
}
