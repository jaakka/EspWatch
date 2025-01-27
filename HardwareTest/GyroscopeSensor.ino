constexpr int gyroIntPin = 23;


void GyroscopeUpdate()
{
  if(GYRO.gyroscopeAvailable()) 
  {
    GYRO.readGyroscope(gyro_x, gyro_y, gyro_z);
    GYRO.readAcceleration(gyro_ax,gyro_ay,gyro_az);
    //Serial.println(String(gyro_ax) + ", "+String(gyro_ay) + ", "+String(gyro_az));
  }
}
