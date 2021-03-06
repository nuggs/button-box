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
            Serial.print("<");
            Serial.print(type);
            Serial.print(",");
            Serial.print(button);
            Serial.print(",");
            Serial.print(value);
            Serial.print(",");
            Serial.print(pressed);
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
                        if (bytes_received >= BUFFER_SIZE) {
                            bytes_received = BUFFER_SIZE - 1;
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

            input_stream = strtok(input_buffer, ",");
            _type = atoi(input_stream);

            input_stream = strtok(NULL, ",");
            _button = atoi(input_stream);

            input_stream = strtok(NULL, ",");
            _value = atoi(input_stream);
        }
};
