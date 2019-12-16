/*
 * nHandbrake
 * Copyright (C) 2018 - 2019  Anthony Goins
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

const uint8_t INPUT_NONE    = 0;
const uint8_t INPUT_AXIS    = 1;
const uint8_t INPUT_BUTTON  = 2;
const uint8_t INPUT_TOGGLE  = 3;
const uint8_t INPUT_ENCODER = 4;

/*
 * class for buttons, we'll use an array of these to store some data like is the shit
 * pressed and whatnot...  So we know when to hold and when to release a button.
 */
class button_list {
    public:
        void set_pressed(bool is_it) {
            _pressed = is_it;
        }

        bool get_pressed(void) { return _pressed; }

        bool _pressed;
        uint8_t passes = 0;
};

class input_data {
    private:
        char input_buffer[32];
        byte bytes_received             = 0;
        bool is_reading                 = false;
        int8_t _button                  = 0;
        int16_t _value                  = 0;
        int8_t _type                    = 0;

    public:
        button_list buttons[38];
        const char START_MARKER         = '<';
        const char END_MARKER           = '>';

        void clear_input(void) {
            _button                     = 0;
            _value                      = 0;
            _type                       = INPUT_NONE;
        }

        int16_t get_value() { return _value; }
        void set_value(int16_t value) { _value = value; }

        int16_t get_button() { return _button; }
        void set_button(int8_t button) { _button = button; }

        int8_t get_type() { return _type; }
        void set_type(int8_t type) { _type = type; }

        void send_input(int8_t type, int8_t button, int16_t value) {
            Serial.print("<");
            Serial.print(type);
            Serial.print(",");
            Serial.print(button);
            if (value > 0) {
                Serial.print(",");
                Serial.print(value);
            }
            Serial.println(">");
        }

        void receive_input(void) {
            char rc;

            while (Serial1.available() > 0) {
                rc = Serial1.read();

                /* Ignore anything that isn't what we want */
                if (is_reading == false && rc != START_MARKER) {
                    break;
                }

                if (is_reading == true) {
                    if (rc != END_MARKER) {
                        input_buffer[bytes_received] = rc;
                        bytes_received++;
                        if (bytes_received >= 32) {
                            bytes_received = 32 - 1;
                        }
                    } else {
                        parse_data();
                        input_buffer[bytes_received] = '\0'; // terminate the string
                        is_reading = false;
                        bytes_received = 0;
                    }
                } else if (rc == START_MARKER) {
                    is_reading = true;
                }
            }
        }

        void parse_data(void) {
            char *input_stream;
            int axis_value = 0;
            int prev_value = _value;
            uint8_t pressed = 0;

            input_stream = strtok(input_buffer, ",");
            _type = atoi(input_stream);

            input_stream = strtok(NULL, ",");
            _button = atoi(input_stream);

            input_stream = strtok(NULL, ",");
            axis_value = atoi(input_stream);
            if (axis_value <= 1023 && axis_value >= 0) {
                _value = axis_value;
            }

            input_stream = strtok(NULL, ",");
            pressed = atoi(input_stream);

            if (pressed != 0) {// && (_button != 0 && _value != 0)) {
                buttons[_button].set_pressed(true);
            } else {
                buttons[_button].set_pressed(false);
            }
        }
};
