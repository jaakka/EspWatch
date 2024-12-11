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

    // If swiping left or right, change the screen
    if (touch.data.gestureID == SWIPE_LEFT) {
      current_screen = current_screen - 1;
    }
    else if (touch.data.gestureID == SWIPE_RIGHT) {
      current_screen = current_screen +1;
    }

    if (current_screen < 0) {
      current_screen = 0;
    }
    else if (current_screen > 1) {
      current_screen = 1;
    }
  }
}