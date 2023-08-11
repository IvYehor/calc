#include <LiquidCrystal.h>
#include "parser.ino"

const int RSPIN = 8;
const int ENPIN = 9;
const int D4PIN = 4;
const int D5PIN = 5;
const int D6PIN = 6;
const int D7PIN = 7;
const int BLPIN = 10;

int checkButtons();
void displayLCD();
void changeChar(char ch);
void moveP(bool left);
void breakDown();
void compute();

LiquidCrystal lcd( RSPIN,  ENPIN,  D4PIN,  D5PIN,  D6PIN,  D7PIN);

int state = 0; 
// 0 - Digits 0-3
// 1 - Digits 4-7
// 2 - Digits 8,9 Parentheses ()
// 3 - +,-,*,/
// 4 - Left, Calculate, Delete, Right
// 5 - Delete all
int button = 0;
// button == RUDL
String expression = "";
int pointer = 0; // [0; 15]
int shift = 0; // [0; max int]

unsigned long pointerTimer = 0;
bool pointerFlag = false;

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

  pointerTimer = millis();
}

void loop() {
  displayLCD();
  button = checkButtons();
  
  if(button == 5) {
    state++;
    state%=6;
  }
  else if(button == 1) { // Right
    switch (state) {
      case 0:
        changeChar('3');
        break;
      case 1:
        changeChar('7');
        break;
      case 2:
        changeChar(')');
        break;
      case 3:
        changeChar('/');
        break;
      case 4: // Move right
        moveP(false);
        break;
      case 5: // Nothing
        break;
    }
  }
  else if(button == 2) { // Up
    switch (state) {
      case 0:
        changeChar('2');
        break;
      case 1:
        changeChar('6');
        break;
      case 2:
        changeChar('(');
        break;
      case 3:
        changeChar('*');
        break;
      case 4: // Delete char
        changeChar('<');
        break;
      case 5: // Nothing
        break;
    }
  }
  else if(button == 3) { // Down
    switch (state) {
      case 0:
        changeChar('1');
        break;
      case 1:
        changeChar('5');
        break;
      case 2:
        changeChar('9');
        break;
      case 3:
        changeChar('-');
        break;
      case 4: // Eval
        expression = String(calculate(expression));
        break;
      case 5: // Nothing
        break;
    }
  }
  else if(button == 4) { // Left
    switch (state) {
      case 0:
        changeChar('0');
        break;
      case 1:
        changeChar('4');
        break;
      case 2:
        changeChar('8');
        break;
      case 3:
        changeChar('+');
        break;
      case 4: // Left
        moveP(true);
        break;
      case 5: // Nothing
        break;
    }
  }
}

void changeChar(char ch) {
  if (ch == '<') {
    if (pointer == 0 && shift == 0)
      return;
    expression.remove(pointer + shift - 1, 1);
    
    pointer--;
    if (pointer < 0) {
      pointer = 0;
      shift--;
    }
  }
  else {
    expression = expression.substring(0, pointer+shift) + ch + expression.substring(pointer+shift, expression.length());

    pointer++;
    if(pointer > 15) {
      pointer = 15;
      shift++;
    }
  }
}

void moveP(bool left) {
  if (left)
    pointer--;
  else if(pointer + shift < expression.length()) // If to the right and pointer + shift <= expression.length()
    pointer++;
  
  if (pointer < 0) {
    pointer = 0;
    shift--;
  }
  if (pointer > 15) {
    pointer = 15;
    shift++;
  }
  if (shift < 0) {
    shift = 0;
  }
}

void displayLCD() {
  //Serial.println(expression);
  
  lcd.setCursor(0, 0);
  String dispString = "";
  int expLen = expression.length();
  
  if (expLen - shift >= 16)
    dispString = expression.substring(shift, shift+16);
  else {
    dispString = expression.substring(shift); // Fix + " "*(a-b)
    for (int i = 0; i < 16 - (expLen - shift); i++)
      dispString += ' ';  
  }

  if(millis() - pointerTimer  > 500) {
    pointerTimer = millis();
    pointerFlag = !pointerFlag;
  }

  if(pointerFlag)
    dispString.setCharAt(pointer, '_');
    
  lcd.print(dispString);
  
  lcd.setCursor(0, 1);
  switch(state)
  {
    case 0:
      lcd.print("S0 ");
      lcd.print("0 1 2 3");
      break;
    case 1:
      lcd.print("S1 ");
      lcd.print("4 5 6 7");
      break;
    case 2:
      lcd.print("S2 ");
      lcd.print("8 9 ( )");
      break;
    case 3:
      lcd.print("S3 ");
      lcd.print("+ - * /");
      break;
    case 4:
      lcd.print("S4 ");
      lcd.print("L = < R");
      break;
    case 5:
      lcd.print("S5 ");
      lcd.print("X      ");
      break;
  }
}

int checkButtons() {
  static int lastPressedButton = 0;
  int inp = analogRead(0);
  int lastInp = lastPressedButton; // lastInp variable is needed to not write "lastPressedButton = inp;" before every return
  lastPressedButton = inp;
  
  if (inp < 60 && lastInp > 800) {
    return 1;
  }
  else if(inp < 200 && lastInp > 800) {
    return 2;
  }
  else if(inp < 400 && lastInp > 800) {
    return 3;
  }
  else if(inp < 600 && lastInp > 800) {
    return 4;
  }
  else if(inp < 800 && lastInp > 800) {
    return 5;
  }
  else{
    return 0;
  }
}
