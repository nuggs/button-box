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

const byte BUFFER_SIZE                  = 32;

class input_data {
    private:
        char input_buffer[BUFFER_SIZE];
        byte bytes_received             = 0;
        bool is_reading                 = false;
        int8_t _button                  = 0;
        int16_t _value                  = 0;
        int8_t _type                    = 0;

    public:
        const byte START_MARKER         = '<';
        const byte END_MARKER           = '>';

        void clear_input(void) {
            input_buffer[BUFFER_SIZE]   = '\0';
            bytes_received              = 0;
            _button                     = 0;
            _value                      = 0;
            _type                       = 0;
            is_reading                  = false;
        }

        int16_t get_value() { return _value; }
        void set_value(int16_t value) { _value = value; }

        int16_t get_button() { return _button; }
        void set_button(int8_t button) { _button = button; }

        int8_t get_type() { return _type; }
        void set_type(int8_t type) { _type = type; }

        void send_input(int8_t type, int8_t button, int16_t value, uint8_t pressed) {
            /*if (type == 0 || button == 0) {
                return;
            }*/

            /*if ((button >= 1 || button <= 25) && value < 1) {
                return;
            }*/

            Serial.print("<");
            Serial.print(type);
            Serial.print(",");
            Serial.print(button);
            Serial.print(",");
            Serial.print(value);
            Serial.print(",");
            Serial.print(pressed);
            Serial.println(">");
            //clear_input();
            //Serial.flush();
            delay(5);
        }

        void receive_input(char input) {
            if (input == END_MARKER) {
                is_reading = false;
                parse_data();
                input_buffer[bytes_received] = 0;
            }

            if (is_reading) {
                input_buffer[bytes_received] = input;
                bytes_received++;
                if (bytes_received == BUFFER_SIZE) {
                    bytes_received = BUFFER_SIZE - 1;
                }
            }

            if (input == START_MARKER) {
                bytes_received = 0;
                is_reading = true;
            }
        }

        void parse_data(void) {
            char *input_stream;

            input_stream = strtok(input_buffer, ",");
            _type = atoi(input_stream);

            input_stream = strtok(NULL, ",");
            _button = atoi(input_stream);

            input_stream = strtok(NULL, ",");
            _value = atoi(input_stream);
        }
};
