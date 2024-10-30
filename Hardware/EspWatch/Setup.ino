void setup()
{
  int allowed_try_times = 100;
  while(!GYRO.begin() && allowed_try_times > 0) // Force gyroscope
  {
    allowed_try_times--;
    Serial.println("Failed to initialize IMU!");
    delay(10);
  }

  allowed_try_times = 100;
  while(!HEARTRATE.begin() && allowed_try_times > 0)
  {
    allowed_try_times--;
    Serial.println("Failed to initialize HeartRate Sensor!");
  }

  if(HEARTRATE.setSamplingRate(kSamplingRate)) 
  { 
    Serial.println("HeartRate sensor initialized");
  }
  BtnsInit();
  LCD.begin();
  LcdStart();

  Serial.begin(9600);
}