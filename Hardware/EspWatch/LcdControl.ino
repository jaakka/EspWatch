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
    LCD.fillScreen(TFT_WHITE);

    if (current_screen == 0) {
      // Screen 1 Gyroscope
      LCD.drawString(last_print1, 40, 15 + 30*4, 4);
      LCD.drawString(last_print2, 40, 15 + 30*5, 4);
      LCD.drawString(last_print3, 50, 15 + 30*2, 4);

      LCD.setTextColor(TFT_BLACK, TFT_WHITE);
      last_print1 = String(gyro_ax) + ", " + String(gyro_ay) + ", " + String(gyro_az);
      LCD.drawString(last_print1, 40, 15 + 30*4, 4);

      last_print2 = String(gyro_x) + ", " + String(gyro_y) + ", " + String(gyro_z);
      LCD.drawString(last_print2, 40, 15 + 30*5, 4);

      last_print3 = "Gyroscope";
      LCD.drawString(last_print3, 50, 15 + 30*2, 4);
    } 
    else if (current_screen == 1) {
      // Screen 2 Heartrate
      LCD.drawString(last_print4, 15, 15 + 30*4, 4);
      LCD.drawString(last_print5, 40, 15 + 30*5, 4);
      LCD.drawString(last_print6, 50, 15 + 30*2, 4);

      LCD.setTextColor(TFT_BLACK, TFT_WHITE);
      last_print4 = String(pulse) + "bpm / AVG: " + String(avg_pulse) + "bpm";
      LCD.drawString(last_print4, 15, 15 + 30*4, 4);

      last_print5 = "spo2: " + String(spo) + " / " + String(avg_spo) + "%";
      LCD.drawString(last_print5, 40, 15 + 30*5, 4);

      last_print6 = "Heartrate";
      LCD.drawString(last_print6, 50, 15 + 30*2, 4);
    }
  }
}