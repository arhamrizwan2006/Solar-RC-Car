#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <INA226.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
INA226 ina226(0x44);

#define ENA 3
#define IN1 2
#define IN2 4
#define IN3 5
#define IN4 7
#define ENB 6
#define SOLAR_PIN A0

uint16_t angleHistory[5] = {0,0,0,0,0};
uint8_t  radiusHistory[5] = {0,0,0,0,0};
int historyIndex = 0;

unsigned long lastDisplayUpdate = 0;

void setup() {
  Dabble.begin(9600, 10, 11);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Wire.begin();

  // LCD setup
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Solar RC Car  ");
  lcd.setCursor(0, 1);
  lcd.print("  Initializing  ");
  delay(2000);
  lcd.clear();

  // INA226 setup
  ina226.begin();
  ina226.setMaxCurrentShunt(0.8, 0.1);  // 0.8A max current, 0.1 ohm shunt

  stopCar();
}

void setSpeed(int spd) {
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
}

void forward(int spd) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  setSpeed(spd);
}

void backward(int spd) {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  setSpeed(spd);
}

void turnLeft(int spd) {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  setSpeed(spd);
}

void turnRight(int spd) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  setSpeed(spd);
}

void stopCar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

uint16_t smoothAngle(uint16_t newAngle) {
  angleHistory[historyIndex] = newAngle;
  uint32_t sum = 0;
  for (int i = 0; i < 5; i++) sum += angleHistory[i];
  return sum / 5;
}

uint8_t smoothRadius(uint8_t newRadius) {
  radiusHistory[historyIndex] = newRadius;
  historyIndex = (historyIndex + 1) % 5;
  uint16_t sum = 0;
  for (int i = 0; i < 5; i++) sum += radiusHistory[i];
  return sum / 5;
}

float getSolarVoltage() {
  int raw = analogRead(SOLAR_PIN);
  float voltage = (raw / 1023.0) * 5.0;
  return voltage * (10.0 + 4.7) / 4.7;
}

float getBatteryPercent(float voltage) {
  
  float percent = (voltage - 6.0) / (8.4 - 6.0) * 100.0;
  return constrain(percent, 0, 100);
}

void updateLCD() {
  float busVoltage = ina226.getBusVoltage();
  float current    = ina226.getCurrent() * 1000.0;  // convert to mA
  float power       = ina226.getPower();              // watts
  float percent     = getBatteryPercent(busVoltage);
  float solarV       = getSolarVoltage();

  // Row 1 — Voltage and Current
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(busVoltage, 1);
  lcd.print("V I:");
  lcd.print(current, 0);
  lcd.print("mA   ");

  // Row 2 — Power, Charge, Solar
  lcd.setCursor(0, 1);
  lcd.print("P:");
  lcd.print(power, 1);
  lcd.print("W C:");
  lcd.print(percent, 0);
  lcd.print("% S:");
  lcd.print(solarV, 1);
}

void loop() {
  // Update LCD every 500ms so it doesnt slow down car control
  if (millis() - lastDisplayUpdate > 500) {
    updateLCD();
    lastDisplayUpdate = millis();
  }

  Dabble.processInput();

  uint16_t rawAngle  = GamePad.getAngle();
  uint8_t  rawRadius = GamePad.getRadius();

  uint16_t angle  = smoothAngle(rawAngle);
  uint8_t  radius = smoothRadius(rawRadius);

  int spd = map(radius, 4, 7, 80, 255);
  spd = constrain(spd, 80, 255);

  if (radius > 4) {
    if      (angle > 225 && angle <= 315) forward(spd);
    else if (angle > 45  && angle <= 135) backward(spd);
    else if (angle <= 45 || angle > 315)  turnRight(spd);
    else if (angle > 135 && angle <= 225) turnLeft(spd);
  }
  else {
    stopCar();
  }
}
