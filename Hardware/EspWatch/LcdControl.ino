void LcdBootScreen(String device, int color)
{
  LCD.setRotation(3);
  LCD.fillScreen(TFT_WHITE);
  LCD.setTextColor(TFT_BLACK, TFT_WHITE);
  LCD.drawCentreString("Starting..",120,45 + 30*0,4);
  if(color == 0){LCD.setTextColor(TFT_WHITE, TFT_BLACK);}
  if(color == 1){LCD.setTextColor(TFT_WHITE, TFT_GREEN);}
  if(color == 2){LCD.setTextColor(TFT_WHITE, TFT_RED);}
  LCD.drawCentreString(device,120,80 + 30*0,4);
}

void LcdStart()
{
  LCD.setRotation(3);
  LCD.fillScreen(TFT_WHITE);
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