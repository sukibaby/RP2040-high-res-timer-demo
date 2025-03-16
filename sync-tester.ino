/*
 * Sync tester for Raspberry Pi Pico / RP2040
 * Uses a simple loop to trigger a keypress every 250ms, which is equivalent to 4Hz (240bpm in 4/4).
 * Best results achieved when compiled at 125MHz, optimizations disabled
 */

// TODO: determine if Keyboard.h or USB protocol is responsible for very slight drift over time; verify 1000hz operation
#include <Keyboard.h>
#include "pico/stdlib.h"

/* button on GP18 */
#define BUTTON_PIN 18
/* 250 ms */
#define INTERVAL_US 250000

bool isSending = false; // toggled by button presses
uint32_t lastTime = 0;
int lastButtonState = HIGH;

void setup()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    Keyboard.begin();
    lastTime = time_us_32();
}

void loop()
{
    // Check button state
    int buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == LOW && lastButtonState == HIGH)
    {
        isSending = !isSending;
        if (isSending)
        {
            lastTime = time_us_32();
        }
    }
    lastButtonState = buttonState;

    // Send the keystroke if isSending is true and the interval has passed
    uint32_t currentTime = time_us_32();
    if (isSending && (currentTime - lastTime >= INTERVAL_US))
    {
        Keyboard.write('S');
        lastTime = currentTime;
    }
}
