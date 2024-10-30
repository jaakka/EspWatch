void GyroscopeUpdate()
{
  if(GYRO.gyroscopeAvailable()) 
  {
    GYRO.readGyroscope(gyro_x, gyro_y, gyro_z);
    GYRO.readAcceleration(gyro_ax,gyro_ay,gyro_az);
  }
}