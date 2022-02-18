/*
  Versatile_RotaryEncoder.h - Robust_EEPROM library
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
#ifndef VERSATILE_ROTARYENCODER_H_INCLUDED
#define VERSATILE_ROTARYENCODER_H_INCLUDED

#include <Arduino.h>

#define SHORTPRESS 100
#define LONGPRESS 1000

class BoxEncoder {
    public:
        enum Encoder : uint8_t {inactive, release, press, hold, rotate, pressrotate, heldrotate};
        enum Button : uint8_t {released, holdup, switchup, switchdown, pressed, holddown, held};
        enum Rotary : int8_t {left = -1, stopped, right}; // Don't change these values, other functions are dependent on them
        using functionHandleRotary = void (*)(int8_t rotation);
        using functionHandleButton = void (*)();
    private:
        uint8_t pin_clk = 0;
        uint8_t pin_dt = 1;
        uint8_t pin_sw = 2;

        uint8_t encoderBits = 0b111; 	// 8 bits
        uint8_t rotaryBits = 0b11; 		// 8 bits
        uint8_t buttonBits = 0b111; 	// 8 bits

        uint32_t lastTouch = 0;
        uint32_t last_encoder_read = 0;
        
        Rotary rotary = stopped;
        Button button = released;
        Encoder encoder = inactive;

        functionHandleRotary handleRotate = nullptr;
        functionHandleRotary handlePressRotate = nullptr;
        functionHandleRotary handleHeldRotate = nullptr;
        functionHandleButton handlePress = nullptr;
        functionHandleButton handleLongPress = nullptr;
        functionHandleButton handleLongPressRelease = nullptr;
        functionHandleButton handlePressRotateRelease = nullptr;
        functionHandleButton handleHeldRotateRelease = nullptr;
    public:
        BoxEncoder(int, int, int);
        void ReadEncoder();
        Rotary getRotary();
        Button getButton();
        Encoder getEncoder();
        uint8_t getEncoderBits();
        uint8_t getButtonBits();
        void setHandleRotate(functionHandleRotary);
        void setHandlePressRotate(functionHandleRotary);
        void setHandleHeldRotate(functionHandleRotary);
        void setHandlePress(functionHandleButton);
        void setHandleLongPress(functionHandleButton);
        void setHandleLongPressRelease(functionHandleButton);
        void setHandlePressRotateRelease(functionHandleButton);
        void setHandleHeldRotateRelease(functionHandleButton);
};

#endif // VERSATILE_ROTARYENCODER_H_INCLUDED