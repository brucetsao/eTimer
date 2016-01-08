#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup() {
  Serial.begin(9600);
    lcd.init();                      // initialize the lcd

  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Hello, world!");

  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");
}

void loop() {
  tmElements_t tm;

  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the Set-Time");
      Serial.println("example to initialize the time and begin run-ning.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the cir-cuitry.");
      Serial.println();
    }
    delay(9000);
  }
    // 將游標設到 　第一行, 　第二列
  // (注意: 　第二列第五行，因為是從 0 開始數起):
  lcd.setCursor(5, 1);
  // 列印 Arduino 重開之後經過的秒數
  lcd.print(millis() / 1000);
  lcd.setCursor(6, 2);
  // 列印 Arduino 重開之後經過的秒數
  lcd.print(millis() / 1000);
  lcd.setCursor(7, 3);
  // 列印 Arduino 重開之後經過的秒數
  lcd.print(millis() / 1000);
  Serial.println(millis() / 1000);

  delay(1000);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

