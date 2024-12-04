void setup()
{
  LCD.begin();
  Serial.begin(9600);
  while (!Serial) {}

  LcdBootScreen("gyroscope",0);
  int allowed_try_times = 100;
  while(true) // Force gyroscope
  {
    if(allowed_try_times == 0){LcdBootScreen("fail",2); break;}
    if(GYRO.begin()){LcdBootScreen("ok",1); break;}
    allowed_try_times--;
    Serial.println("Failed to initialize IMU!");
    delay(10);
  }

  delay(500);

  LcdBootScreen("heartrate",0);
  allowed_try_times = 100;
  while(true)
  {
    if(allowed_try_times == 0){LcdBootScreen("fail",2); break;}
    if(HEARTRATE.begin()){LcdBootScreen("ok",1); break;}
    allowed_try_times--;
    Serial.println("Failed to initialize HeartRate Sensor!");
    delay(10);
  }

  if(HEARTRATE.setSamplingRate(kSamplingRate)) 
  { 
    Serial.println("HeartRate sensor initialized");
  }

  delay(500);

  LcdBootScreen("pcf",0);

  allowed_try_times = 100;
  while(true)
  {
    if(allowed_try_times == 0){LcdBootScreen("fail",2); break;}
    if(pcf.begin()){LcdBootScreen("ok",1); break;}
    allowed_try_times--;
    Serial.println("Failed to initialize PCF8591!");
    delay(10);
  }
  delay(500);
  LcdBootScreen("touch",0);
  touch.begin();
  BtnsInit();
  
  LcdStart();
  PCF8591_Init();
  HallInit();

  Serial.begin(9600);
}