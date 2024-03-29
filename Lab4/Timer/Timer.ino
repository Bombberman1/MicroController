int a0 = 22;
int a1 = 23;
int a2 = 24;
int a3 = 25;
int a4 = 26;
int a5 = 27;
int a6 = 28;
int a7 = 29;

int c2 = 35;
int c3 = 34;
int c4 = 33;
int c5 = 32;
int c6 = 31;
int c7 = 30;

int b0 = 53;
int b1 = 52;
int b2 = 51;
int b3 = 50;

int buzzer = 64;

unsigned char start = 1;
unsigned char oneMinute = 0;
unsigned char logMode = 0;
const int maxSavedTime = 10;
unsigned char log_arr[4];

unsigned char time_arr[4];

struct Time
{
  unsigned char second, minute, hour;
};
Time timer = {0, 0, 0};

void drawTime(unsigned char time[]) {
  for(int time_element = 0; time_element < 3; time_element++) {
    unsigned char arr[4];
    if(time[time_element] > 9) {
      arr[0] = (time[time_element] - (time[time_element] % 10)) / 10;
      arr[1] = time[time_element] % 10;
    } else {
      arr[0] = 0;
      arr[1] = time[time_element];
    }
    for(int char_element = 0; char_element < 2; char_element++) {
      unsigned char integer = arr[char_element];
      delay(1);
      if(time_element != 0 || char_element != 0) digitalWrite((c2 - (2 * time_element) - char_element + 1), LOW);
      else digitalWrite(c7, LOW);
      switch(integer) {
        case 0:
          digitalWrite((c2 - (2 * time_element) - char_element), HIGH);
          digitalWrite(a0, LOW);
          digitalWrite(a1, LOW);
          digitalWrite(a2, LOW);
          digitalWrite(a3, LOW);
          digitalWrite(a4, LOW);
          digitalWrite(a5, LOW);
          digitalWrite(a6, HIGH);
          if(char_element == 1 && time_element != 2) digitalWrite(a7, LOW);
          else digitalWrite(a7, HIGH);
          break;
        case 1:
          digitalWrite((c2 - (2 * time_element) - char_element), HIGH);
          digitalWrite(a0, HIGH);
          digitalWrite(a1, LOW);
          digitalWrite(a2, LOW);
          digitalWrite(a3, HIGH);
          digitalWrite(a4, HIGH);
          digitalWrite(a5, HIGH);
          digitalWrite(a6, HIGH);
          if(char_element == 1 && time_element != 2) digitalWrite(a7, LOW);
          else digitalWrite(a7, HIGH);
          break;
        case 2:
          digitalWrite((c2 - (2 * time_element) - char_element), HIGH);
          digitalWrite(a0, LOW);
          digitalWrite(a1, LOW);
          digitalWrite(a2, HIGH);
          digitalWrite(a3, LOW);
          digitalWrite(a4, LOW);
          digitalWrite(a5, HIGH);
          digitalWrite(a6, LOW);
          if(char_element == 1 && time_element != 2) digitalWrite(a7, LOW);
          else digitalWrite(a7, HIGH);
          break;
        case 3:
          digitalWrite((c2 - (2 * time_element) - char_element), HIGH);
          digitalWrite(a0, LOW);
          digitalWrite(a1, LOW);
          digitalWrite(a2, LOW);
          digitalWrite(a3, LOW);
          digitalWrite(a4, HIGH);
          digitalWrite(a5, HIGH);
          digitalWrite(a6, LOW);
          if(char_element == 1 && time_element != 2) digitalWrite(a7, LOW);
          else digitalWrite(a7, HIGH);
          break;
        case 4:
          digitalWrite((c2 - (2 * time_element) - char_element), HIGH);
          digitalWrite(a0, HIGH);
          digitalWrite(a1, LOW);
          digitalWrite(a2, LOW);
          digitalWrite(a3, HIGH);
          digitalWrite(a4, HIGH);
          digitalWrite(a5, LOW);
          digitalWrite(a6, LOW);
          if(char_element == 1 && time_element != 2) digitalWrite(a7, LOW);
          else digitalWrite(a7, HIGH);
          break;
        case 5:
          digitalWrite((c2 - (2 * time_element) - char_element), HIGH);
          digitalWrite(a0, LOW);
          digitalWrite(a1, HIGH);
          digitalWrite(a2, LOW);
          digitalWrite(a3, LOW);
          digitalWrite(a4, HIGH);
          digitalWrite(a5, LOW);
          digitalWrite(a6, LOW);
          if(char_element == 1 && time_element != 2) digitalWrite(a7, LOW);
          else digitalWrite(a7, HIGH);
          break;
        case 6:
          digitalWrite((c2 - (2 * time_element) - char_element), HIGH);
          digitalWrite(a0, LOW);
          digitalWrite(a1, HIGH);
          digitalWrite(a2, LOW);
          digitalWrite(a3, LOW);
          digitalWrite(a4, LOW);
          digitalWrite(a5, LOW);
          digitalWrite(a6, LOW);
          if(char_element == 1 && time_element != 2) digitalWrite(a7, LOW);
          else digitalWrite(a7, HIGH);
          break;
        case 7:
          digitalWrite((c2 - (2 * time_element) - char_element), HIGH);
          digitalWrite(a0, LOW);
          digitalWrite(a1, LOW);
          digitalWrite(a2, LOW);
          digitalWrite(a3, HIGH);
          digitalWrite(a4, HIGH);
          digitalWrite(a5, HIGH);
          digitalWrite(a6, HIGH);
          if(char_element == 1 && time_element != 2) digitalWrite(a7, LOW);
          else digitalWrite(a7, HIGH);
          break;
        case 8:
          digitalWrite((c2 - (2 * time_element) - char_element), HIGH);
          digitalWrite(a0, LOW);
          digitalWrite(a1, LOW);
          digitalWrite(a2, LOW);
          digitalWrite(a3, LOW);
          digitalWrite(a4, LOW);
          digitalWrite(a5, LOW);
          digitalWrite(a6, LOW);
          if(char_element == 1 && time_element != 2) digitalWrite(a7, LOW);
          else digitalWrite(a7, HIGH);
          break;
        case 9:
          digitalWrite((c2 - (2 * time_element) - char_element), HIGH);
          digitalWrite(a0, LOW);
          digitalWrite(a1, LOW);
          digitalWrite(a2, LOW);
          digitalWrite(a3, LOW);
          digitalWrite(a4, HIGH);
          digitalWrite(a5, LOW);
          digitalWrite(a6, LOW);
          if(char_element == 1 && time_element != 2) digitalWrite(a7, LOW);
          else digitalWrite(a7, HIGH);
          break;
      }
    }
  }
}

void clearTime() {
  for(int digit = 0; digit < 6; digit++) {
    delay(1);
    digitalWrite((c2 - digit), HIGH);
    digitalWrite(a0, HIGH);
    digitalWrite(a1, HIGH);
    digitalWrite(a2, HIGH);
    digitalWrite(a3, HIGH);
    digitalWrite(a4, HIGH);
    digitalWrite(a5, HIGH);
    digitalWrite(a6, HIGH);
    digitalWrite(a7, HIGH);
    digitalWrite((c2 - digit), LOW);
  }
}

struct TimeLog {
  Time savedTime[maxSavedTime];
  int elements = 0;
  unsigned char logSecond = 0;
  void addTime(unsigned char sec, unsigned char min, unsigned char hour) {
    if(elements < maxSavedTime) {
      savedTime[elements] = {sec, min, hour};
      ++elements;
      delay(100);
    } else {
      digitalWrite(buzzer, HIGH);
      delay(1000);
      digitalWrite(buzzer, LOW);
    }
  }
  void printLog() {
    if(logSecond <= elements) {
      log_arr[0] = savedTime[logSecond - 1].hour;
      log_arr[1] = savedTime[logSecond - 1].minute;
      log_arr[2] = savedTime[logSecond - 1].second;
      drawTime(log_arr);
    }
  }
};

TimeLog Log;

ISR(TIMER1_COMPA_vect) {
  if(!start) {
    if(++timer.second == 60) {
      timer.second = 0;
      oneMinute = 1;
      if(++timer.minute == 60) {
        timer.minute = 0;
        if(++timer.hour == 24)
          timer.hour = 0;
      }
    }
  }
  if(logMode) {
    if(++Log.logSecond > Log.elements) {
      Log.logSecond = 0;
    }
  }
}

ISR(TIMER4_COMPA_vect) {
  if(!logMode) {
    time_arr[0] = timer.hour;
    time_arr[1] = timer.minute;
    time_arr[2] = timer.second;
    drawTime(time_arr);
  } else {
    if(Log.logSecond) Log.printLog();
    else clearTime();
  }
}

void setup() {
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
  TCNT1 = 0;
  TIMSK1 = (1 << OCIE1A);
  OCR1A = 15624;
  // (16000000 / (1 * 1024)) - 1

  TCCR4A = 0;
  TCCR4B = (1 << WGM42) | (1 << CS42);
  TCNT4 = 0;
  TIMSK4 = (1 << OCIE4A);
  OCR4A = 624;
  // (16000000 / (100 * 256)) - 1

  pinMode(a0, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(a3, OUTPUT);
  pinMode(a4, OUTPUT);
  pinMode(a5, OUTPUT);
  pinMode(a6, OUTPUT);
  pinMode(a7, OUTPUT);

  pinMode(c2, OUTPUT);
  pinMode(c3, OUTPUT);
  pinMode(c4, OUTPUT);
  pinMode(c5, OUTPUT);
  pinMode(c6, OUTPUT);
  pinMode(c7, OUTPUT);

  pinMode(b0, INPUT);
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);
  pinMode(b0, INPUT_PULLUP);
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);
}

void loop() {
  if(digitalRead(b0) == LOW) {
    if(!logMode) {
      start++;
      delay(100);
      if(start == 2) {
        timer.hour = 0;
        timer.minute = 0;
        timer.second = 0;
        delay(100);
      } else if(start == 3) {
        start = 0;
        delay(100);
      }
    }
  } else if(digitalRead(b1) == LOW) {
    if(!logMode) Log.addTime(timer.second, timer.minute, timer.hour);
  } else if(digitalRead(b2) == LOW) {
    if(!logMode) {
      free(&Log);
      Log = TimeLog();
    }
  } else if(digitalRead(b3) == LOW) {
    if(start) {
      logMode = logMode ^ 1;
      delay(100);
      Log.logSecond = 0;
      delay(100);
      start = 1;
      delay(100);
    }
  }
  if(oneMinute) {
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    oneMinute = 0;
  }
}
