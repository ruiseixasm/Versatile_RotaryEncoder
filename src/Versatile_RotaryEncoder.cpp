/*
  Versatile_RotaryEncoder.cpp - Versatile_RotaryEncoder library
  Original Copyright (c) 2022 Rui Seixas Monteiro. All right reserved.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.
*/
#include "Versatile_RotaryEncoder.h"

Versatile_RotaryEncoder::Versatile_RotaryEncoder(uint8_t clk, uint8_t dt, uint8_t sw) {
    
    pin_clk = clk;
    pin_dt = dt;
    pin_sw = sw;

    // Set encoder pins as inputs
    pinMode(pin_clk, INPUT_TYPE);
    pinMode(pin_dt, INPUT_TYPE);
    pinMode(pin_sw, INPUT_TYPE);
}

bool Versatile_RotaryEncoder::ReadEncoder() {

    bool handled_functions = false;
    
    if (millis() - last_encoder_read >= (uint32_t)read_interval_duration) {
        last_encoder_read = millis();
        encoderBits = (inverted_switch^digitalRead(pin_sw)) << 2 | digitalRead(pin_clk) << 1 | digitalRead(pin_dt);

        // ROTARY READ
        if ((rotaryBits & 0b11) != (encoderBits & 0b11)) {

            rotaryBits = rotaryBits << 2 | (encoderBits & 0b11);

            // Bit Pairs Cyclic Sequence (CLK DT Pair Bits):
            // 1.   2.   3.   4.   5.
            // 11 | 01 | 00 | 10 | 11 for CCW
            // 11 | 10 | 00 | 01 | 11 for CW
            if (rotaryBits == 0b01001011) {
                rotary = left;
            } else if (rotaryBits == 0b10000111) {
                rotary = right;   
            } else {
                rotary = stopped;
            }

        } else {
            rotary = stopped;
        }

        // BUTTON READ
        if ((buttonBits & 0b001) != (encoderBits >> 2 & 0b001)) { // Change state (any action)

            // Bit Pairs Press Release Cyclic Sequence (OFF ON Pair Bits):
            // 1.    2.    3.    4.  (long press/release) (cascate) 1. stand by, 2. triggers cascate
            // 000 | 001 | 011 | 111 for OFF
            // 111 | 110 | 100 | 000 for ON
            if (millis() - last_switch > (uint32_t)short_press_duration) { // triggers cascate
                if ((buttonBits & 0b001) == 0b000)
                    buttonBits = 0b001;
                else
                    buttonBits = 0b110;
                last_switch = millis();
            }
        } else {
            // Sets last time since button was physically operated
            last_switch = millis();
        }
        
        // CASCATE
        switch (buttonBits) {
            case 0b111: break; // OFF cascate dead end
            case 0b000: // ON cascate dead end
                button = held;
                break;
            case 0b100:
                if ((button == pressed || button == double_switchdown) && millis() - last_switchdown > (uint32_t)long_press_duration) {
                    buttonBits = 0b000;
                    button = holddown;
                } else if (button == switchdown) {
                    button = pressed;
                }
                break;
            case 0b011:
                buttonBits = 0b111;
                button = released;
                break;
            case 0b001:
                buttonBits = 0b011;
                if (button == held || button == holddown)
                    button = holdup;
                else
                    button = switchup;
                break;
            case 0b110:
                buttonBits = 0b100;
                button = switchdown;
                if (handleDoublePress != nullptr && millis() - last_switchdown < (uint32_t)double_press_duration) {
                    button = double_switchdown;
                }
                last_switchdown = millis();
                break;
        }

        // ENCODER PROCESSING
        if (rotary == stopped && button == released)
            encoder = inactive;

        if (rotary != stopped) {
            if (button == released) {
                encoder = rotate;
                if (handleRotate != nullptr) {
                    handleRotate(rotary);
                    handled_functions = true;
                }
            } else if ((button > switchdown && button < holddown) || encoder == pressrotate) {
                encoder = pressrotate;
                if (handlePressRotate != nullptr) {
                    handlePressRotate(rotary);
                    handled_functions = true;
                }
            } else if (button > pressed) {
                encoder = heldrotate;
                if (handleHeldRotate != nullptr) {
                    handleHeldRotate(rotary);
                    handled_functions = true;
                }
            }
        }

        if (encoder == inactive) {
            switch (button) {
                case switchdown:
                    encoder = press;
                    if (handlePress != nullptr) {
                        handlePress();
                        handled_functions = true;
                    }
                    break;
                case double_switchdown:
                    encoder = double_press;
                    handleDoublePress();
                    handled_functions = true;
                    break;
                default:
                    // do nothing
                    break;
            }
        } else if (encoder == press || encoder == double_press) {
            switch (button) {
                case switchup:
                    encoder = release;
                    if (handlePressRelease != nullptr) {
                        handlePressRelease();
                        handled_functions = true;
                    }
                    break;
                case holddown:
                    encoder = hold;
                    if (handleLongPress != nullptr) {
                        handleLongPress();
                        handled_functions = true;
                    }
                    break;
                default:
                    // do nothing
                    break;
            }
        } else if (encoder == hold) {
            switch (button) {
                case holdup:
                    encoder = release;
                    if (handleLongPressRelease != nullptr) {
                        handleLongPressRelease();
                        handled_functions = true;
                    }
                    break;
                default:
                    // do nothing
                    break;
            }
        } else if (encoder == pressrotate) {
            switch (button) {
                case switchup:
                case holdup:
                    encoder = release;
                    if (handlePressRotateRelease != nullptr) {
                        handlePressRotateRelease();
                        handled_functions = true;
                    }
                    break;
                default:
                    // do nothing
                    break;
            }
        } else if (encoder == heldrotate) {
            switch (button) {
                case holdup:
                    encoder = release;
                    if (handleHeldRotateRelease != nullptr) {
                        handleHeldRotateRelease();
                        handled_functions = true;
                    }
                    break;
                default:
                    // do nothing
                    break;
            }
        }
    }

    return handled_functions;
}

void Versatile_RotaryEncoder::setInvertedSwitch (bool invert_switch) {
    inverted_switch = 0b1 & invert_switch;
}

void Versatile_RotaryEncoder::setReadIntervalDuration (uint8_t duration) {
    read_interval_duration = duration;
}

void Versatile_RotaryEncoder::setShortPressDuration (uint8_t duration) {
    short_press_duration = duration;
}

void Versatile_RotaryEncoder::setLongPressDuration (uint16_t duration) {
    long_press_duration = duration;
}

void Versatile_RotaryEncoder::setDoublePressDuration (uint16_t duration) {
    double_press_duration = duration;
}

Versatile_RotaryEncoder::Rotary Versatile_RotaryEncoder::getRotary () {
    return rotary;
}

Versatile_RotaryEncoder::Button Versatile_RotaryEncoder::getButton () {
    return button;
}

Versatile_RotaryEncoder::Encoder Versatile_RotaryEncoder::getEncoder () {
    return encoder;
}

uint8_t Versatile_RotaryEncoder::getEncoderBits () {
    return encoderBits >> 2;
}

uint8_t Versatile_RotaryEncoder::getButtonBits () {
    return buttonBits;
}

// Setting Function Handlers

void Versatile_RotaryEncoder::setHandleRotate(functionHandleRotary function_handler) {
    handleRotate = function_handler;
}

void Versatile_RotaryEncoder::setHandlePressRotate(functionHandleRotary function_handler) {
    handlePressRotate = function_handler;
}

void Versatile_RotaryEncoder::setHandleHeldRotate(functionHandleRotary function_handler) {
    handleHeldRotate = function_handler;
}

void Versatile_RotaryEncoder::setHandlePress(functionHandleButton function_handler) {
    handlePress = function_handler;
}

void Versatile_RotaryEncoder::setHandleDoublePress(functionHandleButton function_handler) {
    handleDoublePress = function_handler;
}

void Versatile_RotaryEncoder::setHandlePressRelease(functionHandleButton function_handler) {
    handlePressRelease = function_handler;
}

void Versatile_RotaryEncoder::setHandleLongPress(functionHandleButton function_handler) {
    handleLongPress = function_handler;
}

void Versatile_RotaryEncoder::setHandleLongPressRelease(functionHandleButton function_handler) {
    handleLongPressRelease = function_handler;
}

void Versatile_RotaryEncoder::setHandlePressRotateRelease(functionHandleButton function_handler) {
    handlePressRotateRelease = function_handler;
}

void Versatile_RotaryEncoder::setHandleHeldRotateRelease(functionHandleButton function_handler) {
    handleHeldRotateRelease = function_handler;
}