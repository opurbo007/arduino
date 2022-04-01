//#include <SoftwareSerial.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include<dht11.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>


#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES  4

#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS

// SPI hardware interface
MD_Parola mx = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

LiquidCrystal_I2C lcd(0x27, 20, 4);
//SoftwareSerial bluetoothSerial(10,11);

#define dhtpin 8
dht11 dhtObject;
String readString;
String hoursMsg = "Hours Set:";

int buzzer = 7;

int inputHour;
int inputMin;


void setup() {
  //  bluetoothSerial.begin(9600);
  mx.begin();

  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");

}
void blue() {
  while (Serial.available() > 0) {
    delay(10);  //small delay to allow input buffer to fill
    char c = Serial.read();  //gets one byte from serial buffer
    if (c == ',') {
      break;
    }  //breaks out of capture loop to print readstring
    readString += c;
  } //makes the string readString
  if (readString.startsWith("hrs")) {
    readString.replace("hrs", "");
    inputHour = readString.toInt();
  }

  if (readString.startsWith("min")) {
    readString.replace("min", "");
    inputMin = readString.toInt();
  }
  if (readString.length() > 0) {
    Serial.println(readString); //prints string to serial port out
    //    printDisplay();
    readString = ""; //clears variable for new input
  }
}
void printDisplay() {
  //  String msg = hoursMsg + " " + inputHour + " | Minutes Set: " + inputMin;
  //  char msgBuf[msg.length() + 1];
  //  msg.toCharArray(msgBuf, msg.length());
  //  if (mx.displayAnimate()) {
  //    mx.displayScroll(msgBuf, PA_CENTER, PA_SCROLL_LEFT, 100);
  //    mx.displayReset();
  //  }
  mx.setTextAlignment(PA_CENTER);
  mx.print("DIU");

}
// for weather
void getTemperature()
{
  printDisplay();
  dhtObject.read(dhtpin);
  int temp = dhtObject.temperature;
  int hum = dhtObject.humidity;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.setCursor(12, 0);
  lcd.print(temp);
  lcd.setCursor(15, 0);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity:");
  lcd.setCursor(12, 1);
  lcd.print(hum);
  lcd.setCursor(15, 1);
  lcd.print("%");



}
void time() {
  tmElements_t tm;

  if (RTC.read(tm)) {
    //    Serial.print("Ok, Time = ");

    lcd.setCursor(0, 0);
    lcd.print("Time:");
    print2digits(tm.Hour);
    lcd.print(':');
    print2digits(tm.Minute);
    lcd.print(':');
    print2digits(tm.Second);
    //    Serial.print(", Date (D/M/Y) = ");
    lcd.setCursor(0, 1);
    lcd.print("Date:");
    lcd.print(tm.Day);
    lcd.print('/');
    lcd.print(tm.Month);
    lcd.print('/');
    lcd.print(tmYearToCalendar(tm.Year));

    //    lcd.println();
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
    }
    delay(9000);
  }
  delay(1000);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    lcd.print('0');
  }
  lcd.print(number);
}

void Buzz() {
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(5000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(500);        // ...for 1sec
}

void loop() {
  // put your main code here, to run repeatedly:
  getTemperature();
  delay(2000);
  lcd.clear();
  time();
  delay(2000);
  lcd.clear();
  tmElements_t tm;
  if (RTC.read(tm)) {
    if ((tm.Hour) == inputHour && (tm.Minute) == inputMin) {
      Buzz();
    }
  }
  blue();
}
