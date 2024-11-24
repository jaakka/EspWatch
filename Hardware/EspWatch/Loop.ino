void loop() {
  GyroscopeUpdate();
  LcdUpdate();
  PulseLoop();
  PCF8591_Loop();
}