#include "calculatorapp.h"

void CalculatorApp::init() {
    enteredNumber = "";
    savedNumber1 = ""; // Initialize the saved number
    savedNumber2 = ""; // Initialize the saved number
    operatorSymbol = ""; // Initialize the operator symbol
    isEnteringNumber = true;
    wasTouched = false; // Initialize the previous touch state
    currentScreen = 0; // Start with the calculator screen
    showresult = false;
}

void CalculatorApp::handleApplication() {
    if (touch.userSwipeUp()) {
        exit_application = true;
        enteredNumber = ""; // Reset entered numbers when exiting the app
        savedNumber1 = ""; // Reset saved number when exiting the app
        savedNumber2 = ""; // Reset second saved number when exiting the app
        operatorSymbol = ""; // Reset operator symbol when exiting the app
    }

    if (touch.userSwipeLeft()) {
        if (savedNumber1.isEmpty()) {
            savedNumber1 = enteredNumber; // Save the entered number only if savedNumber1 is empty
        } else if (savedNumber2.isEmpty()) {
            savedNumber2 = enteredNumber; // Save the entered number to savedNumber2 if savedNumber1 is not empty
        }
        enteredNumber = ""; // Reset entered number
        currentScreen = 1; // Switch to operator screen
    } else if (touch.userSwipeRight()) {
        currentScreen = 0; // Switch back to calculator screen
        showresult = false;
    }

    if (currentScreen == 0) {
        bool isTouched = touch.userTouch();
        if (isEnteringNumber && isTouched && !wasTouched) {
            int touchX = touch.x;
            int touchY = touch.y;

            // Assuming a simple layout where numbers are arranged in a grid
            int number = -1;
            if (touchY >= SCREEN_HEIGHT / 2 - 50 && touchY < 3 * SCREEN_HEIGHT / 4 - 50) {
                if (touchX < SCREEN_WIDTH / 5) number = 1;
                else if (touchX < 2 * SCREEN_WIDTH / 5) number = 2;
                else if (touchX < 3 * SCREEN_WIDTH / 5) number = 3;
                else if (touchX < 4 * SCREEN_WIDTH / 5) number = 4;
                else number = 5;
            } else if (touchY >= 3 * SCREEN_HEIGHT / 4 - 50 && touchY < SCREEN_HEIGHT - 50) {
                if (touchX < SCREEN_WIDTH / 5) number = 6;
                else if (touchX < 2 * SCREEN_WIDTH / 5) number = 7;
                else if (touchX < 3 * SCREEN_WIDTH / 5) number = 8;
                else if (touchX < 4 * SCREEN_WIDTH / 5) number = 9;
                else number = 0;
            } else if (touchY >= SCREEN_HEIGHT - 50) {
                if (touchX >= 2 * SCREEN_WIDTH / 5 && touchX < 3 * SCREEN_WIDTH / 5) {
                    // Handle arrow touch to remove the last added number
                    if (!savedNumber2.isEmpty()) {
                        savedNumber2.remove(savedNumber2.length() - 1);
                        Serial.println("Backspace"); // Write the backspace action to the serial monitor
                    } else if (!operatorSymbol.isEmpty()) {
                        operatorSymbol.remove(operatorSymbol.length() - 1);
                        Serial.println("Backspace"); // Write the backspace action to the serial monitor
                    } else if (!savedNumber1.isEmpty()) {
                        savedNumber1.remove(savedNumber1.length() - 1);
                        Serial.println("Backspace"); // Write the backspace action to the serial monitor
                    } else if (!enteredNumber.isEmpty()) {
                        enteredNumber.remove(enteredNumber.length() - 1);
                        Serial.println("Backspace"); // Write the backspace action to the serial monitor
                    }
                }
            }

            if (number != -1) {
                if (!savedNumber1.isEmpty() && !operatorSymbol.isEmpty()) {
                    savedNumber2 += String(number);
                } else {
                enteredNumber += String(number);
                }
                Serial.println(number); // Write the selected number to the serial monitor
            }
        }
        wasTouched = isTouched; // Update the previous touch state
    } else if (currentScreen == 1) {
        bool isTouched = touch.userTouch();
        if (isTouched && !wasTouched) {
            int touchX = touch.x;
            int touchY = touch.y;

            // Assuming a simple layout where operators are arranged in a single row
            String selectedOperator = "";
            if (touchY >= SCREEN_HEIGHT / 2 - 50 && touchY < SCREEN_HEIGHT / 2 + 50) {
                if (touchX < SCREEN_WIDTH / 6) selectedOperator = "C";
                else if (touchX < 2 * SCREEN_WIDTH / 6) selectedOperator = "+";
                else if (touchX < 3 * SCREEN_WIDTH / 6) selectedOperator = "-";
                else if (touchX < 4 * SCREEN_WIDTH / 6) selectedOperator = "*";
                else if (touchX < 5 * SCREEN_WIDTH / 6) selectedOperator = "/";
                else selectedOperator = "=";
            } else if (touchY >= SCREEN_HEIGHT - 50) {
                if (touchX >= 2 * SCREEN_WIDTH / 5 && touchX < 3 * SCREEN_WIDTH / 5) {
                    // Handle arrow touch to remove the last added number
                    if (!savedNumber2.isEmpty()) {
                        savedNumber2.remove(savedNumber2.length() - 1);
                        Serial.println("Backspace"); // Write the backspace action to the serial monitor
                    } else if (!operatorSymbol.isEmpty()) {
                        operatorSymbol.remove(operatorSymbol.length() - 1);
                        Serial.println("Backspace"); // Write the backspace action to the serial monitor
                    } else if (!savedNumber1.isEmpty()) {
                        savedNumber1.remove(savedNumber1.length() - 1);
                        Serial.println("Backspace"); // Write the backspace action to the serial monitor
                    } else if (!enteredNumber.isEmpty()) {
                        enteredNumber.remove(enteredNumber.length() - 1);
                        Serial.println("Backspace"); // Write the backspace action to the serial monitor
                    }
                }
            }

            if (!selectedOperator.isEmpty()) {
                if (selectedOperator == "C") {
                    enteredNumber = ""; // Clear the entered numbers
                    savedNumber1 = ""; // Clear the saved number
                    savedNumber2 = ""; // Clear the second saved number
                    operatorSymbol = ""; // Clear the operator symbol
                    Serial.println("Clear"); // Write the clear action to the serial monitor
                } else if (selectedOperator == "=") {
                    if (!savedNumber1.isEmpty() && !savedNumber2.isEmpty() && !operatorSymbol.isEmpty()) {
                        // Perform the calculation if all required values are present
                        float result = 0;
                        float num1 = savedNumber1.toFloat();
                        float num2 = savedNumber2.toFloat();
                        if (operatorSymbol == "+") {
                            result = num1 + num2;
                        } else if (operatorSymbol == "-") {
                            result = num1 - num2;
                        } else if (operatorSymbol == "*") {
                            result = num1 * num2;
                        } else if (operatorSymbol == "/") {
                            result = num1 / num2;
                        }
                        enteredNumber = String(result); // Set the result as the entered number
                        savedNumber1 = ""; // Clear the saved number
                        savedNumber2 = ""; // Clear the second saved number
                        operatorSymbol = ""; // Clear the operator symbol
                        showresult = true;
                        Serial.println(result); // Write the result to the serial monitor
                    }
                } else if (operatorSymbol.isEmpty()) {
                    operatorSymbol = selectedOperator; // Set the operator symbol if it is empty
                    Serial.println(operatorSymbol); // Write the selected operator to the serial monitor
                }
            }
        }
        wasTouched = isTouched; // Update the previous touch state
    }
}

void CalculatorApp::handleApplicationBackground() {
    // No background tasks for now
}

void CalculatorApp::drawApplication(int x, int y, float scale) {
    const float scaled_width = ((float)SCREEN_WIDTH / 10) * scale;
    const float scaled_height = ((float)SCREEN_HEIGHT / 10) * scale;

    const float abs_middle_x = x + scaled_width / 2;
    const float abs_middle_y = y + scaled_height / 2;

    frame.fillCircle(abs_middle_x, abs_middle_y, scaled_width / 2 + 2, rgb(255, 255, 255));

    float font_scale = (scale / 10);
    frame.setTextSize(font_scale);

    if (currentScreen == 0) {
        // Draw entered number
        frame.setTextColor(rgb(0, 0, 0));
    
        // Draw saved numbers and operator symbol if they exist
        if (!savedNumber1.isEmpty() || !operatorSymbol.isEmpty() || !savedNumber2.isEmpty()) {
            String displayString = savedNumber1 + " " + operatorSymbol + " " + savedNumber2;
            frame.drawString(displayString, abs_middle_x - 50, abs_middle_y - SCREEN_HEIGHT / 4, 4);
        }
        else if (!enteredNumber.isEmpty()) {
            frame.drawString(enteredNumber, abs_middle_x - 50, abs_middle_y - SCREEN_HEIGHT / 4, 4);
        }

        // Draw number grid
        frame.setTextColor(rgb(0, 0, 0));
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 5; j++) {
                int number = i * 5 + j + 1;
                if (number == 10) number = 0;
                if (number > 10) continue;
                int x_pos = j * (SCREEN_WIDTH / 5) + (SCREEN_WIDTH / 10);
                int y_pos = (i + 2) * (SCREEN_HEIGHT / 4) + (SCREEN_HEIGHT / 8) - 50;
                frame.drawString(String(number), x_pos, y_pos, 4);
            }
        }

        // Draw arrow below the grid
        frame.setTextColor(rgb(0, 0, 0));
        frame.drawString("<-", 2 * (SCREEN_WIDTH / 5) + (SCREEN_WIDTH / 10), SCREEN_HEIGHT - 40, 4);
        
    } else if (currentScreen == 1) {
        // Draw second screen with saved number + operator + second saved number
        frame.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, rgb(255, 255, 255));
        frame.setTextColor(rgb(0, 0, 0));
        if (showresult == true) {
            frame.drawString(enteredNumber, abs_middle_x - 50, abs_middle_y - SCREEN_HEIGHT / 4, 4);
        } else if (!savedNumber1.isEmpty() || !operatorSymbol.isEmpty() || !savedNumber2.isEmpty()) {
            String displayString = savedNumber1 + " " + operatorSymbol + " " + savedNumber2;
            frame.drawString(displayString, abs_middle_x - 50, abs_middle_y - SCREEN_HEIGHT / 4, 4);
        }

        // Draw operator grid
        frame.setTextColor(rgb(0, 0, 0));
        const char* operators[] = {"C", "+", "-", "*", "/", "="};
        for (int j = 0; j < 6; j++) {
            int x_pos = j * (SCREEN_WIDTH / 6) + (SCREEN_WIDTH / 12);
            int y_pos = SCREEN_HEIGHT / 2;
            frame.drawString(operators[j], x_pos, y_pos, 4);
        }

        // Draw arrow below the grid
        frame.setTextColor(rgb(0, 0, 0));
        frame.drawString("<-", 2 * (SCREEN_WIDTH / 5) + (SCREEN_WIDTH / 10), SCREEN_HEIGHT - 40, 4);
    }
}

void CalculatorApp::drawApplicationIcon(int x, int y, float scale) {
    float scaled_width = ((float)SCREEN_WIDTH / 10) * scale;
    float scaled_height = ((float)SCREEN_HEIGHT / 10) * scale;
    frame.fillCircle(x, y, scaled_width / 2, rgb(255, 255, 255));

    // Make the icon 10% smaller
    float icon_width = scaled_width * 0.54; // 0.6 * 0.9
    float icon_height = scaled_height * 0.72; // 0.8 * 0.9
    float icon_x = x - icon_width / 2;
    float icon_y = y - icon_height / 2;

    frame.fillRect(icon_x, icon_y, icon_width, icon_height, rgb(0, 0, 0));

    float screen_height = icon_height * 0.3;
    frame.fillRect(icon_x + icon_width * 0.1, icon_y + icon_height * 0.1, icon_width * 0.8, screen_height, rgb(255, 255, 255));

    float button_size = icon_width * 0.2;
    float button_spacing = icon_width * 0.1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            float button_x = icon_x + icon_width * 0.1 + j * (button_size + button_spacing);
            float button_y = icon_y + icon_height * 0.5 + i * (button_size + button_spacing);
            frame.fillRect(button_x, button_y, button_size, button_size, rgb(255, 255, 255));
        }
    }
}

bool CalculatorApp::areAvailableOnMenu() {
    return true;
}
