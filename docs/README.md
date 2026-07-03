# Solar RC Car with Bluetooth Control & Power Monitoring

A solar-assisted RC car controlled via the Dabble app (Bluetooth gamepad module) with 
real-time voltage, current, power, solar input, and battery percentage monitoring 
displayed live on an I2C LCD.

## Overview
This project combines embedded systems, power electronics, and wireless control. 
The car is driven using Dabble's gamepad joystick over Bluetooth (HC-05), while an 
INA226 sensor continuously measures battery voltage, current draw, and power 
consumption — all displayed live on an LCD, alongside a solar panel voltage reading 
taken through a resistor voltage divider.

## Features
- Gamepad/joystick control via the Dabble app over Bluetooth (HC-05)
- Smooth, averaged joystick input for stable driving (angle & radius smoothing)
- Real-time voltage, current, and power monitoring using INA226
- Live battery charge percentage estimation
- Live solar panel voltage reading via analog voltage divider
- All readings displayed on a 16x2 I2C LCD, updated every 500ms without blocking car control

## Components Used
| Component            | Purpose                                |
|-----------------------|------------------------------------------|
| Arduino Uno            | Main controller                          |
| HC-05 Bluetooth        | Wireless control via Dabble app          |
| INA226                 | Voltage/current/power sensing            |
| 16x2 I2C LCD           | Displays live readings                   |
| L298N Motor Driver     | Drives 2 DC motors                       |
| Solar Panel            | Auxiliary power source (voltage-monitored)|
| 2S Li-ion Battery      | Main power source                        |

## Wiring & Connections
Full pin-by-pin wiring is documented in [docs/wiring_connections.md](docs/wiring_connections.md).

## Code
Full Arduino sketch: [code/solar_car_final.ino](code/solar_car_final.ino)

Key libraries used:
- `Dabble.h` — Bluetooth gamepad control
- `Wire.h` — I2C communication
- `LiquidCrystal_I2C.h` — LCD display
- `INA226.h` — voltage/current/power sensing

### How control works
The Dabble app's gamepad module sends joystick angle and radius over Bluetooth. 
The code smooths these values using a 5-sample rolling average to avoid jittery 
movement, maps the radius to a motor speed (80–255 PWM), and converts the angle 
into one of four directions (forward, backward, turn left, turn right) via the 
L298N motor driver.

## Challenges & Debugging
This project involved real hardware debugging, documented in full in 
[docs/troubleshooting.md](docs/troubleshooting.md) — including an I2C address 
conflict on the INA226, float display formatting issues on AVR boards, and a 
floating sense-pin issue that caused incorrect voltage readings.

## Demo

### Live Stats — At Rest
![Stats at rest](images/stats_at_rest.png)

### Live Stats — In Motion
![Stats in motion](images/stats_in_motion.png)

### Car Driving
`images/car_driving.mp4` (click to download/view — GitHub doesn't auto-play mp4 in README)

### Voltage Monitoring While Working
`images/voltage_monitoring.mp4` (click to download/view — GitHub doesn't auto-play mp4 in README)

## What I Learned
- I2C communication and resolving address conflicts between multiple sensors
- Real-time sensor data processing without blocking control responsiveness
- Bluetooth-based gamepad control using the Dabble app
- Power monitoring circuit design and battery percentage estimation
- Reading analog sensor values safely via voltage dividers

## Future Improvements
- Add GPS or obstacle avoidance sensors
- Improve solar charging efficiency with a dedicated MPPT circuit
- Log power data over time for analysis