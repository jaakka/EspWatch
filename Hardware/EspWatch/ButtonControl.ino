void BtnsInit()
{
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
}

bool ButtonDown()
{
   int sensorVal = digitalRead(BTN_DOWN);
   return (sensorVal == 1);
}

bool ButtonUp()
{
   int sensorVal = digitalRead(BTN_UP);
   return (sensorVal == 1);
}