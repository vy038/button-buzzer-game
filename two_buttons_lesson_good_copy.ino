#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//declare variables
int bluePin = 3;
int redPin = 7;
int speakerPin = 9;
bool buzzable = false;
int long readBlue, readRed;
bool notReady = true;


void setup() {
  Serial.begin(9600);

  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
 //display.setContrast (0); // dim display
 
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  //sets inputs
  pinMode(bluePin, INPUT);
  pinMode(redPin, INPUT);

  display.setCursor(27, 28);
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.println("Ready?");
  display.display();

  while (notReady) {
    if (Serial.available()) {
      Serial.println("Ready!");
      break;
    }
  }

  tone(speakerPin, 261.6, 100);
  tone(speakerPin, 523.3, 500);

  startUp(); //start function (countdown of sorts)

  while(true) { //infinite loops to keep going until someone presses button (breaks out of loop and displays a screen)
    if (digitalRead(bluePin) == 1) {
      readBlue += 1;
    } 
    if (digitalRead(redPin) == 1) {
      readRed += 1;
    } 

    if (readBlue == 1) {
      display.clearDisplay();
      tone(speakerPin, 329.6, 500);
      turn(2);
      break;
    } 
    
    if (readRed == 1) {
      display.clearDisplay();
      tone(speakerPin, 261.63, 500);
      turn(1);
      break;
    }
  }
}


void loop() {
  if(Serial.available()) {
    Serial.print("Recieved data (ASCII): ");
    Serial.println(Serial.readString());
    if (Serial.readString() == "Blue") {
      Serial.println("Blue turn!");
      display.setCursor(0, 0);
      display.setTextSize(2);
      display.setTextColor(SH110X_WHITE);
      display.println("Blue turn!");
      //random text at bottom
      display.setTextColor(SH110X_BLACK, SH110X_WHITE); 
      display.setTextSize(1);
      display.setCursor(0, 50);
      display.print("Manual Switch");
      //arrow
      display.setTextColor(SH110X_WHITE); 
      display.setTextSize(2);
      display.setCursor(0, 25);
      display.print("--------->");
    } else if (Serial.readString() == "Red") {
      Serial.println("Red turn!");
      display.setCursor(0, 0);
      display.setTextSize(2);
      display.setTextColor(SH110X_WHITE);
      display.println("Red turn!");
      //random text at bottom
      display.setTextColor(SH110X_BLACK, SH110X_WHITE); 
      display.setTextSize(1);
      display.setCursor(0, 50);
      display.print("Manual Switch");
      //arrow
      display.setTextColor(SH110X_WHITE); 
      display.setTextSize(2);
      display.setCursor(0, 25);
      display.print("<---------");
    } else if (Serial.readString() == "Redo") {
      Serial.println("Restarting");
      restart();
    }
  }
}

void startUp() {
  checkBreak(2000);
  tone(speakerPin, 261.63, 500);
  display.clearDisplay();
  display.setCursor(55, 25);
  display.setTextSize(3);
  display.println("3");
  display.display();
  for (int i = 0; i < 100; i++) {
    if (digitalRead(bluePin) == 1) {
      readBlue += 1;
      if (readBlue >= 2) {
        readBlue = 2;
      }
    } 
    if (digitalRead(redPin) == 1) {
      if (readRed >= 2) {
        readRed = 2;
      }
      readRed += 1;
    } 
  }
  checkBreak(1000);
  tone(speakerPin, 329.63, 500);
  display.clearDisplay();
  display.setCursor(55, 25);
  display.println("2");
  display.display();
  for (int i = 0; i < 100; i++) {
    if (digitalRead(bluePin) == 1) {
      readBlue += 1;
    } 
    if (digitalRead(redPin) == 1) {
      readRed += 1;
    } 
  }
  checkBreak(1000);
  tone(speakerPin, 392, 500);
  display.clearDisplay();
  display.setCursor(55, 20);
  display.println("1");
  display.display();
  for (int i = 0; i < 100; i++) {
    if (digitalRead(bluePin) == 1) {
      readBlue += 1;
    } 
    if (digitalRead(redPin) == 1) {
      readRed += 1;
    } 
  }
  checkBreak(1000);
  tone(speakerPin, 523.3, 500);
  display.clearDisplay();
  display.setCursor(5, 25);
  display.setTextSize(4);
  display.setTextColor(SH110X_WHITE);
  display.println("BUZZ!");
  display.display();
  if (readRed >= 2 || readBlue >=2) {
    buzzable = false;
  } else {
    buzzable = true;
  }
  return;
}

void checkBreak(int i) {
  for (int j = 0; j < i; j++) {
    if (digitalRead(bluePin) == 1) {
      readBlue += 1;
    } 
    if (digitalRead(redPin) == 1) {
      readRed += 1;
    } 
    delay(1);
  }
  return;
}

void turn(int i) {
  switch (i) {
    case 1://red buzzed first
      pinMode(redPin, OUTPUT);
      digitalWrite(redPin, HIGH);
      //Big text at top displaying who's turn it is
      display.setCursor(0, 0);
      display.setTextSize(2);
      display.setTextColor(SH110X_WHITE);
      display.println("Red turn!");
      //random text at bottom
      display.setTextColor(SH110X_BLACK, SH110X_WHITE); 
      display.setTextSize(1);
      display.setCursor(0, 50);
      if (buzzable == false) {
        display.print("Blue buzzed early");
      } else {
        display.print("Red buzzed faster");
      }
      //arrow
      display.setTextColor(SH110X_WHITE); 
      display.setTextSize(2);
      display.setCursor(0, 25);
      display.print("<---------");
      break;
    case 2: //blue buzzed first
      pinMode(bluePin, OUTPUT);
      digitalWrite(bluePin, HIGH);
      //Big text at top displaying who's turn it is
      display.setCursor(0, 0);
      display.setTextSize(2);
      display.setTextColor(SH110X_WHITE);
      display.println("Blue turn!");
      //random text at bottom
      display.setTextColor(SH110X_BLACK, SH110X_WHITE); 
      display.setTextSize(1);
      display.setCursor(0, 50);
      if (buzzable == false) {
        display.print("Red buzzed early");
      } else {
        display.print("Blue buzzed faster");
      }
      //arrow
      display.setTextColor(SH110X_WHITE); 
      display.setTextSize(2);
      display.setCursor(0, 25);
      display.print("--------->");
      break;
  }
  display.display();
}

void restart() {
  setup();
  loop();
}