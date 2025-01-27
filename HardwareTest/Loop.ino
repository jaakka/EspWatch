void loop() {
  GyroscopeUpdate();
  LcdUpdate();
  PulseLoop();
  TouchLoop();
  PCF8591_Loop();
}
