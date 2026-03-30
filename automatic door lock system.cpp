#include <Adafruit_LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

//the configuration
char password[] = "8213";  
char enteredCode[5];       
int keyCount = 0;
const int rfidPin = 10;    // The pin for our "RFID Button"

//these are the settings for the hardware part
Servo myServo;
Adafruit_LiquidCrystal lcd(0); 

// Keypad setup for Pins 7, 6, 5, 4, 3, 2, 9, 8
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {7, 6, 5, 4}; 
byte colPins[COLS] = {3, 2, 9, 8}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  myServo.attach(11);      
  myServo.write(0); 
  
  pinMode(rfidPin, INPUT_PULLUP); // Setup the RFID button
  
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  resetDisplay();
}

void loop() {
  // this is card detection section
  if (digitalRead(rfidPin) == LOW) { // Button pressed = Card Tapped
    lcd.clear();
    lcd.print("CARD Detected");
    lcd.setCursor(0, 1);
    lcd.print("Valid Card!");
    delay(500);
    openDoor();
  }

  //Check Keypad
  char key = keypad.getKey();
  if (key) {
    if (key == '*') {
      keyCount = 0;
      resetDisplay();
    } 
    else if (key == 'D') {
      if (keyCount == 4) {
        checkPassword();
      } else {
        lcd.clear();
        lcd.print("Enter 4 digits");
        delay(1000);
        resetDisplay();
        lcd.setCursor(0, 1);
        for(int i = 0; i < keyCount; i++) lcd.print('*');
      }
    }
    else if (keyCount < 4) {
      if (key >= '0' && key <= '9') {
        enteredCode[keyCount] = key;
        lcd.setCursor(keyCount, 1);
        lcd.print('*'); 
        keyCount++;
      }
    }
  }
}

void checkPassword() {
  if (enteredCode[0] == password[0] && 
      enteredCode[1] == password[1] && 
      enteredCode[2] == password[2] && 
      enteredCode[3] == password[3]) {
    lcd.clear();
    lcd.print("Access Granted");
    openDoor();
  } else {
    lcd.clear();
    lcd.print("Wrong Code!");
    delay(2000);
  }
  keyCount = 0;
  resetDisplay();
}

void openDoor() {
  lcd.setCursor(0, 1);
  lcd.print("Opening..");
  delay(1000);
  lcd.print("Welcome");
  myServo.write(90);  
  delay(3000);        
  myServo.write(0);   
  resetDisplay();
}

void resetDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Pin/Card:");
  lcd.setCursor(0, 1);
}