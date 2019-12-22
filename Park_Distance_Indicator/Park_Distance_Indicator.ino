#include <MaxMatrix.h>

// Sonic sensor
const int triggerPin = 9;
const int echoPin = 10;

// MAX7219
const int dinPin = 5;
const int csPin = 6;
const int clockPin = 7;

// Rendering constants
const int CAR_WIDTH = 2; // columns
const int MATRIX_WIDTH = 8; // columns;
const int MATRIX_HEIGHT = 8; // rows

// Distance config
const int MIN_DISTANCE = 10; // cm
const int MAX_DISTANCE = 50; // cm
const int RANGE = MAX_DISTANCE - MIN_DISTANCE; // cm
const int SEGMENT_DISTANCE = RANGE / MATRIX_HEIGHT; // cm per pixel

// Blink
const int BLINK_CYCLE_TIME = 1500; // ms

MaxMatrix matrixView(dinPin, csPin, clockPin, 1);

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  matrixView.init();
  matrixView.setIntensity(5);

  Serial.begin(9600);
}

int calculateDistance() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(triggerPin, LOW);

  const int duration = pulseIn(echoPin, HIGH);
  const int distance = duration * 0.034 / 2;

  return distance;
}

void updateDisplay(int distance) {
  matrixView.clear();

  if(distance > MAX_DISTANCE){
    blinkRow(7);
    return;
  }
  
  if(distance < MIN_DISTANCE) {
    blinkRow(0);
    return;
  }
  
  matrixView.setDot(0, 0, true);
  matrixView.setDot(0,7, true);
  matrixView.setDot(7, 0, true);
  matrixView.setDot(7, 7, true);

  
  
  drawCar(carPosX(), carPosY(distance));  
}

int carPosY(int distance) {
  return distance % SEGMENT_DISTANCE;
}

int carPosX() {
  return (MATRIX_WIDTH / 2) - (CAR_WIDTH / 2);
}

void setRow(int row, bool value) {
   for(int col = 0; col < MATRIX_WIDTH; col++){
      matrixView.setDot(col,row, value);
   }
}

void setColumn(int col, bool value) {
   for(int row = 0; row < MATRIX_HEIGHT; row++){
      matrixView.setDot(col,row, value);
   }
}

// Draws the car at the specified row, filling in the rows to 7
void drawCar(int posX, int posY) {
   for(int row = 7; row > posY; row--) {
     for(int col = posX; col < posX + CAR_WIDTH; col++) {
      matrixView.setDot(col, row, true);
     }
   }
}

void blinkRow(int row) {
  long onTime = BLINK_CYCLE_TIME / 2;
  if (millis() % BLINK_CYCLE_TIME < onTime) {
    setRow(row, true); 
  } else {
    setRow(row, false);
  }
}

void blinkColumn(int column) {
  long onTime = BLINK_CYCLE_TIME / 2;
  if (millis() % BLINK_CYCLE_TIME < onTime) {
    setColumn(column, true); 
  } else {
    setColumn(column, false);
  }
}

void loop() {
  int distance = calculateDistance();
  updateDisplay(distance);
  delay(250);
}
