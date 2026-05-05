#include <Arduino.h>
#include <Wire.h>
#include <GyverINA.h>
#include <U8g2lib.h>

/*
   Warning!!! The INA226 shunt voltage measurement range is +/- 81.92 mV
   Using an INA226 module with a 0.1 Ohm shunt gives a maximum measurable current of I ~ 820 mA
   When using a different shunt, it is recommended to size it so the voltage drop does not exceed 82 mV!

   Example:
   Max expected current = 5 A
   Maximum shunt voltage drop = 80 mV
   Shunt resistance = 0.08 V / 5 A = 0.016 Ohm
   The shunt should be 0.016 Ohm (16 mOhm)
*/

// Create object: INA226 ina(shunt resistance, max expected current, I2C address);
// INA226 ina(0x41);              // Standard module settings but altered address
// INA226 ina(0.05f);             // Standard address and max current, but different shunt (0.05 Ohm)
// INA226 ina(0.05f, 1.5f);       // Standard address, different shunt (0.05 Ohm), and max expected current (1.5 A)
// INA226 ina(0.05f, 1.5f, 0x41); // Fully customizable variant with manual parameter specification
INA226 ina(0.002f, 5.0f);       // Use 2 mOhm shunt resistor (R002) and expected max current 5 A
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup() {
  // Open serial port
  Serial.begin(9600);

  // Initialize I2C bus for INA226 and OLED
  Wire.begin();
  u8g2.begin();

  Serial.print(F("INA226..."));

  // Check for and initialize INA226
  if (ina.begin()) {	// ina.begin(4, 5) // For ESP32/ESP8266 you can specify I2C pins
    Serial.println(F("connected!"));
  } else {
    Serial.println(F("not found!"));
    while (1);
  }

  // You can put it to sleep by calling .sleep(true), and wake it by calling .sleep(false)
  // ina.sleep(true);   // Put INA226 to sleep
  // ina.sleep(false);  // Wake INA226 up

  // INA226 supports built-in current measurement calibration via a special calibration value
  // After startup, the library automatically computes and writes the calibration value based on the input data
  // You can read this value using .getCalibration(); for modification and/or saving to EEPROM
  Serial.print(F("Calibration value: ")); Serial.println(ina.getCalibration());
  // You can then modify this value to match the actual shunt resistance and save it to EEPROM
  // To write the calibration value to INA226 use .setCalibration(value);
  // ina.setCalibration(ina.getCalibration() + 10); // Read-modify-write calibration value
  // You can also use .adjCalibration(offset) to adjust calibration without direct reading
  // ina.adjCalibration(10);  // Increase calibration value by 10
  // ina.adjCalibration(-20); // Decrease calibration value by 20
  // You can store an offset in EEPROM and load it into INA226 instead of the raw calibration value

  // To improve noise immunity, INA226 allows configuring sample time for voltage and current
  // INA226 captures a piece of the signal for the selected duration, increasing accuracy on noisy signals
  // By default the conversion takes 1100 µs, but it can be changed with .setSampleTime(channel, time);
  // See sample time options in the table (file INA226.h)
  ina.setSampleTime(INA226_VBUS, INA226_CONV_2116US);   // Increase voltage sample time twofold
  ina.setSampleTime(INA226_VSHUNT, INA226_CONV_8244US); // Increase current sample time eightfold

  // Built-in sample averaging is also available
  // Averaging applies to both voltage and current and proportionally increases conversion time
  // Recommended for noisy loads, set with .setAveraging(number of averages) (see table in INA226.h)
  ina.setAveraging(INA226_AVG_X4); // Enable built-in 4x averaging; default is no averaging 

  Serial.println("");
}

void loop() {
  float voltage = ina.getVoltage();
  float current = ina.getCurrent();
  float power = ina.getPower();

  // Serial output
  Serial.print(F("Voltage: "));
  Serial.print(voltage, 3);
  Serial.println(F(" V"));
  Serial.print(F("Current: "));
  Serial.print(current, 3);
  Serial.println(F(" A"));
  Serial.print(F("Power: "));
  Serial.print(power, 3);
  Serial.println(F(" W"));
  Serial.println("");

  // OLED display output
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB12_tr);
  char buf[16];

  dtostrf(voltage, 6, 3, buf);
  u8g2.drawStr(0, 14, "U:");
  u8g2.drawStr(24, 14, buf);
  u8g2.drawStr(88, 14, "V");

  dtostrf(current, 6, 3, buf);
  u8g2.drawStr(0, 30, "I:");
  u8g2.drawStr(24, 30, buf);
  u8g2.drawStr(88, 30, "A");

  dtostrf(power, 6, 3, buf);
  u8g2.drawStr(0, 46, "P:");
  u8g2.drawStr(24, 46, buf);
  u8g2.drawStr(88, 46, "W");

  u8g2.sendBuffer();

  delay(1000);
}
