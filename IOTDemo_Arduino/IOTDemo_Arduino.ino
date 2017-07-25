#include<Wire.h>
#include<LCD.h>
#include<LiquidCrystal_I2C.h>
#include<Timer.h>
#include<OneWire.h>
#include<DS18B20.h>
#include<RFID.h>
#include<SPI.h>
#define I2C_ADDR 0x27
Timer tick;
LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7);
DS18B20 dd(2);
RFID rfid(10,9);
float oldtemp;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myLcdSetup();
  mySwitchSetup();
  myLedSetup();
  tick.every(1500,myTemperature);
  myRfidSetup();
  tick.every(500,myRfid);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    String s = Serial.readString();
    if(s.startsWith("LCD")){
      myLcd(s.substring(3));
    }else{
      myLed(s);
    }
  }
  mySwitch();
  tick.update();
}
void myLcdSetup(){
  //Serial.println("myLcdSetup");
  lcd.begin(16,2);
  lcd.setBacklightPin(3,POSITIVE);
  lcd.backlight();
}
void myLcd(String s){
  //Serial.println("myLcd");
  
    lcd.clear();
    
    if(s.length()>32){
      lcd.setCursor(0,0);
      lcd.print("Error!More than");
      lcd.setCursor(0,1);
      lcd.print("32 words");
    }else if(s.length()>16){
      lcd.setCursor(0,0);
      lcd.print(s);
      lcd.setCursor(0,1);
      lcd.print(s.substring(16));
    }else{
      lcd.setCursor(0,0);
      lcd.print(s);
    }
  
}
void mySwitchSetup(){
  //Serial.println("mySwitchSetup");
  pinMode(7,INPUT_PULLUP);
}
int myswitch = LOW;//close
void mySwitch(){
  //Serial.println("mySwitch");
  int sensorValue = digitalRead(7);
  if(sensorValue != myswitch){//狀態改變
    if(sensorValue == LOW){
      Serial.println("close");
    }else{
      Serial.println("open");
    }
    myswitch = sensorValue;
  }
}
void myTemperature(){
  //Serial.println("myTemperature");
  
  float temp = dd.getTemperature();
  if(temp == oldtemp){
    return;
  }
  oldtemp = temp;
  Serial.print("temp");
  Serial.println(temp);
}
void myRfidSetup(){
  SPI.begin();
  rfid.init();
}
void myRfid(){
  if(rfid.isCard()){
    if(rfid.readCardSerial()){
      if(rfid.serNum[0] == 213 && rfid.serNum[1] == 211 &&
      rfid.serNum[2] == 227 && rfid.serNum[3] == 82 &&
      rfid.serNum[4] == 183){
        Serial.println("legitimate");
      }else{
        Serial.println("illegitimate");
      }
    }
  }
}
void myLedSetup(){
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(3,OUTPUT);
}
void myLed(String s){
    if(s == "greenon"){
      digitalWrite(6,HIGH);
    }else if(s == "greenoff"){
      digitalWrite(6,LOW);
    }else if(s == "yellowon"){
      digitalWrite(5,HIGH);
    }else if(s == "yellowoff"){
      digitalWrite(5,LOW);
    }else if(s == "redon"){
      digitalWrite(3,HIGH);
    }else if(s == "redoff"){
      digitalWrite(3,LOW);
    }
}
//void led(int i){
//  analogWrite(i, brightness);
//  brightness = brightness + fadeAmount;
//  if (brightness <= 0 || brightness >= 255) {
//    fadeAmount = -fadeAmount ;
//  }    
//
//  delay(delayDuration); 
//}

