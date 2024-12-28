#include "debug.h"


DEBUG::DEBUG(HEARTRATE& hr, GYROSCOPE& gyro, PCF& pcf) 
        : heartrate(hr), gyroscope(gyro), pcf(pcf)
        {
        DEBUG::printMenu();
        }



void DEBUG::printMenu() {
  
    Serial.println("");
    Serial.println("");


    Serial.println("Enter a number to select an action:");
    Serial.println("0: Print menu");
    Serial.println("1: Begin heart rate sensor");
    Serial.println("2: Test pulseDetected()");
    Serial.println("3: Enable heart rate sensor");
    Serial.println("4: Disable heart rate sensor");
    Serial.println("5: Set heart rate mode");
    Serial.println("6: Set heart rate sampling rate");
    Serial.println("7: Set heart rate sample averaging");
    Serial.println("8: Set heart rate LED current");
    Serial.println("9: Set heart rate pulse width");

    Serial.println("10: Begin gyroscope");
    Serial.println("11: Run gyroscope loop");

    Serial.println("12: Read acceleration value");
    Serial.println("13: Get acceleration sample rate");
    Serial.println("14: Check if gyro acceleration is available");

    Serial.println("15: Read gyroscope value");
    Serial.println("16: Get gyro sample rate");
    Serial.println("17: Check if gyro is available");

    Serial.println("18: Read gyroscope temperature");
    Serial.println("19: Get gyro temperature sample rate");
    Serial.println("20: Check if gyro temperature is available");

    Serial.println("21: Begin PCF8591");
    Serial.println("22: Read analog values from all PCF8591 pins, send anything to stop");
    Serial.println("23: Write analog value to PCF8591 DAC");
    Serial.println("24: Enable PCF8591 DAC");
    Serial.println("25: Disable PCF8591 DAC");


    Serial.println("");
    Serial.println("");
}


void DEBUG::loop() {
    if (Serial.available() > 0) {
        action = Serial.parseInt();

        switch (action) {
            case 0:
                DEBUG::printMenu();
                break;

            case 1:
                if (heartrate.begin()) 
                {
                    Serial.println("Heart rate sensor begun");
                } 
                else 
                {
                    Serial.println("Failed to begin heart rate sensor");
                }
                DEBUG::printMenu();
                break;


            case 2:
                Serial.println("Testing pulseDetected() ...");
                for (int i = 0; i < 1000; i++) {
                    Serial.println(heartrate.pulseDetected());
                }
                DEBUG::printMenu();
                break;


            case 3:
                if (heartrate.enableSensor()) 
                {
                    Serial.println("Heart rate sensor enabled");
                } 
                else 
                {
                    Serial.println("Failed to enable heart rate sensor");
                }
                DEBUG::printMenu();
                break;


            case 4:
                if (heartrate.disableSensor()) 
                {
                    Serial.println("Heart rate sensor disabled");
                }
                else 
                {
                    Serial.println("Failed to disable heart rate sensor");
                }
                DEBUG::printMenu();
                break;


            case 5:
                Serial.println("Enter 1 for heart rate only mode or 2 for SpO2 mode: ");
                while (Serial.available() <= 0) {delay(100);}
                
                mode = Serial.parseInt();
                if (mode == 1) 
                {
                    if (heartrate.setMode("MODE_HR_ONLY")) 
                    {
                        Serial.println("Heart rate mode set");
                    } 
                    else 
                    {
                        Serial.println("Failed to set heart rate mode");
                    }
                } 
                else if (mode == 2) 
                {
                    if (heartrate.setMode("MODE_SPO2")) 
                    {
                        Serial.println("Heart rate mode set");
                    } 
                    else 
                    {
                        Serial.println("Failed to set heart rate mode");
                    }
                } 
                else 
                {
                    Serial.println("Invalid mode");
                }
                DEBUG::printMenu();
                break;


            case 6:
                Serial.println("Enter a sampling rate (50, 100, 200, 400, 800, 1000, 1600, 3200): ");
                while (Serial.available() <= 0) {delay(100);}

                samplingRate = Serial.parseInt();

                if (heartrate.setSamplingRate(samplingRate)) 
                {
                    Serial.println("Heart rate sampling rate set");
                } 
                else 
                {
                    Serial.println("Failed to set heart rate sampling rate");
                }
                DEBUG::printMenu();
                break;


            case 7:
                Serial.println("Enter a sample averaging (0, 2, 4, 8, 16, 32): ");
                while (Serial.available() <= 0) {delay(100);}

                sampleAveraging = Serial.parseInt();

                if (heartrate.setSampleAveraging(sampleAveraging)) 
                {
                    Serial.println("Heart rate sample averaging set");
                } 
                else 
                {
                    Serial.println("Failed to set heart rate sample averaging");
                }
                DEBUG::printMenu();
                break;


            case 8:
                Serial.println("Enter 1 for red LED or 2 for IR LED: ");
                while (Serial.available() <= 0) {delay(100);}

                led = Serial.parseInt();
                if (led == 1) 
                {
                    Serial.println("Enter a current for the red LED (0-50) in 0.2mA steps: ");
                    while (Serial.available() <= 0) {delay(100);}

                    current = Serial.parseFloat();
                    if (heartrate.setLedCurrent("LED_RED", current)) 
                    {
                        Serial.println("Heart rate LED current for red set");
                    } 
                    else 
                    {
                        Serial.println("Failed to set heart rate LED current");
                    }
                }
                else if (led == 2) 
                {
                    Serial.println("Enter a current for the IR LED (0-50) in 0.2mA steps: ");
                    while (Serial.available() <= 0) {delay(100);}

                    current = Serial.parseFloat();
                    if (heartrate.setLedCurrent("LED_IR", current)) 
                    {
                        Serial.println("Heart rate LED current for IR set");
                    }
                    else 
                    {
                        Serial.println("Failed to set heart rate LED current");
                    }
                }
                else 
                {
                    Serial.println("Invalid LED");
                }
                DEBUG::printMenu();
                break;


            case 9:
                Serial.println("Enter a pulse width (69, 118, 215, 411): ");
                while (Serial.available() <= 0) {delay(100);}

                pulseWidth = Serial.parseInt();
                if (heartrate.setPulseWidth(pulseWidth)) 
                {
                    Serial.println("Heart rate pulse width set");
                } 
                else 
                {
                    Serial.println("Failed to set heart rate pulse width");
                }
                DEBUG::printMenu();
                break;


            case 10:
                if (gyroscope.begin()) 
                {
                    Serial.println("Gyroscope begun");
                } 
                else 
                {
                    Serial.println("Failed to begin gyroscope");
                }
                DEBUG::printMenu();
                break;


            case 11:
                gyroscope.loop();

                DEBUG::printMenu();
                break;


            case 12:
                if (gyroscope.readAcceleration(x_Acc, y_Acc, z_Acc)) 
                {
                    Serial.print("Acceleration x: ");
                    Serial.print(x_Acc);
                    Serial.print(" y: ");
                    Serial.print(y_Acc);
                    Serial.print(" z: ");
                    Serial.println(z_Acc);
                } 
                else 
                {
                    Serial.println("Failed to read acceleration");
                }
                DEBUG::printMenu();
                break;


            case 13:
                Serial.print("Acceleration sample rate: ");
                Serial.println(gyroscope.getAccelerationSampleRate());

                DEBUG::printMenu();
                break;


            case 14:
                if (gyroscope.accelerationAvailable()) 
                {
                    Serial.println("Acceleration available");
                } 
                else 
                {
                    Serial.println("Acceleration not available");
                }
                DEBUG::printMenu();
                break;


            case 15:
                if (gyroscope.readGyroscope(x_Gyro, y_Gyro, z_Gyro)) 
                {
                    Serial.print("Gyroscope x: ");
                    Serial.print(x_Gyro);
                    Serial.print(" y: ");
                    Serial.print(y_Gyro);
                    Serial.print(" z: ");
                    Serial.println(z_Gyro);
                } 
                else 
                {
                    Serial.println("Failed to read gyroscope");
                }
                DEBUG::printMenu();
                break;


            case 16:
                Serial.print("Gyroscope sample rate: ");
                Serial.println(gyroscope.getGyroscopeSampleRate());

                DEBUG::printMenu();
                break;


            case 17:
                if (gyroscope.gyroscopeAvailable()) 
                {
                    Serial.println("Gyroscope available");
                } 
                else 
                {
                    Serial.println("Gyroscope not available");
                }
                DEBUG::printMenu();
                break;


            case 18:
                if (gyroscope.readTemperature(temperature)) {
                    Serial.print("Temperature: ");
                    Serial.print(temperature);
                    Serial.println(" °C");
                } else {
                    Serial.println("Failed to read temperature");
                }
                DEBUG::printMenu();
                break;


            case 19:
                Serial.print("Temperature sample rate: ");
                Serial.println(gyroscope.getTemperatureSampleRate());

                DEBUG::printMenu();
                break;


            case 20:
                if (gyroscope.temperatureAvailable()) 
                {
                    Serial.println("Temperature available");
                } 
                else 
                {
                    Serial.println("Temperature not available");
                }
                DEBUG::printMenu();
                break;


            case 21:
                if (pcf.begin()) 
                {
                    Serial.println("PCF8591 begun");
                } 
                else 
                {
                    Serial.println("Failed to begin PCF8591");
                }
                DEBUG::printMenu();
                break;


            case 22:
                while (Serial.available() <= 0) {
                    Serial.print("Analog values: ");
                    for (uint8_t i = 0; i < 4; i++) {
                        Serial.print(pcf.analogRead(i));
                        Serial.print(" ");
                    }
                    Serial.println("");
                    delay(200);
                }
                // Clear buffer since the input is only used to stop the loop
                while (Serial.available() > 0) {
                    Serial.read();
                }
                DEBUG::printMenu();
                break;

            
            case 23:
                Serial.println("Enter a value to write to the DAC (0-255): ");
                while (Serial.available() <= 0) {delay(100);}

                pcf.analogWrite(Serial.parseInt());
                

                DEBUG::printMenu();
                break;


            case 24:
                pcf.DAC_enabled(true);
                Serial.println("PCF8591 DAC enabled");
                DEBUG::printMenu();
                break;

            case 25:
                pcf.DAC_enabled(false);
                Serial.println("PCF8591 DAC disabled");
                DEBUG::printMenu();
                break;


            default:
                Serial.println("Invalid action");

                DEBUG::printMenu();
                break;
        }
    }
}