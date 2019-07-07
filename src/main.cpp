#include <Arduino.h>

#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 255;
const int colorB = 0;

const int buttonPin = 2; // the number of the pushbutton pin
const int ledPin = 13;   // the number of the LED pin

// Variables will change:
int ledState = HIGH;       // the current state of the output pin
int buttonState;           // the current reading from the input pin
int lastButtonState = LOW; // the previous reading from the input pin

// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50;   // the debounce time; increase if the output flickers

int profileIndex = -1;

int profiles[3][3] = {{100, 200, 300},
                      {400, 50, 600},
                      {400, 210, 320}};

void setup()
{

    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);

    lcd.setRGB(colorR, colorG, colorB);

    // Print a message to the LCD.
    lcd.print("ShittySpotWelder");
    lcd.setCursor(0, 1);
    lcd.print("made by Pawel");
    delay(300);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Profile #" + String(profileIndex));
}

void switchProfile()
{
    // read the state of the switch into a local variable:
    int reading = digitalRead(buttonPin);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited
    // long enough since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState)
    {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) + debounceDelay) //put  greater than sign instead of plus (+). i cant do in youtube description{
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

                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Profile #" + String(profileIndex));
                lcd.setCursor(0, 1);
                lcd.print("H" + String(profiles[profileIndex][0]) + " C" + String(profiles[profileIndex][1]) + " W" + String(profiles[profileIndex][2]));
            }
        }

    // set the LED:
    // digitalWrite(ledPin, ledState);

    // save the reading. Next time through the loop,
    // it'll be the lastButtonState:
    lastButtonState = reading;
}

void loop()
{
    switchProfile();
}
