/*
  Versatile_RotaryEncoder.cpp - Robust_EEPROM library
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

BoxEncoder::BoxEncoder(int clk, int dt, int sw) {
    
    pin_clk = clk;
    pin_dt = dt;
    pin_sw = sw;

    // Set encoder pins as inputs
    pinMode(pin_clk, INPUT);
    pinMode(pin_dt, INPUT);
    pinMode(pin_sw, INPUT_PULLUP);
}

void BoxEncoder::ReadEncoder() {
    
    if (millis() - last_encoder_read > 1) {
        last_encoder_read = millis();
        encoderBits = digitalRead(pin_sw) << 2 | digitalRead(pin_clk) << 1 | digitalRead(pin_dt);

        // ROTARY READ
        if ((rotaryBits & 0b11) != (encoderBits & 0b11)) {

            rotaryBits = rotaryBits << 2 | encoderBits & 0b11;

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
            // 1.    2.    3.    4.  (long press/release)
            // 000 | 001 | 011 | 111 for OFF
            // 111 | 110 | 100 | 000 for ON
            if ((buttonBits & 0b001) == 0b000) {
                buttonBits = 0b001;
            } else {
                buttonBits = 0b110;
            }	
            // Sets time since button was physically operated
            lastTouch = millis();
        
        } else if (millis() - lastTouch > (uint32_t)SHORTPRESS) { // Same state
            switch (buttonBits) {
                case 0b110:
                    buttonBits = 0b100;
                    button = switchdown;
                    break;
                case 0b100:
                    if (button == pressed && millis() - lastTouch > (uint32_t)LONGPRESS) {
                        buttonBits = 0b000;
                        button = holddown;
                    } else {
                        button = pressed;
                    }
                    break;
                case 0b000:
                    button = held;
                    break;
                case 0b001:
                    buttonBits = 0b011;
                    if (button == held || button == holddown) {
                        button = holdup;
                    } else {
                        button = switchup;
                    }
                    break;
                case 0b011:
                    buttonBits = 0b111;
                    button = released;
                    break;
            }
        }

        // ENCODER PROCESSING
        if (rotary != stopped) {
            if (button == released) {
                encoder = rotate;
                if (handleRotate != nullptr)
                    handleRotate(rotary);
            } else if (button > switchdown) {
                encoder = pressrotate;
                if (handlePressRotate != nullptr)
                    handlePressRotate(rotary);
            } else if (button > pressed && encoder != pressrotate) {
                encoder = heldrotate;
                if (handleHeldRotate != nullptr)
                    handleHeldRotate(rotary);
            }
        } else if (encoder == inactive) {
            switch (button) {
                case switchup:
                {
                    encoder = press;
                    if (handlePress != nullptr)
                        handlePress();
                }
                    break;
                case holddown:
                {
                    encoder = hold;
                    if (handleLongPress != nullptr)
                        handleLongPress();
                }
                    break;
                default:
                    // do nothing
                    break;
            }
        } else if (encoder == hold) {
            switch (button) {
                case holdup:
                {
                    encoder = release;
                    if (handleLongPressRelease != nullptr)
                        handleLongPressRelease();
                }
                    break;
                default:
                    // do nothing
                    break;
            }
        } else if (encoder == pressrotate || encoder == heldrotate) {
            switch (button) {
                case switchup:
                {
                    encoder = press;
                    if (handlePressRotateRelease != nullptr)
                        handlePressRotateRelease();
                }
                    break;
                case holdup:
                {
                    encoder = press;
                    if (handleHeldRotateRelease != nullptr)
                        handleHeldRotateRelease();
                }
                    break;
                default:
                    // do nothing
                    break;
            }
        } else if (rotary == stopped && button == released)
            encoder = inactive;
    }
}

BoxEncoder::Rotary BoxEncoder::getRotary () {
    return rotary;
}

BoxEncoder::Button BoxEncoder::getButton () {
    return button;
}

BoxEncoder::Encoder BoxEncoder::getEncoder () {
    return encoder;
}

uint8_t BoxEncoder::getEncoderBits () {
    return encoderBits >> 2;
}

uint8_t BoxEncoder::getButtonBits () {
    return buttonBits;
}

// Setting Function Handlers

void BoxEncoder::setHandleRotate(functionHandleRotary function_handler) {
    handleRotate = function_handler;
}

void BoxEncoder::setHandlePressRotate(functionHandleRotary function_handler) {
    handlePressRotate = function_handler;
}

void BoxEncoder::setHandleHeldRotate(functionHandleRotary function_handler) {
    handleHeldRotate = function_handler;
}

void BoxEncoder::setHandlePress(functionHandleButton function_handler) {
    handlePress = function_handler;
}

void BoxEncoder::setHandleLongPress(functionHandleButton function_handler) {
    handleLongPress = function_handler;
}

void BoxEncoder::setHandleLongPressRelease(functionHandleButton function_handler) {
    handleLongPressRelease = function_handler;
}

void BoxEncoder::setHandlePressRotateRelease(functionHandleButton function_handler) {
    handlePressRotateRelease = function_handler;
}

void BoxEncoder::setHandleHeldRotateRelease(functionHandleButton function_handler) {
    handleHeldRotateRelease = function_handler;
}