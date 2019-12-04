
#include <HID-Project.h>
#include <HID-Settings.h>
#include "mcu-io.h"

input_data handler;

/*void show_input_data(void) {
    Serial.print("Recieved: ");
    Serial.println(handler.get_type());
    Serial.println(handler.get_button());
    Serial.println(handler.get_value());
}*/

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);
    Gamepad.begin();
}

void loop() {
    handler.receive_input();

    if (handler.buttons[handler.get_button()].get_pressed() == false) {
        Gamepad.release(handler.get_button());
    }

    switch (handler.get_type()) {
        case INPUT_BUTTON:
            //show_input_data();
            if (handler.buttons[handler.get_button()].get_pressed() == true) {
                Gamepad.press(handler.get_button());
            }
        break;
        case INPUT_AXIS:
            //show_input_data();
            //Gamepad.xAxis(handler.get_value());
        break; 
        case INPUT_TOGGLE:
        break;
        case INPUT_ENCODER:
        break;
        case INPUT_NONE:
        break;
        default:
            if (handler.get_button() == 0) {
                Serial.println("lol what");
                Gamepad.releaseAll();
            }
        break;
    }
    Gamepad.write();
}
