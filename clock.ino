/* 
 *  Bare Minimum Arduino Clock without any optimalization
 *  Created by Tibor Simon
 *  2015-12-31
 *  
 *  Tibor Simon
 */

int tickCounter = 0;
int event = 0;
int editEvent = 0;
int debounceCounter = 0;
int state = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;
const int editHM = A4;
const int editDir = A5;

int displayBrightness = 127;
const int D1 = 6;
const int D2 = 9;
const int D3 = 10;
const int D4 = 11;

int dstate = 0;

const int digitA = 7;
const int digitB = 8;
const int digitC = 12;
const int digitD = 13;
const int digitE = A3;
const int digitF = A2;
const int digitG = A1;

int i = 0;

void setup() {
  pinMode(editHM, INPUT);
  pinMode(editDir, INPUT);

  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  
  pinMode(digitA, OUTPUT);
  pinMode(digitB, OUTPUT);
  pinMode(digitC, OUTPUT);
  pinMode(digitD, OUTPUT);
  pinMode(digitE, OUTPUT);
  pinMode(digitF, OUTPUT);
  pinMode(digitG, OUTPUT);
  
  analogWrite(5, 128);
  attachInterrupt(digitalPinToInterrupt(3), tick, RISING);
  attachInterrupt(digitalPinToInterrupt(2), edit, RISING);
  Serial.begin(9600);
}

void tick() {
  if(debounceCounter) {
    if(++debounceCounter == 100) {
      debounceCounter = 0;
    }
  }

  if(++tickCounter == 978 /*489*/) {
    tickCounter = 0;
    event = 1;
  }
  return;
}

void edit() {
  if(!debounceCounter) {
    debounceCounter = 1;
    editEvent = 1;
  }
  return;
}

void printTime() {
  Serial.print(hours);
  Serial.print(":");
  Serial.print(minutes);
  Serial.print(":");
  Serial.println(seconds);
}

void renderNumber(int number) {
  switch(number) {
    case 0:
      digitalWrite(digitA, LOW);
      digitalWrite(digitB, LOW);
      digitalWrite(digitC, LOW);
      digitalWrite(digitD, LOW);
      digitalWrite(digitE, LOW);
      digitalWrite(digitF, LOW);
      digitalWrite(digitG, HIGH);
      break;
    case 1:
      digitalWrite(digitA, HIGH);
      digitalWrite(digitB, LOW);
      digitalWrite(digitC, LOW);
      digitalWrite(digitD, HIGH);
      digitalWrite(digitE, HIGH);
      digitalWrite(digitF, HIGH);
      digitalWrite(digitG, HIGH);
      break;
    case 2:
      digitalWrite(digitA, LOW);
      digitalWrite(digitB, LOW);
      digitalWrite(digitC, HIGH);
      digitalWrite(digitD, LOW);
      digitalWrite(digitE, LOW);
      digitalWrite(digitF, HIGH);
      digitalWrite(digitG, LOW);
      break;
    case 3:
      digitalWrite(digitA, LOW);
      digitalWrite(digitB, LOW);
      digitalWrite(digitC, LOW);
      digitalWrite(digitD, LOW);
      digitalWrite(digitE, HIGH);
      digitalWrite(digitF, HIGH);
      digitalWrite(digitG, LOW);
      break;
    case 4:
      digitalWrite(digitA, HIGH);
      digitalWrite(digitB, LOW);
      digitalWrite(digitC, LOW);
      digitalWrite(digitD, HIGH);
      digitalWrite(digitE, HIGH);
      digitalWrite(digitF, LOW);
      digitalWrite(digitG, LOW);
      break;
    case 5:
      digitalWrite(digitA, LOW);
      digitalWrite(digitB, HIGH);
      digitalWrite(digitC, LOW);
      digitalWrite(digitD, LOW);
      digitalWrite(digitE, HIGH);
      digitalWrite(digitF, LOW);
      digitalWrite(digitG, LOW);
      break;
    case 6:
      digitalWrite(digitA, LOW);
      digitalWrite(digitB, HIGH);
      digitalWrite(digitC, LOW);
      digitalWrite(digitD, LOW);
      digitalWrite(digitE, LOW);
      digitalWrite(digitF, LOW);
      digitalWrite(digitG, LOW);
      break;
    case 7:
      digitalWrite(digitA, LOW);
      digitalWrite(digitB, LOW);
      digitalWrite(digitC, LOW);
      digitalWrite(digitD, HIGH);
      digitalWrite(digitE, HIGH);
      digitalWrite(digitF, HIGH);
      digitalWrite(digitG, HIGH);
      break;
    case 8:
      digitalWrite(digitA, LOW);
      digitalWrite(digitB, LOW);
      digitalWrite(digitC, LOW);
      digitalWrite(digitD, LOW);
      digitalWrite(digitE, LOW);
      digitalWrite(digitF, LOW);
      digitalWrite(digitG, LOW);
      break;
    case 9:
      digitalWrite(digitA, LOW);
      digitalWrite(digitB, LOW);
      digitalWrite(digitC, LOW);
      digitalWrite(digitD, LOW);
      digitalWrite(digitE, HIGH);
      digitalWrite(digitF, LOW);
      digitalWrite(digitG, LOW);
      break;
    case 10:
      digitalWrite(digitA, HIGH);
      digitalWrite(digitB, HIGH);
      digitalWrite(digitC, HIGH);
      digitalWrite(digitD, HIGH);
      digitalWrite(digitE, HIGH);
      digitalWrite(digitF, HIGH);
      digitalWrite(digitG, HIGH);
      break;
    default:
      break;
  }
}

void displayTime() {
  // handle brightness
//  displayBrightness = map(analogRead(A0), 0, 1023, 0, 255);
//  if(!displayBrightness) displayBrightness = 1;
//  analogWrite(D1, displayBrightness);
//  analogWrite(D2, displayBrightness);
//  analogWrite(D3, displayBrightness);
//  analogWrite(D4, displayBrightness);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);

  // handle numbers
  int temp = 0;
  switch(dstate) {
    case 0:
      temp = hours / 10;
      renderNumber(temp);
      pinMode(D4, INPUT);
      pinMode(D1, OUTPUT);
      dstate = 1;
      break;
    case 1:
      temp = hours % 10;
      renderNumber(temp);
      pinMode(D1, INPUT);
      pinMode(D2, OUTPUT);
      dstate = 2;
      break;
    case 2:
      temp = minutes / 10;
      renderNumber(temp);
      pinMode(D2, INPUT);
      pinMode(D3, OUTPUT);
      dstate = 3;
      break;
    case 3:
      temp = minutes % 10;
      renderNumber(temp);
      pinMode(D3, INPUT);
      pinMode(D4, OUTPUT);
      dstate = 0;
      break;
  }
  delay(2);
}

void handleEditEvent() {
  editEvent = 0;
  int var = digitalRead(editDir) ? -1 : 1;
  if(digitalRead(editHM)) {
    hours += var;
    if(hours == -1) hours = 23;
    if(hours == 24) hours = 0;
  } else {
    minutes += var;
    if(minutes == -1) minutes = 59;
    if(minutes == 60) minutes = 0;
  }
  seconds = 0;
}

void handleEvent() {
  event = 0;
  if(++seconds == 60) {
    seconds = 0;
    if(++minutes == 60) {
      minutes = 0;
      if(++hours == 24) {
        hours = 0;
      }
    }
  }
}

void loop() {
  if(editEvent) {
    handleEditEvent();
  }
  if(event) {
    handleEvent();
//    printTime();
  }
  displayTime();
}


