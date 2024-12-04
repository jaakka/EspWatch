



void PCF8591_Init() 
{
  pcf.analogWrite(255);   // We can set this to max output right away, power is controlled with pcf.enableDAC(true/false);
  pcf.enableDAC(true);    // Use this to enable and disable lightsensor power     # DELETE THIS AFTER POWER CONTROL HAS BEEN IMPLEMENTED ELSEWHERE
}


void PCF8591_Loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - last_pcf >= 200)
  {
    Serial.print("Hall1:");
    Serial.print(pcf.analogRead(0));
    Serial.print("    ");

    Serial.print("Hall2:");
    Serial.print(pcf.analogRead(1));
    Serial.print("    ");

    Serial.print("Lightsensor_value:");
    Serial.print(pcf.analogRead(2));

    Serial.println("");
    last_pcf = millis();
  }
}


















