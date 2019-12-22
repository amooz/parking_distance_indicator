#include <MaxMatrix.h>
#include <LiquidCrystal.h>

// Sonic distance sensor
const int DISTANCE_SENSOR_TRIGGER_PIN = 8;
const int DISTANCE_SENSOR_ECHO_PIN = 9;

// LCD Screen
const int LCD_RS_PIN = 2;
const int LCD_ENABLE_PIN = 3;
const int LCD_D4_PIN = 10;
const int LCD_D5_PIN = 11;
const int LCD_D6_PIN = 12;
const int LCD_D7_PIN = 13;

// MAX7219 Matrix display
const int MAX_MATRIX_DIN_PIN = 5;
const int MAX_MATRIX_CS_PIN = 6;
const int MAX_MATRIX_CLOCK_PIN = 7;

// Rendering constants
const int CAR_WIDTH = 2; // columns
const int MATRIX_WIDTH = 8; // columns;
const int MATRIX_HEIGHT = 8; // rows

// Distance config
const int MIN_DISTANCE = 50; // cm
const int MAX_DISTANCE = 150; // cm
const int DISTANCE_RANGE = MAX_DISTANCE - MIN_DISTANCE; // cm
const int SEGMENT_DISTANCE = DISTANCE_RANGE / MATRIX_HEIGHT; // cm per pixel

// Blink
const int BLINK_CYCLE_TIME = 1500; // ms

// String values
const String CURRENT_READING = "Distance: ";
const String CM = "cm";
const String TARGET_DISTANCE = "Target: ";

MaxMatrix matrixView(MAX_MATRIX_DIN_PIN, MAX_MATRIX_CS_PIN, MAX_MATRIX_CLOCK_PIN, 1);
LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

void setup() {
  pinMode(DISTANCE_SENSOR_TRIGGER_PIN, OUTPUT);
  pinMode(DISTANCE_SENSOR_ECHO_PIN, INPUT);

  matrixView.init();
  matrixView.setIntensity(5);

  lcd.begin(16, 2);

  Serial.begin(9600);
}

int calculateDistance() {
  digitalWrite(DISTANCE_SENSOR_TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  
  digitalWrite(DISTANCE_SENSOR_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(DISTANCE_SENSOR_TRIGGER_PIN, LOW);

  const int duration = pulseIn(DISTANCE_SENSOR_ECHO_PIN, HIGH);
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

void updateLcd(int distance) {
  lcd.clear();
  
  lcd.print(CURRENT_READING + distance + CM);
  lcd.setCursor(0, 1);
  lcd.print(TARGET_DISTANCE + MIN_DISTANCE + CM);
}

int carPosY(int distance) {
  return (distance - MIN_DISTANCE) / SEGMENT_DISTANCE;
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
  updateLcd(distance);
  delay(500);
}
