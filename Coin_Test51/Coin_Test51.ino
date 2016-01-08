#include <DS1307RTC.h>
#include <Time.h>
#include <Ethernet.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define CoinMinute 1

#define RelayPin 8
#define turnon HIGH
#define turnoff LOW
boolean PowerStatus = false ;
//=======================

// Local Network Settings
byte mac[] = {
  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
};
IPAddress ip(192, 168, 2, 200);     //fix ip
IPAddress dnServer(168, 95, 1, 1);    //dns server ip
// the router's gateway address:
IPAddress gateway(192, 168, 2, 1);      //inet gateway ip
// the subnet:
IPAddress subnet(255, 255, 255, 0);

// ThingSpeak Settings
byte server[]  = { 184, 106, 153, 149 }; // IP Address for the ThingSpeak API
const int updateInterval = 2000;        // Time interval in milliseconds to update ThingSpeak
EthernetClient client ;


// Variable Setup
long lastConnectionTime = 0;
boolean lastConnected = false;
int resetCounter = 0;

// Initialize Arduino Ethernet Client
// ThingSpeak Settings
String writeAPIKey = "F86O9SXIVV6N3VGW ";    // Write API Key for a ThingSpeak Channel


//=================

//  ===========  use time variable
int UseSecond = 0 ;
int UseMinute = 0 ;

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display


int coinnum = 0 ;
tmElements_t tm;
int TotalCoin = 0 ;
int CoinTime = 0 ;

void setup()
{
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd

  // Print a message to the LCD.
  lcd.backlight();
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, turnoff) ;
  attachInterrupt(1, addcoin, RISING);
  //attachInterrupt(1, addcoin, CHANGE);
  // attachInterrupt(RFIDIntrupNumber, ReadRfidID, CHANGE);
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, dnServer, gateway, subnet);

  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

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
  LCDCoin(TotalCoin, CoinTime) ;
  CheckStatus() ;
  if (PowerStatus)
  {
    //        UseSecond = UseSecond + 15 ;
    UseSecond ++ ;
    CheckUseTIme() ;
  }
  delay(1000);
}

void addcoin()
{
  TotalCoin ++ ;
  CoinTime = CoinTime + CoinMinute ;
  updateThingSpeak("field1=" + StrDate() + "&field2=" + StrTime() + "&field3=" + "1" + "&field4=" + CoinMinute);
  Serial.println("Update thingspeak is ok");

}
String  print2digits(int number) {
  String ttt ;
  if (number >= 0 && number < 10)
  {
    ttt = String("0") + String(number);
  }
  else
  {
    ttt = String(number);
  }
  return ttt ;
}

String  print4digits(int number) {
  String ttt ;
  ttt = String(number);
  return ttt ;
}



String  StrDate() {
  String ttt ;
  ttt = print4digits(tmYearToCalendar(tm.Year)) + "/" + print2digits(tm.Month) + "/" + print2digits(tm.Day) ;
  return ttt ;
}

String  StrTime() {
  String ttt ;
  ttt = print2digits(tm.Hour) + ":" + print2digits(tm.Minute) + ":" + print2digits(tm.Second) ;
  return ttt ;
}

boolean  getDateTime()
{ if (RTC.read(tm))
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

void LCDCoin(int c1, int c2)
{
  lcd.setCursor(1, 3);
  lcd.print("Credit:");
  lcd.print(c1);
  lcd.print(" / ") ;
  lcd.print(c2);
  lcd.print(" min");

  Serial.println(coinnum);

}

void CheckStatus()
{
  if (!PowerStatus  && ( CoinTime > 0 ) )
  {
    // UseSecond ++ ;
    PowerStatus = true ;
    digitalWrite(RelayPin, turnon);
    Serial.println("Now Turn On Power") ;
  }
  if (PowerStatus  && ( CoinTime <= 0 ) )
  {
    CoinTime = 0 ;
    UseSecond = 0 ;
    TotalCoin = 0 ;
    PowerStatus = false ;
    digitalWrite(RelayPin, turnoff);
    Serial.println("Now Turn off Power") ;
  }

}

void CheckUseTIme()
{
  if (UseSecond >= 60)
  {
    CoinTime --;
    UseSecond = 0 ;
  }
}

//===============
void updateThingSpeak(String tsData)
{
  if (client.connect(server, 80))
  {
    Serial.println("Connected to ThingSpeak...");
    Serial.print("Data is :(");
    Serial.print(tsData);
    Serial.print(")\n");

    Serial.println();

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);

    lastConnectionTime = millis();

    resetCounter = 0;

  }
  else
  {
    Serial.println("Connection Failed.");
    Serial.println();

    resetCounter++;

    if (resetCounter >= 5 ) {
      resetEthernetShield();
    }

    lastConnectionTime = millis();
  }
}

void resetEthernetShield()
{
  Serial.println("Resetting Ethernet Shield.");
  Serial.println();

  client.stop();
  delay(1000);

  Ethernet.begin(mac, ip, gateway, subnet);
  delay(1000);
}


//===============


