/*
 * yeah dude.  shit and stuff.
 * yeet.
 */

#include <FastLED.h>
#include <Bounce2.h>
#include <ClickEncoder.h>
#include "mcu-io.h"
#include "input.h"

#define NUM_LEDS 30
#define DATA_PIN 13
CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

/*
 * Toggle switches starting from the left.
 */
Input toggle_one(22, 0, 0, INPUT_TOGGLE, TOGGLE_ONE, 0, INPUT_PULLUP);
Input toggle_two(23, 0, 0, INPUT_TOGGLE, TOGGLE_TWO, 0, INPUT_PULLUP);
Input toggle_three(24, 0, 0, INPUT_TOGGLE, TOGGLE_THREE, 0, INPUT_PULLUP);

Input switch_one_one(25, 0, 0, INPUT_BUTTON, SWITCH_ONE_ONE, 0, INPUT_PULLUP);
Input switch_one_two(26, 0, 0, INPUT_BUTTON, SWITCH_ONE_TWO, 0, INPUT_PULLUP);
Input switch_two_one(27, 0, 0, INPUT_BUTTON, SWITCH_TWO_ONE, 0, INPUT_PULLUP);
Input switch_two_two(28, 0, 0, INPUT_BUTTON, SWITCH_TWO_TWO, 0, INPUT_PULLUP);
Input switch_three_one(29, 0, 0, INPUT_BUTTON, SWITCH_THREE_ONE, 0, INPUT_PULLUP);
Input switch_three_two(30, 0, 0, INPUT_BUTTON, SWITCH_THREE_TWO, 0, INPUT_PULLUP);
Input switch_four_one(31, 0, 0, INPUT_BUTTON, SWITCH_FOUR_ONE, 0, INPUT_PULLUP);
Input switch_four_two(32, 0, 0, INPUT_BUTTON, SWITCH_FOUR_TWO, 0, INPUT_PULLUP);
Input switch_five_one(33, 0, 0, INPUT_BUTTON, SWITCH_FIVE_ONE, 0, INPUT_PULLUP);
Input switch_five_two(34, 0, 0, INPUT_BUTTON, SWITCH_FIVE_TWO, 0, INPUT_PULLUP);
Input switch_six_one(35, 0, 0, INPUT_BUTTON, SWITCH_SIX_ONE, 0, INPUT_PULLUP);
Input switch_six_two(36, 0, 0, INPUT_BUTTON, SWITCH_SIX_TWO, 0, INPUT_PULLUP);

Input button_one(37, 0, 0, INPUT_BUTTON, BUTTON_MID_ONE, 0, INPUT_PULLUP);
Input button_two(38, 0, 0, INPUT_BUTTON, BUTTON_MID_TWO, 0, INPUT_PULLUP);
Input button_three(39, 0, 0, INPUT_BUTTON, BUTTON_MID_THREE, 0, INPUT_PULLUP);
Input button_four(40, 0, 0, INPUT_BUTTON, BUTTON_MID_FOUR, 0, INPUT_PULLUP);
Input button_five(41, 0, 0, INPUT_BUTTON, BUTTON_MID_FIVE, 0, INPUT_PULLUP);
Input button_six(42, 0, 0, INPUT_BUTTON, BUTTON_MID_SIX, 0, INPUT_PULLUP);
Input button_seven(43, 0, 0, INPUT_BUTTON, BUTTON_MID_SEVEN, 0, INPUT_PULLUP);
Input button_eight(44, 0, 0, INPUT_BUTTON, BUTTON_MID_EIGHT, 0, INPUT_PULLUP);
Input button_nine(45, 0, 0, INPUT_BUTTON, BUTTON_MID_NINE, 0, INPUT_PULLUP);
Input button_ten(46, 0, 0, INPUT_BUTTON, BUTTON_MID_TEN, 0, INPUT_PULLUP);

Input encoder_one(A6, A5, 49, INPUT_ENCODER, ENCODER_ONE, ENCODER_ONE_HIGH, INPUT);
Input encoder_two(A8, A9, 10, INPUT_ENCODER, ENCODER_TWO, ENCODER_TWO_HIGH, INPUT);
Input encoder_three(A11, A10, 11, INPUT_ENCODER, ENCODER_THREE, ENCODER_THREE_HIGH, INPUT);
Input encoder_four(A13, A12, A7, INPUT_ENCODER, ENCODER_FOUR, ENCODER_FOUR_HIGH, INPUT);

unsigned long prev_millis;
const uint8_t led_wait = 30;

void setup() {
    Serial.begin(2000000);

    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

    /* holy shit this is a pain in the ass. */
    toggle_one.begin();
    toggle_two.begin();
    toggle_three.begin();

    switch_one_one.begin();
    switch_one_two.begin();
    switch_two_one.begin();
    switch_two_two.begin();
    switch_three_one.begin();
    switch_three_two.begin();
    switch_four_one.begin();
    switch_four_two.begin();
    switch_five_one.begin();
    switch_five_two.begin();
    switch_six_one.begin();
    switch_six_two.begin();

    button_one.begin();
    button_two.begin();
    button_three.begin();
    button_four.begin();
    button_five.begin();
    button_six.begin();
    button_seven.begin();
    button_eight.begin();
    button_nine.begin();
    button_ten.begin();

    encoder_one.begin();
    encoder_two.begin();
    encoder_three.begin();
    encoder_four.begin();
}

void loop() {
    toggle_one.update();
    toggle_two.update();
    toggle_three.update();

    switch_one_one.update();
    switch_one_two.update();
    switch_two_one.update();
    switch_two_two.update();
    switch_three_one.update();
    switch_three_two.update();
    switch_four_one.update();
    switch_four_two.update();
    switch_five_one.update();
    switch_five_two.update();
    switch_six_one.update();
    switch_six_two.update();

    button_one.update();
    button_two.update();
    button_three.update();
    button_four.update();
    button_five.update();
    button_six.update();
    button_seven.update();
    button_eight.update();
    button_nine.update();
    button_ten.update();

    encoder_one.update();
    encoder_two.update();
    encoder_three.update();
    encoder_four.update();

    /* Update FastLED stuff. */
    unsigned long current_millis = millis();
    if (current_millis >= prev_millis + led_wait) {
        prev_millis = millis();
        ChangePalettePeriodically();
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* motion speed */

        FillLEDsFromPaletteColors( startIndex);
        FastLED.show();
    }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}
