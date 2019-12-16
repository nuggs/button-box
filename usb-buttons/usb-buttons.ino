
#include <HID-Project.h>
#include <HID-Settings.h>
#include "mcu-io.h"

input_data      handler;

void setup() {
  /* This is where we get our data from the other MCU. */
  Serial1.begin(2000000);
  Gamepad.begin();
}

void loop() {
    handler.receive_input();
    handler.buttons[handler.get_button()].passes++;

    if (handler.buttons[handler.get_button()].get_pressed() == false) {
        Gamepad.release(handler.get_button());
    }

    if (handler.get_type() == INPUT_ENCODER && handler.buttons[handler.get_button()].get_pressed() == false) {
        if (handler.buttons[handler.get_button()].passes++ >= 10) {
            Gamepad.dPad1(0);
        }
    }

    switch (handler.get_type()) {
        case INPUT_BUTTON:
            if (handler.buttons[handler.get_button()].get_pressed() == true) {
                Gamepad.press(handler.get_button());
            }
        break;
        case INPUT_AXIS:
            //Gamepad.xAxis(handler.get_value());
        break;
        case INPUT_TOGGLE:
            if (handler.buttons[handler.get_button()].get_pressed() == true) {
                Gamepad.press(handler.get_button());
            }
        break;
        case INPUT_ENCODER:
            if (handler.buttons[handler.get_button()].get_pressed() == true) {
                if (handler.get_button() >= 1 || handler.get_button() <= 4) {
        
                    Gamepad.dPad1(handler.get_value());
                }
            }
        break;
        case INPUT_NONE:
            break;
        default:
            if (handler.get_button() == 0) {
                Gamepad.releaseAll();
            }
        break;
    }
    Gamepad.write();
}
