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
 * three toggle switches
 */
#define TOGGLE_ONE              1
#define TOGGLE_TWO              2
#define TOGGLE_THREE            3

/*
 * top set of momentary switches, two inputs each
 */
#define SWITCH_ONE_ONE          4
#define SWITCH_ONE_TWO          5
#define SWITCH_TWO_ONE          6
#define SWITCH_TWO_TWO          7
#define SWITCH_THREE_ONE        8
#define SWITCH_THREE_TWO        9
#define SWITCH_FOUR_ONE         10
#define SWITCH_FOUR_TWO         11
#define SWITCH_FIVE_ONE         12
#define SWITCH_FIVE_TWO         13
#define SWITCH_SIX_ONE          14
#define SWITCH_SIX_TWO          15

/*
 * Middle row of buttons
 */
#define BUTTON_MID_ONE          16
#define BUTTON_MID_TWO          17
#define BUTTON_MID_THREE        18
#define BUTTON_MID_FOUR         19
#define BUTTON_MID_FIVE         20
#define BUTTON_MID_SIX          21
#define BUTTON_MID_SEVEN        22
#define BUTTON_MID_EIGHT        23
#define BUTTON_MID_NINE         24
#define BUTTON_MID_TEN          25

#define ENCODER_ONE_PUSH        26 // This is on pin 49 I think
#define ENCODER_TWO_PUSH        27
#define ENCODER_THREE_PUSH      28
#define ENCODER_FOUR_PUSH       29

#define MAX_BUTTON_INPUTS       32

/*
 * These aren't used yet.
 * In the future, it will either be a button press
 * or send the encoding info as a slider or something.
 */
#define ENCODER_ONE             1
#define ENCODER_TWO             2
#define ENCODER_THREE           3
#define ENCODER_FOUR            4

#define ENCODER_ONE_HIGH        1
#define ENCODER_ONE_LOW         2
#define ENCODER_TWO_HIGH        3
#define ENCODER_TWO_LOW         4
#define ENCODER_THREE_HIGH      5
#define ENCODER_THREE_LOW       6
#define ENCODER_FOUR_HIGH       7
#define ENCODER_FOUR_LOW        8

#define TOGGLE_MODE_RELEASE     1
#define TOGGLE_MODE_HOLD        2

const uint8_t INPUT_NONE        = 0;
const uint8_t INPUT_AXIS        = 1;
const uint8_t INPUT_BUTTON      = 2;
const uint8_t INPUT_TOGGLE      = 3;
const uint8_t INPUT_ENCODER     = 4;

class Input {
    private:
        bool    _state          = false;
        bool    _sent           = false;
        bool    _encoder_sent   = false;

        uint8_t _pin;
        uint8_t _pinb;
        uint8_t _pinc;
        uint8_t _type;
        uint8_t _mode;
        uint8_t _button;
        uint8_t _button_two;
        int16_t _last_encoder_value;
        int16_t _encoder_value;
        uint8_t _encoder_passes;

        /* This is for the toggle switches */
        bool _toggle_active     = false;
        uint8_t _toggle_mode    = TOGGLE_MODE_RELEASE;

        int16_t _min_range  = 0;
        int16_t _max_range  = 0;

        Bounce Debounce;
        ClickEncoder *encoder;
        ClickEncoder::Button encoder_button;
        input_data _handler;

    public:
        Input(uint8_t pina, uint8_t pinb, uint8_t pinc, uint8_t type, uint8_t button, uint8_t button_two, char mode): _pin(pina), _pinb(pinb), _pinc(pinc), _button(button), _button_two(button_two), _type(type), _mode(mode) {}

        void begin(void) {
            switch (_type) {
                case INPUT_BUTTON:
                    pinMode(_pin, _mode);
                    Debounce = Bounce();
                    Debounce.attach(_pin);
                    Debounce.interval(15);      /* Change to 5 again...  maybe... */
                    _state = Debounce.read();
                break;
                case INPUT_AXIS:
                    /*
                     * Handle axis...  We don't have those for this...  yet...
                     */
                break;
                case INPUT_TOGGLE:
                    pinMode(_pin, _mode);
                    Debounce = Bounce();
                    Debounce.attach(_pin);
                    Debounce.interval(15);
                    _state = Debounce.read();
                break;
                case INPUT_ENCODER:
                    encoder = new ClickEncoder(_pin, _pinb, _pinc, 4);
                break;
                default:
                    Serial.print("Some shit went wrong, call the cops\n");
                break;
            }
        }

        void update() {
            uint8_t state = 0;

            switch(_type) {
                case INPUT_BUTTON:
                    Debounce.update();
                    state = Debounce.read();
                    if (state != _state) {
                        // set state change to release button
                        _state = state;
                        if (_sent == true) {
                            _handler.send_input(_type, _button, 0, 0);
                            _sent = false;
                            break;
                        }
                    }
    
                    if (_state == LOW) {
                        if (_button == 0) {
                            break;
                        }

                        if (_sent == false) {
                            _handler.send_input(_type, _button, 0, 1);
                            _sent = true;
                            break;
                        }
                    }
                break;
                case INPUT_TOGGLE:
                    Debounce.update();
                    state = Debounce.read();
                    if (state != _state) {
                        _state = state;
                        if (_sent == true) {
                            _handler.send_input(_type, _button, 0, 0);
                            _sent = false;
                            break;
                        }
                    }
    
                    if (_state == LOW) {
                        if (_button == 0) {
                            break;
                        }

                        if (_sent == false) {
                            _handler.send_input(_type, _button, 0, 1);
                            _sent = true;
                            break;
                        }
                    }
                break;
                case INPUT_ENCODER:
                    encoder->service();
                    _encoder_value += encoder->getValue();

                    if (_encoder_sent == true) {
                        _encoder_passes++;
                        if (_encoder_passes >= 16) {
                            _handler.send_input(_type, _button, _encoder_value < _last_encoder_value ? _button_two : _button_two+1, 0);
                            _last_encoder_value = _encoder_value;
                            _encoder_passes = 0;
                            _encoder_sent = false;
                        }
                    }

                    if (_encoder_value != _last_encoder_value && _encoder_sent == false) {
                        _handler.send_input(_type, _button, _encoder_value <= _last_encoder_value ? _button_two : _button_two+1, 1);
                        _encoder_sent = true;
                    }

                    encoder_button = encoder->getButton();
                    if (encoder_button != ClickEncoder::Open) {
                        const int encoder_buttons[] = {
                            0, ENCODER_ONE_PUSH, ENCODER_TWO_PUSH, ENCODER_THREE_PUSH, ENCODER_FOUR_PUSH
                        };
                        switch (encoder_button) {
                            case ClickEncoder::Pressed:
                                if (_sent == false) {
                                    _handler.send_input(INPUT_BUTTON, encoder_buttons[_button], 0, 1);
                                    _sent = true;
                                }
                            break;
                            case ClickEncoder::DoubleClicked:
                            case ClickEncoder::Clicked:
                            case ClickEncoder::Released:
                                if (_sent == true) {
                                    _handler.send_input(INPUT_BUTTON, encoder_buttons[_button], 0, 0);
                                    _sent = false;
                                }
                            break;
                            default: break;
                        }
                    }
                break;
                default: break;
            }
        }

        void receive_input(char input) {
            _handler.receive_input();
        }
};
