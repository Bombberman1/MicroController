#include <LiquidCrystal.h>

#define DDR_KEYPAD  DDRC
#define PORT_KEYPAD PORTC
#define PIN_KEYPAD  PINC
#include "keypad4x4.h"

const int buzzerPin = 21;

bool oneMinute = false;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 5, rw = 6, en = 7, d0 = 22, d1 = 23, d2 = 24, d3 = 25, d4 = 26, d5 = 27, d6 = 28, d7 = 29;
LiquidCrystal lcd(rs, rw, en, d0, d1, d2, d3, d4, d5, d6, d7);

bool start = false;

const int maxSavedTime = 60;

bool logMode = false;

int clicksB = 0;

char numberInput[3] = "--";

int inputs = 0;

const PROGMEM  char sixty[60][3] = {
  {"00"}, {"01"}, {"02"}, {"03"}, {"04"}, {"05"}, {"06"}, {"07"}, {"08"}, {"09"},
  {"10"}, {"11"}, {"12"}, {"13"}, {"14"}, {"15"}, {"16"}, {"17"}, {"18"}, {"19"},
  {"20"}, {"21"}, {"22"}, {"23"}, {"24"}, {"25"}, {"26"}, {"27"}, {"28"}, {"29"},
  {"30"}, {"31"}, {"32"}, {"33"}, {"34"}, {"35"}, {"36"}, {"37"}, {"38"}, {"39"},
  {"40"}, {"41"}, {"42"}, {"43"}, {"44"}, {"45"}, {"46"}, {"47"}, {"48"}, {"49"},
  {"50"}, {"51"}, {"52"}, {"53"}, {"54"}, {"55"}, {"56"}, {"57"}, {"58"}, {"59"}
};

struct Time
{
  unsigned char second, minute, hour;
};
Time T2 = {0, 0, 0};

void LCD_WriteStrPROGMEM(char *str, int n)  //вивід масиву символів,
{ //записаних у флеші
  for (int i = 0; i < n; i++)
    lcd.print( (char)pgm_read_byte( &(str[i]) ) );
}

ISR(TIMER4_COMPA_vect)  // Таймер Т1 по співпадінню А, кожної 1 сек.
{
  if (start) {
    if (++T2.second == 60) {
      T2.second = 0;
      oneMinute = true;
      if (++T2.minute == 60) {
        T2.minute = 0;
        if (++T2.hour == 24)
          T2.hour = 0;
      }
    }
    lcd.setCursor(3, 0);
    LCD_WriteStrPROGMEM(sixty[T2.hour], 2);
    lcd.write(':');
    LCD_WriteStrPROGMEM(sixty[T2.minute], 2);
    lcd.write(':');
    LCD_WriteStrPROGMEM(sixty[T2.second], 2);
  }
}

struct TimeLog {
  Time savedTime[maxSavedTime];
  int elements = 0;
  void addTime(unsigned char sec, unsigned char min, unsigned char hour) {
    if(elements < maxSavedTime) {
      savedTime[elements] = {sec, min, hour};
      elements++;
    } else {
      lcd.setCursor(11, 1);
      lcd.print("Error");
      delay(800);
      lcd.setCursor(11, 1);
      lcd.print("     ");
    }
  }
  void printLog(int clicks) {
    lcd.clear();
    if(elements > 0) {
      lcd.setCursor(13, 1);
      lcd.write('M');
      lcd.print(elements);
    }
    int row = 0;
    for(int i = clicks; i < clicks + 2 && i < elements; i++) {
      lcd.setCursor(1, row);
      lcd.print(i + 1);
      lcd.write('-');
      LCD_WriteStrPROGMEM(sixty[savedTime[i].hour], 2);
      lcd.write(':');
      LCD_WriteStrPROGMEM(sixty[savedTime[i].minute], 2);
      lcd.write(':');
      LCD_WriteStrPROGMEM(sixty[savedTime[i].second], 2);
      row++;
    }
  }
  void printTimeByKey(int key) {
    if(key > 0 && key <= elements) {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print(key);
      lcd.write('-');
      LCD_WriteStrPROGMEM(sixty[savedTime[key - 1].hour], 2);
      lcd.write(':');
      LCD_WriteStrPROGMEM(sixty[savedTime[key - 1].minute], 2);
      lcd.write(':');
      LCD_WriteStrPROGMEM(sixty[savedTime[key - 1].second], 2);
      digitalWrite(buzzerPin, HIGH);
      delay(200);
      digitalWrite(buzzerPin, LOW);
      delay(100);
      digitalWrite(buzzerPin, HIGH);
      delay(200);
      digitalWrite(buzzerPin, LOW);
      delay(800);
      printLog(0);
    } else {
      lcd.clear();
      lcd.setCursor(5, 1);
      char err[6] = "Error";
      for(int i = 0; i < 5; i++) lcd.write(err[i]);
      digitalWrite(buzzerPin, HIGH);
      delay(1000);
      digitalWrite(buzzerPin, LOW);
      printLog(0);
    }
  }
};

int getIntFromChars(char arr[]) {
  if(isdigit(arr[0])) {
    if(isdigit(arr[1])) {
      return (arr[0] - 48) * 10 + (arr[1] - 48);
    } else return arr[0] - 48;
  } else return -1;
}

TimeLog Log;

void setup() {
  noInterrupts();           // disable all interrupts

  // Таймер#1: Скид при співпадінні OCR1A (1sec) + дільник=256
  TCCR4A = 0x00;
  TCCR4B = (1 << WGM42) | (1 << CS42) | (1 << CS40); //CTC mode & Prescaler @ 1024
  TIMSK4 = (1 << OCIE4A); // дозвіл на переривання по співпадінню
  OCR4A = 0x3D08;// compare value = 1 sec (16MHz AVR)
  
  //KeyPad 4x4
  initKeyPad();

  //LCD 16x2
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  interrupts();  // Enable global interrupts

  //Buzzer
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
  if ( isButtonPressed() ) {
    char pressedButton = readKeyFromPad4x4();
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    switch (pressedButton) {
      case 'D':
        if(!logMode) {
          start = !start;
          delay(200);
        }
        break;
      case 'C':
        start = false;
        delay(200);
        T2 = {0, 0, 0};
        lcd.setCursor(3, 0);
        LCD_WriteStrPROGMEM(sixty[T2.hour], 2);
        lcd.write(':');
        LCD_WriteStrPROGMEM(sixty[T2.minute], 2);
        lcd.write(':');
        LCD_WriteStrPROGMEM(sixty[T2.second], 2);
        break;
      case 'F':
        if(logMode) {
          Log = TimeLog();
          lcd.clear();
        }
        break;
      case 'E':
        if(start) Log.addTime(T2.second, T2.minute, T2.hour);
        else if(logMode) {
          Log.printTimeByKey(getIntFromChars(numberInput));
          inputs = 0;
          numberInput[0] = '-';
          numberInput[1] = '-';
        }
        break;
      case 'A':
        start = false;
        delay(200);
        logMode = !logMode;
        if(logMode) {
          Log.printLog(0);
        } else {
          lcd.clear();
          clicksB = 0;
          inputs = 0;
          numberInput[0] = '-';
          numberInput[1] = '-';
          start = true;
          delay(200);
        }
        break;
      case 'B':
        if(logMode) {
          clicksB++;
          if(clicksB == Log.elements) clicksB = 0;
          Log.printLog(clicksB);
        }
        break;
      default:
        if(logMode) {
          if(inputs < 2) {
            numberInput[inputs] = pressedButton;
            lcd.clear();
            lcd.setCursor(14, 0);
            lcd.print(getIntFromChars(numberInput));
            inputs++;
          }
        }
    }
  }
  if(oneMinute) {
    digitalWrite(buzzerPin, HIGH);
    delay(300);
    digitalWrite(buzzerPin, LOW);
    oneMinute = false;
  }
}
