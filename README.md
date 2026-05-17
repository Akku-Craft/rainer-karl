# Akku-Craft Arduino INA226 Hand Crank Generator

An Arduino-based measurement system for a hand generator (crank dynamo) with INA226 current sensor and OLED display.

## Project Overview

This project measures and displays real-time parameters of a hand generator:

- **Voltage (U)** - in Volts
- **Current (I)** - in Amperes
- **Power (P)** - in Watts
- **Energy (E)** - in Watt-Hours (Wh)

The measured values are displayed simultaneously on a small OLED display and via serial console.

### The Hand Crank Generator

![Hand Crank Generator Photo](kurbel.png)

## Hardware

### Components

- **Arduino Uno R3**
- **INA226 Current Sensor**
- **SSD1306 128 x 64 OLED Display** - [Buy on Amazon](https://amzn.eu/d/0i8YL0yu)
- **Hand Crank Generator** - [Buy on AliExpress](https://de.aliexpress.com/item/1005004269696080.html)

### Schematics

> The schematics will be available soon.

## Features

### Measurements

- **Real-time Values**: Continuous capture of all parameters
- **Energy Calculation**: Cumulative storage of generated energy based on power and time
- **Threshold Detection**: Measurements start at voltages > 3.0 V

### Display

- **OLED Display**: Compact display for portable use
- **Serial Monitor (9600 Baud)**: Detailed data output for analysis and debugging

## Software

### Libraries Used

- **GyverINA** - INA226 sensor control
- **U8g2lib** - OLED display driver
- **Wire** - I2C communication

### Installing Dependencies

```bash
# Via Arduino IDE or PlatformIO
# Search term: GyverINA
# Search term: U8g2
```

## Configuration

### Adjusting Shunt Resistance

The file `src/main.cpp` contains the shunt configuration (around line 18):

```cpp
INA226 ina(0.002f, 5.0f);  // 2 mOhm shunt, max. 5 A
```

**Calculation for other shunt values:**

- Max. expected current: 5 A
- Max. voltage drop across shunt: 80 mV
- Shunt resistance = 0.08 V / 5 A = 0.016 Ω

### I2C Addresses

- **INA226** (default): 0x40
- **OLED Display** (default): 0x3C

## Upload & Operation

### Using Arduino IDE

1. Open Arduino IDE
2. File → Examples → GyverINA → ... load example
3. Install required libraries
4. Board: Arduino Uno
5. Select port
6. Upload

### Using PlatformIO

```bash
pio run --target upload
```

### Serial Monitor

- Baud rate: **9600**
- Shows: Sensor status, calibration value, live measurement data

## Measurement Results

The system displays the following information:

**On OLED Display:**

```
U: 12.5 V    I: 0.45 A
P:  5.6 W    E: 0.12 Wh
```

**In Serial Monitor (example):**

```
INA226...connected!
Calibration value: 20480
U: 12.5 V, I: 450 mA, P: 5.6 W, E: 0.12 Wh
```

## Important Notes

⚠️ **Warning! INA226 Shunt Voltage Measurement Range:**

- Maximum shunt voltage: ±81.92 mV
- With 0.1 Ω shunt: max. ~820 mA measurable
- With 0.002 Ω shunt: max. ~5 A measurable
- Shunt voltage should not exceed 80 mV!

## Troubleshooting

| Problem                       | Solution                                                   |
| ----------------------------- | ---------------------------------------------------------- |
| INA226 not found              | Check I2C address, call Wire.begin()                       |
| Display stays black           | Check OLED I2C address (default: 0x3C), verify connections |
| Measurement values inaccurate | Adjust calibration value, check shunt resistance           |
| No data in Serial Monitor     | Set baud rate to 9600, check USB cable                     |

## License

This project uses open-source libraries. See the documentation of the used libraries for details.
