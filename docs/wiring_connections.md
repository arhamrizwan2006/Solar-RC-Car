## Components Used
- Arduino Uno (or Nano — confirm your board)
- HC-05 Bluetooth Module
- INA226 Current/Voltage Sensor
- 16x2 LCD Display (I2C)
- L298N Motor Driver
- 2x DC Motors
- Solar Panel
- Battery (2S Li-ion, ~7.4V nominal / 8.4V full charge)

## Wiring / Pin Connections

### HC-05 Bluetooth Module
- VCC → 5V
- GND → GND
- TX → Arduino D10
- RX → Arduino D11

### L298N Motor Driver
- ENA → Arduino D3
- IN1 → Arduino D2
- IN2 → Arduino D4
- IN3 → Arduino D5
- IN4 → Arduino D7
- ENB → Arduino D6
- 12V (motor power in) → Battery VIN-
- GND → Battery- and common Arduino GND

### LCD (I2C)
- VCC → 5V
- GND → GND
- SDA → Arduino A4
- SCL → Arduino A5
- I2C Address: 0x27

### INA226 (I2C, Power Monitoring)
- VCC → 5V
- GND → GND
- SDA → Arduino A4 (shared I2C bus with LCD)
- SCL → Arduino A5 (shared I2C bus with LCD)
- I2C Address: 0x44
- VIN+ → Battery+
- VIN- → L298N 12V input
- VBS → L298N 12V input

### Solar Panel (Voltage Sensing via Voltage Divider)
- Solar(+) → 10kΩ resistor → A0 → 4.7kΩ resistor → GND (voltage divider to safely read solar panel voltage)
- Solar(-) → GND

### Power Distribution
- Arduino 5V → L298N 5V rail / USB power
- Common GND across Arduino, L298N, HC-05, INA226, LCD, and battery negative