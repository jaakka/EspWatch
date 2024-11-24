constexpr int hall_pwr_pin = 18;

void HallInit()
{
  pinMode(hall_pwr_pin, OUTPUT);

  analogWrite(hall_pwr_pin, 255);
}