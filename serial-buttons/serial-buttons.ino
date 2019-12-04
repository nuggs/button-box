
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
    button_one.update(button_flags[BUTTON_MID_ONE]);
    button_two.update(button_flags[BUTTON_MID_TWO]);
    button_three.update(button_flags[BUTTON_MID_THREE]);
    button_four.update(button_flags[BUTTON_MID_FOUR]);
    button_five.update(button_flags[BUTTON_MID_FIVE]);
    button_six.update(button_flags[BUTTON_MID_SIX]);
    button_seven.update(button_flags[BUTTON_MID_SEVEN]);
    button_eight.update(button_flags[BUTTON_MID_EIGHT]);
    button_nine.update(button_flags[BUTTON_MID_NINE]);
    button_ten.update(button_flags[BUTTON_MID_TEN]);
}
