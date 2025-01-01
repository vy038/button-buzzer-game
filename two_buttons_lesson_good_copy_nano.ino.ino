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

//declare pins with leds and speaker pin
int bluePin = 7;
int redPin = 2;
int greenPin = 10;
int yellowPin = 5;
int speakerPin = 3;
int inPin = 6;


//other variables
bool buzzable = false;
int long readBlue, readRed, readGreen, readYellow = 0;
bool notReady = true;

void setup() {
  Serial.begin(9600);

  //OLED STUFF
  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
 //display.setContrast (0); // dim display
  display.display();
  delay(2000);
  // Clear the buffer.
  display.clearDisplay();


  //sets inputs and outputs for all pins 
  pinMode(bluePin, INPUT);
  pinMode(redPin, INPUT);
  pinMode(greenPin, INPUT);
  pinMode(yellowPin, INPUT);
  pinMode(inPin, INPUT);
  pinMode(speakerPin, OUTPUT);
  

  //OLED ready screen display
  display.setCursor(27, 28);
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.println("Ready?");
  display.display();

  //boolean set to false, when black button pressed, countdown begins
  while (notReady) {
    if (digitalRead(inPin) == HIGH) {
      notReady = false;
    }
  }

  tone(speakerPin, 523, 500);

  
  //carries on with a startup function
  startUp(); //start function (countdown of sorts)

  //after countdown, while no buttons are pressed, keep checking if any are pressed
  while (true) { //infinite loops to keep going until someone presses button (breaks out of loop and displays a screen)
    //add one to counter if button is pressed (would be greater than 1 if pressed before)
    if (digitalRead(bluePin) == HIGH) {
      readBlue += 1;
    } else if (digitalRead(redPin) == HIGH) {
      readRed += 1;
    } else if (digitalRead(greenPin) == HIGH) {
      readGreen += 1;
    } else if (digitalRead(yellowPin) == HIGH) {
      readYellow += 1;
    } 

    //if a button is pressed ONLY ONCE, make it their turn
    if (readBlue == 1) {
      display.clearDisplay();
      turn(2);
      noTone(speakerPin);
      tone(speakerPin, 329, 500);
      break;
    } 
    
    if (readRed == 1) {
      display.clearDisplay();
      turn(1);
      noTone(speakerPin);
      tone(speakerPin, 261, 500);
      break;
    }

    if (readGreen == 1) {
      display.clearDisplay();
      turn(3);
      noTone(speakerPin);
      tone(speakerPin, 392, 500);
      break;
    }

    if (readYellow == 1) {
      display.clearDisplay();
      turn(4);
      noTone(speakerPin);
      tone(speakerPin, 523, 500);
      break;
    }
  }
}


void loop() { //empty
}

void startUp() { //starts 3 second countdown, checking if any buttons are pressed in the meantime
  //brief delay
  checkBreak(2000);
  //three
  tone(speakerPin, 261, 150);
  delay(150);
  display.clearDisplay();
  display.setCursor(55, 25);
  display.setTextSize(3);
  display.println("3");
  display.display();
  checkBreak(1000);
  //two
  noTone(speakerPin);
  tone(speakerPin, 261, 150);
  delay(150);
  display.clearDisplay();
  display.setCursor(55, 25);
  display.println("2");
  display.display();
  checkBreak(1000);
  //one
  noTone(speakerPin);
  tone(speakerPin, 261, 150);
  delay(150);
  display.clearDisplay();
  display.setCursor(55, 20);
  display.println("1");
  display.display();
  checkBreak(1000);
  //BUZZ
  noTone(speakerPin);
  tone(speakerPin, 523, 100);
  delay(100);
  display.clearDisplay();
  display.setCursor(5, 25);
  display.setTextSize(4);
  display.setTextColor(SH110X_WHITE);
  display.println("BUZZ!");
  display.display();
  //triggers buzzable variable
  if (readRed >= 2 || readBlue >=2) {
    buzzable = false;
  } else {
    buzzable = true;
  }
  return;
}

void checkBreak(int i) { //function for checking if buttons are pressed with delay
  for (int j = 0; j < i/2; j++) {
    if (digitalRead(bluePin) == 1) {
      readBlue += 1;
    } 
    if (digitalRead(redPin) == 1) {
      readRed += 1;
    } 
    if (digitalRead(greenPin) == 1) {
      readGreen += 1;
    } 
    if (digitalRead(yellowPin) == 1) {
      readYellow += 1;
    } 
    delay(2);
  }
  return;
}

void turn(int i) { //performs task based on who's turn it is, lights LED and displays OLED screen stuff
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
      display.print("Red buzzed faster (and on time)");
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
      display.print("Blue buzzed faster (and on time)");
      //arrow
      display.setTextColor(SH110X_WHITE); 
      display.setTextSize(2);
      display.setCursor(0, 25);
      display.print("--------->");
      break;

    case 3: //green
      pinMode(greenPin, OUTPUT);
      digitalWrite(greenPin, HIGH);
      display.setCursor(0, 0);
      display.setTextSize(2);
      display.setTextColor(SH110X_WHITE);
      display.println("Grn turn!");
      //random text at bottom
      display.setTextColor(SH110X_BLACK, SH110X_WHITE); 
      display.setTextSize(1);
      display.setCursor(0, 50);
      display.print("Green buzzed faster (and on time)");
      //arrow
      display.setTextColor(SH110X_WHITE); 
      display.setTextSize(2);
      display.setCursor(0, 25);
      display.print("--------->");
      break;
    case 4: //yellow
      pinMode(yellowPin, OUTPUT);
      digitalWrite(yellowPin, HIGH);
      display.setCursor(0, 0);
      display.setTextSize(2);
      display.setTextColor(SH110X_WHITE);
      display.println("Yllw turn!");
      //random text at bottom
      display.setTextColor(SH110X_BLACK, SH110X_WHITE); 
      display.setTextSize(1);
      display.setCursor(0, 50);
      display.print("Yellow buzzed faster (and on time)");
      //arrow
      display.setTextColor(SH110X_WHITE); 
      display.setTextSize(2);
      display.setCursor(0, 25);
      display.print("<---------");
      break;
  }
  display.display();
}