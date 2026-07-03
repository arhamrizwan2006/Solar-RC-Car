## Issues Solved During Development

1. **INA226 not detected on default I2C address**
   - Problem: Sensor wasn't responding when using the default address (0x40).
   - Cause: This particular board's address pins were configured differently, giving it address 0x44 instead.
   - Fix: Scanned the I2C bus, identified the correct address and initialized the sensor with `INA226 ina226(0x44)`.

2. **LCD showing incorrect/garbled float values**
   - Problem: Voltage, current and power values displayed incorrectly on the LCD.
   - Cause: AVR-based Arduino boards (like the Uno) don't support `%f` formatting in `sprintf()`.
   - Fix: Used the LCD library's built-in float printing (`lcd.print(value, decimalPlaces)`) instead of manually formatting strings with sprintf.

3. **Near-zero bus voltage readings from INA226**
   - Problem: INA226 reported almost 0V even though the battery was properly connected.
   - Cause: The VBS (bus voltage sense) pin was left floating/unconnected.
   - Fix: Connected VBS directly to the same voltage rail as VIN- (L298N 12V input), giving the sensor a proper reference to measure against.

4. **Shunt current limit configuration**
   - Problem: Needed to make sure current readings stayed accurate and didn't clip.
   - Fix: Explicitly configured the shunt using `ina226.setMaxCurrentShunt(0.8, 0.1)` — 0.8A max expected current with a 0.1Ω shunt resistor, matching the actual hardware setup.