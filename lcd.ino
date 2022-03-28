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

int buzzer = 7;

int inputHour = 16;
int inputMin = 54;

//String text;
//char c;


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
//void blue() {
//
//  if (bluetoothSerial.available() > 0)
//  {
//    c = bluetoothSerial.read();
//
////    mx.print(c);
////    lcd.print(c);
//    Serial.print(c);
//  }
//
//}
//void display() {
//
//
// lcd.setCursor(0, 1);
//  lcd.print("hello");
//
//
//
//}
// for weather
void getTemperature()
{
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
      //      Serial.println();
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



void dotMat() {

  if (mx.displayAnimate()) {
    mx.displayText("Welcome to DIU", PA_CENTER, 30, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    mx.displayReset();
  }



}

void loop() {
  // put your main code here, to run repeatedly:
    //display();
     getTemperature();
        delay(3000);
     lcd.clear();

    time();
      delay(3000);
       lcd.clear();
    tmElements_t tm;
    if (RTC.read(tm)) {
      if ((tm.Hour) == inputHour && (tm.Minute) == inputMin) {
        Buzz();
      }
      Serial.print(tm.Hour);
      Serial.print(tm.Minute);
    }
  dotMat();
//   blue();

}
