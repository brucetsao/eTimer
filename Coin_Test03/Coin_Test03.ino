#include <DS1307RTC.h>
#include <Time.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define CoinMinute 3 ;

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display


int coinnum = 0 ;
  tmElements_t tm;
  int TotalCoin = 0 ;
  int CoinTime = 0 ;
  
void setup()
{
  lcd.init();                      // initialize the lcd

  // Print a message to the LCD.
  lcd.backlight();
  //pinMode(3,INPUT);
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
  getDateTime() ;
  LCDDateTime() ;
 LCDCoin(TotalCoin,CoinTime) ;
  delay(200);
}

void addcoin()
{
    TotalCoin ++ ;
    CoinTime = CoinTime + CoinMinute
}
String  print2digits(int number) {
  String ttt ;
  if (number >= 0 && number < 10) 
  {
     ttt =String("0")+String(number);
  }
  else
  {
     ttt =String(number);
  }
   return ttt ;
}

String  print4digits(int number) {
  String ttt ;
     ttt =String(number);
   return ttt ;
}



String  StrDate() {
  String ttt ;
    ttt = print4digits(tmYearToCalendar(tm.Year))+"/"+print2digits(tm.Month)+"/"+print2digits(tm.Day) ;
   return ttt ;
}

String  StrTime() {
  String ttt ;
    ttt = print2digits(tm.Hour)+":"+print2digits(tm.Minute)+":"+print2digits(tm.Second) ;
   return ttt ;
}

boolean  getDateTime()
{   if (RTC.read(tm))
        return true ;
        else
        return false ;
}         

  void LCDDateTime()
  {
        lcd.setCursor(0, 0);
        lcd.print(StrDate());
        lcd.setCursor(11, 0);
        lcd.print(StrTime());
        
  }

void LCDCoin(int c1,int c2)
{
   lcd.setCursor(1, 3);
   lcd.print("Credit:");
   lcd.print(c1);
   lcd.print(" / ") ;
   lcd.print(c2);
   lcd.print(" min");
   
  Serial.println(coinnum);
 
}

