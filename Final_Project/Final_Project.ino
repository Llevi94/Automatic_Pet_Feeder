// Name: Liron Levi, ID: *********

//************libraries**************//
#include <Wire.h>
#include <RTClib.h>
#include "DFRobot_LCD.h"
#include <LiquidCrystal_I2C.h>
#include <DFRobot_LCD.h>
#include <Servo.h>

//************************************//
LiquidCrystal_I2C lcd (0x27, 16, 2);
RTC_DS1307 RTC;
Servo myservo;
#define lightsensorpin A0


//************Button*****************//
int P1=6; // Button SET MENU'
int P2=7; // Button +
int P3=8; // Button -

//************Variables**************//
int hourupg; // now hour
int minupg; // now min
int hourfeed; // feed hour
int minfeed; // feed min
int menu=0;
const int debounce = 35; //time to stop
int pin_servo = 9; // pin in the arduino

void setup()
{
  myservo.attach(pin_servo);

  lcd.init();
  lcd.clear();

  pinMode(P1,INPUT); // btn menu
  pinMode(P2,INPUT); // btn plus
  pinMode(P3,INPUT); // btn minus

  Serial.begin(9600);
  Wire.begin();
  
  RTC.begin();
  myservo.write(180);
}
 
void loop()
{ 
  int analogValue = analogRead(lightsensorpin); // light sensor value

  Serial.print("Analog reading: ");
  Serial.print(analogValue);   // the raw analog reading
  Serial.print("\n");
  delay(100);


  
// set button
  if (menu==0)
    {
     DisplayDateTime(); // void DisplayDateTime
    }
  if (menu==1)
    {
      delay(100);
      DisplaySetHour();
    }
  if (menu==2)
    {
      delay(100);
      DisplaySetMinute();
    }
  if (menu==3)
    {
      delay(100);
      DisplaySetFeedingHour();
    }
  if (menu==4)
    {
      delay(100);
      DisplaySetFeedingMinute();
    }
  if (menu==5)
    {
      delay(100);
      StoreAgg(); 
      menu=0;
    }
    
    delay(50);

// need to change the analogValue by the place
  if ( ((hourupg == hourfeed) || ((hourupg-12) == hourfeed)) && (minupg == minfeed)&& (RTC.now().second()== 00) && (analogValue >= 300))
    {
      while(analogValue >= 300)
      {
        myservo.write(80); // at 0 - back to place
        delay(400);
        myservo.write(180);
        delay(1500);

        //read again
        analogValue = analogRead(lightsensorpin);

        Serial.print("Analog reading in loop: ");
        Serial.print(analogValue);   // the raw analog reading
        Serial.print("\n");
        delay(100);
      }
      
    } 

}

void DisplayDateTime ()
{
// We show the current date and time
  DateTime now = RTC.now();
  hourfeed = now.month();
  DateTime feed(0, 0, 0, hourfeed, minfeed,0); // RTC values
  
  if(digitalRead(P1)){ // if click on menu btn
    menu=1;
    delay(debounce); // stop
  }

  //*feed time*
  // set hour
  lcd.setCursor(0, 1); //feeding time
  lcd.print("Feed:");
  if (feed.hour() <=9)
  {
    lcd.print("0");
  }
  lcd.print(feed.hour(), DEC);
  hourfeed=hourfeed; // feed hour
  lcd.print(":");
  // set min
  if (feed.minute()<=9)
  {
    lcd.print("0");
  }
  lcd.print(feed.minute(), DEC);
  minfeed=minfeed; // feed min
  lcd.print(":");
  if(digitalRead(P1)){
   menu=1;
   delay(debounce);
  }
  // set sec
  if (feed.second()<=9)
  {
    lcd.print("0");
  }
  lcd.print(feed.second(), DEC);



// *real time*
  lcd.setCursor(0, 0); // now time
  lcd.print("Time:");
  if (now.hour()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.hour(), DEC);
  hourupg=now.hour();
  lcd.print(":");
  // set min
  if (now.minute()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.minute(), DEC);
  minupg=now.minute();
  lcd.print(":");
  if(digitalRead(P1)){
   menu=1;
   delay(debounce);
  }
  // set sec
  if (now.second()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.second(), DEC);

if(digitalRead(P1)){
   menu=1;
   delay(debounce);
}
}

void DisplaySetHour()
{
// time setting
  lcd.clear();
  DateTime now = RTC.now();
  
  lcd.setCursor(0,0);
  lcd.print("Set Hour:");
  lcd.setCursor(0,1);
  lcd.print(hourupg,DEC);
 
  while(!digitalRead(P1)){
  if(digitalRead(P2)==HIGH)
  {
    if(hourupg==23)
    {
      hourupg=0;
    }
    else
    {
      hourupg=hourupg+1;
    }
    delay(debounce);
  
  }
   if(digitalRead(P3)==HIGH)
  {
    if(hourupg==0)
    {
      hourupg=23;
    }
    else
    {
      hourupg=hourupg-1;
    }
    delay(debounce);
  }
  DateTime now = RTC.now();
  lcd.setCursor(0,0);
  lcd.print("Set Hour:");
  lcd.setCursor(0,1);
  lcd.print(hourupg,DEC);
  lcd.print(" ");
  delay(200);
  }
     menu=2;
     delay(debounce);
}

void DisplaySetMinute()
{
// Setting the minutes

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Set Minutes:");
  lcd.setCursor(0,1);
  lcd.print(minupg,DEC);


  while(!digitalRead(P1)){
    if(digitalRead(P2)==HIGH)
    {
      if (minupg==59)
      {
        minupg=0;
      }
      else
      {
        minupg=minupg+1;
      }
      delay(debounce);
    }
     if(digitalRead(P3)==HIGH)
    {
      if (minupg==0)
      {
        minupg=59;
      }
      else
      {
        minupg=minupg-1;
      }
      delay(debounce);
    }
  lcd.setCursor(0,0);
  lcd.print("Set Minutes:");
  lcd.setCursor(0,1);
  lcd.print(minupg,DEC);
  lcd.print(" ");
  delay(200);
  }
  menu=3;
  delay(debounce);
}

  
//feed
void DisplaySetFeedingHour()
{
// time setting
  lcd.clear();
  DateTime now = RTC.now();
  
  lcd.setCursor(0,0);
  lcd.print("Set Feeding Hour:");
  lcd.setCursor(0,1);
  lcd.print(hourfeed,DEC);
 
  while(!digitalRead(P1)){
  if(digitalRead(P2)==HIGH)
  {
    if(hourfeed==12)
    {
      hourfeed=1;
    }
    else
    {
      hourfeed=hourfeed+1;
    }
    delay(debounce);
  
  }
   if(digitalRead(P3)==HIGH)
  {
    if(hourfeed==1)
    {
      hourfeed=12;
    }
    else
    {
      hourfeed=hourfeed-1;
    }
    delay(debounce);
  }
  DateTime now = RTC.now();
  lcd.setCursor(0,0);
  lcd.print("Set Feeding Hour:");
  lcd.setCursor(0,1);
  lcd.print(hourfeed,DEC);
  lcd.print(" ");
  delay(200);
  }
     menu=4;
     delay(debounce);
}

void DisplaySetFeedingMinute()
{
// Setting the minutes

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Set Feeding Minutes:");
  lcd.setCursor(0,1);
  lcd.print(minfeed,DEC);


  while(!digitalRead(P1)){
    if(digitalRead(P2)==HIGH)
    {
      if (minfeed==59)
      {
        minfeed=0;
      }
      else
      {
        minfeed=minfeed+1;
      }
      delay(debounce);
    }
     if(digitalRead(P3)==HIGH)
    {
      if (minfeed==0)
      {
        minfeed=59;
      }
      else
      {
        minfeed=minfeed-1;
      }
      delay(debounce);
    }
  lcd.setCursor(0,0);
  lcd.print("Set Feeding Minutes:");
  lcd.setCursor(0,1);
  lcd.print(minfeed,DEC);
  lcd.print(" ");
  delay(200);
  }
  menu=5;
  delay(debounce);
}


// RTC save
void StoreAgg()
{
// Variable saving
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SAVING IN");
  lcd.setCursor(0,1);
  lcd.print("PROGRESS");
  RTC.adjust(DateTime(0,hourfeed,0,hourupg,minupg,0));
  delay(200);
}
