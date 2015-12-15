//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
int coinnum = 0 ;
void setup()
{
  lcd.init();                      // initialize the lcd

  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Hello, world!");
  pinMode(3,INPUT);
 attachInterrupt(1, addcoin, RISING);
  //attachInterrupt(1, addcoin, CHANGE);
 // attachInterrupt(RFIDIntrupNumber, ReadRfidID, CHANGE);
}

void loop()
{
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
//  lcd.print(digitalRead(3));
   lcd.print(coinnum);
  Serial.println(coinnum);
  delay(200);
}

void addcoin()
{
    coinnum ++ ;
}

