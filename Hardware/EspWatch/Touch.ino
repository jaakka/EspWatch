void TouchLoop()
{
  if (touch.available()) 
  {
    Serial.print(touch.gesture());
    Serial.print("\t");
    Serial.print(touch.data.points);
    Serial.print("\t");
    Serial.print(touch.data.event);
    Serial.print("\t");
    Serial.print(touch.data.x);
    Serial.print("\t");
    Serial.println(touch.data.y);
  }
}