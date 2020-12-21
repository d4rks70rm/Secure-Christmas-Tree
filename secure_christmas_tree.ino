#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <DS3231.h>

#define red_led 2
#define blue_led 6
#define yellow_led 5
#define green_led 3
#define white_led 4

#define RST_PIN   8
#define SS_PIN    53

String RFID = "0D E6 15 A3";

MFRC522 mfrc522(SS_PIN, RST_PIN);

LiquidCrystal lcd(27, 28, 29, 30, 31, 32);

DS3231 clock;
RTCDateTime dt;

int christmasDay = 25;
int christmasMonth = 12;

bool readRFID(){
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return false;
  }
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     //Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  //Serial.println();
  //Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == RFID) //change here the UID of the card/cards that you want to give access
  {
    return true;
  }
 else{
    return false;
  }
}

void calculateDate(){
  dt = clock.getDateTime();
  int currentDay = dt.day;
  int currentMonth = dt.month; 
  if ( currentMonth == 12){
    if ( currentDay < 25 ){
      int days = christmasDay - currentDay;
      lcd.clear();      
      lcd.print("Christmas Tree");
      lcd.setCursor(0, 1);
      lcd.print(days);
      lcd.print(" days to xmas");
    }else{
      lcd.clear();      
      lcd.setCursor(0, 0);
      lcd.print("Xmas as passed");
      lcd.setCursor(0, 1);
      lcd.print("return next year");
    }
  }else{
     lcd.clear();      
      lcd.print("Christmas Tree");
      lcd.setCursor(0, 1);
      lcd.print("Start at Dec");      
    }
}

void setup() {
  pinMode(red_led, OUTPUT);
  pinMode(blue_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(white_led, OUTPUT);

  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.begin(16, 2);
  lcd.print("Christmas Tree");
  clock.begin();
  //clock.setDateTime(__DATE__, __TIME__);
}

void loop() {
  if( readRFID() ){
    lcd.setCursor(0, 1);
    lcd.print("Authorized access");
    while (true){
      calculateDate();
      int randNumber1 = random(2,7);
      int randNumber2 = random(2,7);
      while(randNumber2 == randNumber1){
        randNumber2 = random(2,6);
      }
      digitalWrite(randNumber1,HIGH);
      digitalWrite(randNumber2,HIGH);
      delay(500);
      digitalWrite(randNumber1,LOW);
      digitalWrite(randNumber2,LOW);
    }
  }else{
    lcd.setCursor(0, 1);
    lcd.print("Access denied");
    delay(1000);    
  }
}
