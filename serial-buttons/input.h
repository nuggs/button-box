/*
 * Turncoats Button Box, I don't have a name for this...
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

/*
 * Middle row of buttons
 */
#define BUTTON_MID_ONE          1
#define BUTTON_MID_TWO          2
#define BUTTON_MID_THREE        3
#define BUTTON_MID_FOUR         4
#define BUTTON_MID_FIVE         5
#define BUTTON_MID_SIX          6
#define BUTTON_MID_SEVEN        7
#define BUTTON_MID_EIGHT        8
#define BUTTON_MID_NINE         9
#define BUTTON_MID_TEN          10

/*
 * top set of momentary switches, two inputs each
 */
#define SWITCH_ONE_ONE          11
#define SWITCH_ONE_TWO          12
#define SWITCH_TWO_ONE          13
#define SWITCH_TWO_TWO          14
#define SWITCH_THREE_ONE        15
#define SWITCH_THREE_TWO        16
#define SWITCH_FOUR_ONE         17
#define SWITCH_FOUR_TWO         18
#define SWITCH_FIVE_ONE         19
#define SWITCH_FIVE_TWO         20
#define SWITCH_SIX_ONE          21
#define SWITCH_SIX_TWO          22

/*
 * three toggle switches
 */
#define TOGGLE_ONE              23
#define TOGGLE_TWO              24
#define TOGGLE_THREE            25

#define MAX_BUTTON_INPUTS       26

#define PRESSED                 1
#define RELEASED                0

/*
 * This is for sending bytes to the USB MCU or whatever
 * I don't remember this lingo anymore...  I forgot
 * what I was doing it was so long ago I was doing this.
 * 
 * Also, this isn't in use at the moment...  Not even sure
 * what I was doing when I was passing arguments to update.
 */
const char button_flags[MAX_BUTTON_INPUTS] = {
    0, /* We don't care about this one */

    /* two middle rows */
    (0<<0), (1<<0), (1<<1), (1<<2), (1<<3),
    (1<<4), (1<<5), (1<<6), (1<<7), (1<<8),

    /* top row of switches */
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,

    /* toggle switches */
    0, 0, 0
};

const uint8_t INPUT_NONE    = 0;
const uint8_t INPUT_AXIS    = 1;
const uint8_t INPUT_BUTTON  = 2;
const uint8_t INPUT_TOGGLE  = 3;
const uint8_t INPUT_ENCODER = 4;

class Input {
    private:
        bool    _state;
        bool    _sent;
        uint8_t _pin;
        uint8_t _type;
        uint8_t _mode;
        uint8_t _button;

        int16_t _min_range  = 0;
        int16_t _max_range  = 0;

        Bounce Debounce;
        input_data _handler;

    public:
        Input(uint8_t pin, uint8_t type, uint8_t button, char mode): _pin(pin), _button(button), _type(type), _mode(mode) {}

        void begin(void) {
            pinMode(_pin, _mode);

            switch (_type) {
                case INPUT_BUTTON:
                    Debounce = Bounce();
                    Debounce.attach(_pin);
                    Debounce.interval(5);
                    _state = Debounce.read();
                break;
                case INPUT_AXIS:
                    /*
                     * Handle axis...  We don't have those for this...
                     */
                break;
                case INPUT_TOGGLE:
                    /*
                     * This is for toggle switches.  I imagine
                     * some games will one a press for on and
                     * then another for off.  So we'll either
                     * send and keep it pressed until released
                     * or send a press when we toggle it on and then
                     * send it again when we toggle it off.
                     */
                    
                break;
                case INPUT_ENCODER:
                    /* handle encoders here */
                break;
                default:
                    Serial.print("Some shit went wrong, call the cops\n");
                break;
            }
        }

        void update() {
            Debounce.update();
            int state = Debounce.read();

            switch(_type) {
                case INPUT_BUTTON:
                    if (state != _state) {
                        // set state change to release button
                        _state = state;
                        if (_sent == true) {
                            _handler.send_input(_type, _button, 0, RELEASED);
                            _sent = false;
                        }
                    }
    
                    if (_state == LOW) {
                        if (_button == 0) {
                            return;
                        }

                        if (_sent == false) {
                            // shit, button press, we need to communicate with the USB MCU
                            _handler.send_input(_type, _button, 0, PRESSED);
                            _sent = true;
                        }
                    }
                break;
                default: break;
            }
        }

        void receive_input(char input) {
            _handler.receive_input(input);
        }
};
