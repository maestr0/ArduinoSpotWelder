#include <Arduino.h>

#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 255;
const int colorB = 0;

const int weldButtonPin = 3; // the number of the pushbutton pin
const int menuButtonPin = 2; // the number of the pushbutton pin

// Variables will change:
int ledState = HIGH;        // the current state of the output pin
int buttonState;            // the current reading from the input pin
int lastButtonState = HIGH; // the previous reading from the input pin

// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50;   // the debounce time; increase if the output flickers

int profileIndex = 0;

int profiles[][3] = {{50, 500, 500},
                     {100, 500, 500},
                     {150, 500, 500}};

void updateProfileScreen();

void setup()
{

    pinMode(menuButtonPin, INPUT_PULLUP);
    pinMode(weldButtonPin, INPUT_PULLUP);
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);

    lcd.setRGB(colorR, colorG, colorB);

    // Print a message to the LCD.
    lcd.print("ShittySpotWelder");
    lcd.setCursor(0, 1);
    lcd.print("made by Pawel");
    delay(300);
    updateProfileScreen();
}

void switchProfile()
{
    // read the state of the switch into a local variable:
    int reading = digitalRead(menuButtonPin);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited
    // long enough since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState)
    {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) < debounceDelay)
    {
        // whatever the reading is at, it's been there for longer
        // than the debounce delay, so take it as the actual current state:

        // if the button state has changed:
        if (reading != buttonState)
        {
            buttonState = reading;

            // only toggle the LED if the new button state is HIGH
            if (buttonState == HIGH)
            {
                ledState = !ledState;
                profileIndex++;
                profileIndex = profileIndex % (sizeof(profiles) / sizeof(profiles[0]));

                updateProfileScreen();
            }
        }

        // set the LED:
        // digitalWrite(ledPin, ledState);

        // save the reading. Next time through the loop,
        // it'll be the lastButtonState:
        lastButtonState = reading;
    }
}

void updateProfileScreen()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Profile #" + String(profileIndex));
    lcd.setCursor(0, 1);
    lcd.print("PW" + String(profiles[profileIndex][0]) + " P" + String(profiles[profileIndex][1]) + " W" + String(profiles[profileIndex][2]));
}

void loop()
{
    switchProfile();
}
