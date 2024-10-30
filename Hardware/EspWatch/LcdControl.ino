void LcdStart()
{
    LCD.setRotation(3);
    LCD.fillScreen(TFT_WHITE);
    LCD.drawCentreString("Gyroscope",120,15 + 30*0,4);
    LCD.drawCentreString("Heartrate",120,15 + 30*3,4);
}

void LcdUpdate()
{
   if (last_write + 100 < millis()) {
    last_write = millis();

    LCD.setTextColor(TFT_WHITE, TFT_WHITE);
    LCD.drawString(last_print, 20, 15 + 30*1,4);
    LCD.drawString(last_print2, 20, 15 + 30*2,4);
    LCD.drawString(last_print3, 15, 15 + 30*4,4);
    LCD.drawString(last_print4, 50, 15 + 30*5,4);

    LCD.setTextColor(TFT_BLACK, TFT_WHITE);
    last_print = String(gyro_ax) + ", "+String(gyro_ay) + ", "+String(gyro_az);
    LCD.drawString(last_print, 20, 15 + 30*1,4);

    last_print2 = String(gyro_x) + ", "+String(gyro_y) + ", "+String(gyro_z);
    LCD.drawString(last_print2, 20, 15 + 30*2,4);

    last_print3 = String(pulse)+"bpm / AVG: "+String(avg_pulse)+"bpm";
    LCD.drawString(last_print3, 15, 15 + 30*4,4);

    last_print4 = "spo2: "+String(spo)+" / "+String(avg_spo)+"%";
    LCD.drawString(last_print4, 50, 15 + 30*5,4);
  }
}