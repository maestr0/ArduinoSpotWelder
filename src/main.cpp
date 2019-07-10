#include <Arduino.h>

#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 255;
const int colorB = 0;

const int weldButtonPin = 3; // the number of the pushbutton pin
const int menuButtonPin = 2; // the number of the pushbutton pin
const int ssrPin = 7;

// Variables will change:
int ssrButtonState = HIGH; // the current state of the output pin
int lastSsrState = HIGH;   // the current state of the output pin

int menuButtonState;            // the current reading from the input pin
int lastMenuButtonState = HIGH; // the previous reading from the input pin

// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;    // the last time the output pin was toggled
unsigned long lastSsrDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 100;      // the debounce time; increase if the output flickers

int profileIndex = 0;

int profiles[][3] = {{50, 500, 500},
                     {100, 500, 500},
                     {1000, 100, 2000}};

void updateProfileScreen();
void activateSsr();
void switchProfile();

void setup()
{

    pinMode(menuButtonPin, INPUT_PULLUP);
    pinMode(weldButtonPin, INPUT_PULLUP);
    pinMode(ssrPin, OUTPUT);
    digitalWrite(ssrPin, LOW);

    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);
    lcd.print("Spot Welder v0.1");
    lcd.setCursor(0, 1);
    lcd.print("made by Pawel");
    delay(100);
    updateProfileScreen();
}

void loop()
{
    switchProfile();
    activateSsr();
}

void switchProfile()
{
    // read the state of the switch into a local variable:
    int reading = digitalRead(menuButtonPin);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited
    // long enough since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastMenuButtonState)
    {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        // if the button state has changed:
        if (reading != menuButtonState)
        {
            menuButtonState = reading;

            // only toggle the LED if the new button state is HIGH
            if (menuButtonState == LOW)
            {
                profileIndex++;
                profileIndex = profileIndex % (sizeof(profiles) / sizeof(profiles[0]));

                updateProfileScreen();
            }
        }
    }

    lastMenuButtonState = reading;
}

void updateProfileScreen()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Profile #" + String(profileIndex));
    lcd.setCursor(0, 1);
    lcd.print("PW" + String(profiles[profileIndex][0]) + " P" + String(profiles[profileIndex][1]) + " W" + String(profiles[profileIndex][2]));
}

void activateSsr()
{
    int reading = digitalRead(weldButtonPin);

    if (reading != lastSsrState)
    {
        lastSsrDebounceTime = millis();
    }

    if ((millis() - lastSsrDebounceTime) > debounceDelay)
    {
        if (reading != ssrButtonState)
        {
            ssrButtonState = reading;
            if (ssrButtonState == LOW)
            {
                lcd.setRGB(255, 0, 0);
                digitalWrite(ssrPin, HIGH);
                delay(profiles[profileIndex][0]);
                digitalWrite(ssrPin, LOW);
                delay(profiles[profileIndex][1]);
                digitalWrite(ssrPin, HIGH);
                delay(profiles[profileIndex][2]);
                digitalWrite(ssrPin, LOW);
                lcd.setRGB(colorR, colorG, colorB);
            }
        }        
    }
    lastSsrState = reading;
}