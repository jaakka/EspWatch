void PCF8591_Init() 
{
//  pcf.enableDAC(true);
//  pcf.analogWrite(255);
}


/* Ensimmäinen debug-toiminta, tulee muuttumaan*/

/* Using example code from the PCF8591 Adafruit library*/


constexpr float ADC_REFERENCE_VOLTAGE = 3.3;

elapsedMillis luku;


void PCF8591_Loop()
{
  if (luku >= 200)
  {
    Serial.print(int_to_volts(pcf.analogRead(0), 8, ADC_REFERENCE_VOLTAGE));
    Serial.print("V, ");
    Serial.print(int_to_volts(pcf.analogRead(1), 8, ADC_REFERENCE_VOLTAGE));
    Serial.print("V, ");
    Serial.print(int_to_volts(pcf.analogRead(2), 8, ADC_REFERENCE_VOLTAGE));
    Serial.print("V, ");

    Serial.println("");

    luku = 0;
  }
}

float int_to_volts(uint16_t dac_value, uint8_t bits, float logic_level) {
  return (((float)dac_value / ((1 << bits) - 1)) * logic_level);
}