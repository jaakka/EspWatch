void setup()
{
  Serial.begin(9600);
  while (!Serial) {}

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
    delay(10);
  }

  if(HEARTRATE.setSamplingRate(kSamplingRate)) 
  { 
    Serial.println("HeartRate sensor initialized");
  }

  allowed_try_times = 100;
  while(!pcf.begin() && allowed_try_times > 0)
  {
    allowed_try_times--;
    Serial.println("Failed to initialize PCF8591!");
    delay(10);
  }

  touch.begin();
  BtnsInit();
  LCD.begin();
  LcdStart();
  PCF8591_Init();
  HallInit();

  Serial.begin(9600);
}