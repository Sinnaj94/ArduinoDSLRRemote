
//LCD Shield
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
String t;
int a;
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

//NEX
#include <multiCameraIrControl.h>
Sony NEX(9);
double timeDelay;
long interval;
double timeSetting;
int numberPhotosTaken;
String outputTime;
String outputPossible;
String outputTaken;
int fps;
double maxSeconds;
double minSeconds;
bool locked;
long previousMillis = 0;
void setup() {
  fps = 25;
  minSeconds = .1;
  maxSeconds = 60;
  timeDelay = 1;
  setTime(timeDelay);
  outputTaken = "";
  numberPhotosTaken = 0;
  locked = false;
  
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.setCursor(0,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(buttonPress()!=btnSELECT){
    checkButtons();
  }
  while(true){
    photoLoop();
  }
  
}

void photoLoop(){
  /*unsigned long currentMillis = millis();
  checkButtons();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    takePhoto();
  }*/

  //this is mostly accurate!
  takePhoto();
  delay(interval);
}

void checkButtons(){
  if(buttonPress()==btnLEFT){
      increaseTime(-1);
    }else if(buttonPress() == btnRIGHT){
      increaseTime(1);
    }else if(buttonPress() == btnUP){
      increaseTime(.1);
    }else if(buttonPress() == btnDOWN){
      increaseTime(-.1);
    }
    delay(150);
    printOutputTime();
    convertTimeToFit();
}

void convertTimeToFit(){
  interval = timeDelay * 1000;
}

void setTime(double seconds){
  if(seconds > 0 && seconds <= maxSeconds){
    timeDelay = seconds;
    outputTime = getTimeFormat(timeDelay)+ "     ";
  }
  
}

String calculateTime(int seconds){
  int calc = seconds*fps;
  return getTimeFormat(calc) + "/" + (String)fps;
}


String getTimeFormat(double seconds){
  String r = "";
  int h = 0;
  int m = 0;
  double s;
  while(seconds >= 3600){
    h++;
    seconds-=3600;
  }
  while(seconds >= 60){
    m++;
    seconds-=60;
  }
  s = seconds;
  if(h>0){
    r+= (String)h + "h";
  }
  if(m>0){
    r+= (String)m + "m";
  }if(s>0){
    r+= (String)s + "s";
  }
  return r;
}


double getCurrentSeconds(){
  return timeDelay;
}

void increaseTime(double s){
  setTime(getCurrentSeconds() + s);
}

//NEX
void takePhoto(){
  //NEX.shutterNow();
  numberPhotosTaken++;
  outputTaken = (String)numberPhotosTaken + " photos taken";
  printLCD();
}

void printLCD(){
  lcd.setCursor(0,1);
  lcd.print(outputTaken);
}



void printOutputTime(){
  lcd.setCursor(0,0);
  lcd.print(outputTime);
}

int read_LCD_buttons()
{
 adc_key_in = analogRead(0);
 if (adc_key_in > 1000) return btnNONE;
 
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   



 return btnNONE;  // when all others fail, return this...
}

int buttonPress(){
  lcd_key = read_LCD_buttons();  // read the buttons
  return lcd_key;
}

