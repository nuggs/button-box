/*
 * FastLED stuff for later.

#include <FastLED.h>
#define NUM_LEDS 60
#define DATA_PIN 2
CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

...setup()...

    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
<-

...loop()...
    ChangePalettePeriodically();
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; * motion speed *
    
    FillLEDsFromPaletteColors( startIndex);
    FastLED.show(); 
    delay(30); 
<-


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
 */

#include <Bounce2.h>
#include "mcu-io.h"
#include "input.h"

/*
 * These are the ten buttons in the middle.
 * they top left is button one, bottom right
 * is button ten.  I don't know why, but
 * that's what I'm doing.
 */
Input button_one(22, INPUT_BUTTON, BUTTON_MID_ONE, INPUT_PULLUP);
Input button_two(23, INPUT_BUTTON, BUTTON_MID_TWO, INPUT_PULLUP);
Input button_three(24, INPUT_BUTTON, BUTTON_MID_THREE, INPUT_PULLUP);
Input button_four(25, INPUT_BUTTON, BUTTON_MID_FOUR, INPUT_PULLUP);
Input button_five(26, INPUT_BUTTON, BUTTON_MID_FIVE, INPUT_PULLUP);
Input button_six(27, INPUT_BUTTON, BUTTON_MID_SIX, INPUT_PULLUP);
Input button_seven(28, INPUT_BUTTON, BUTTON_MID_SEVEN, INPUT_PULLUP);
Input button_eight(29, INPUT_BUTTON, BUTTON_MID_EIGHT, INPUT_PULLUP);
Input button_nine(30, INPUT_BUTTON, BUTTON_MID_NINE, INPUT_PULLUP);
Input button_ten(31, INPUT_BUTTON, BUTTON_MID_TEN, INPUT_PULLUP);

void setup() {
    Serial.begin(115200);

    /* holy shit this is a pain in the ass. */
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
}

void loop() {
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
}
