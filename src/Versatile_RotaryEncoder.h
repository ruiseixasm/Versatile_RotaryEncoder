/*
  Versatile_RotaryEncoder.h - Versatile_RotaryEncoder library
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
/**
 * Decomment below line to enable std::function as callbacks
*/
//#define USE_STD_FUNCTION

/**
 * Decomment below line to use internal pullup resistor
*/
#define USE_INTERNAL_PULLUP

#if defined(USE_INTERNAL_PULLUP)
#define INPUT_TYPE INPUT_PULLUP
#else
#define INPUT_TYPE INPUT
#endif

class Versatile_RotaryEncoder {
    public:
        enum Encoder : uint8_t {inactive, release, press, double_press, hold, rotate, pressrotate, heldrotate};
        enum Button : uint8_t {released, holdup, switchup, switchdown, double_switchdown, pressed, holddown, held};
        enum Rotary : int8_t {left = -1, stopped, right}; // Don't change these values, other functions are dependent on them
        #if defined(USE_STD_FUNCTION)
        using functionHandleRotary = std::function<void(int8_t)>;
        using functionHandleButton = std::function<void()>;
        #else
        using functionHandleRotary = void (*)(int8_t rotation);
        using functionHandleButton = void (*)();
        #endif
    private:
        uint8_t pin_clk = 0;
        uint8_t pin_dt = 1;
        uint8_t pin_sw = 2;

        uint8_t encoderBits = 0b111; 	// 8 bits
        uint8_t rotaryBits = 0b11; 		// 8 bits
        uint8_t buttonBits = 0b111; 	// 8 bits

        uint8_t inverted_switch = 0b0; // the default is from vcc to ground
        uint8_t read_interval_duration = 1; // by default reads the encoder each 1ms
        uint8_t short_press_duration = 50; // debounce duration to avoid noise triggering
        uint16_t long_press_duration = 1000;
        uint16_t double_press_duration = 250;
        uint32_t last_encoder_read = 0;
        uint32_t last_switch;
        uint32_t last_switchdown;
        
        Rotary rotary = stopped;
        Button button = released;
        Encoder encoder = inactive;

        functionHandleRotary handleRotate = nullptr;
        functionHandleRotary handlePressRotate = nullptr;
        functionHandleRotary handleHeldRotate = nullptr;
        functionHandleButton handlePress = nullptr;
        functionHandleButton handleDoublePress = nullptr; // A value of 'nullptr' disables Double Press
        functionHandleButton handlePressRelease = nullptr;
        functionHandleButton handleLongPress = nullptr;
        functionHandleButton handleLongPressRelease = nullptr;
        functionHandleButton handlePressRotateRelease = nullptr;
        functionHandleButton handleHeldRotateRelease = nullptr;
    public:
        Versatile_RotaryEncoder(uint8_t clk, uint8_t dt, uint8_t sw);
        bool ReadEncoder();
        void setInvertedSwitch(bool invert_switch);
        void setReadIntervalDuration(uint8_t duration);
        void setShortPressDuration(uint8_t duration);
        void setLongPressDuration(uint16_t duration);
        void setDoublePressDuration(uint16_t duration);
        Rotary getRotary();
        Button getButton();
        Encoder getEncoder();
        uint8_t getEncoderBits();
        uint8_t getButtonBits();
        void setHandleRotate(functionHandleRotary function_handler);
        void setHandlePressRotate(functionHandleRotary function_handler);
        void setHandleHeldRotate(functionHandleRotary function_handler);
        void setHandlePress(functionHandleButton function_handler);
        void setHandleDoublePress(functionHandleButton function_handler);
        void setHandlePressRelease(functionHandleButton function_handler);
        void setHandleLongPress(functionHandleButton function_handler);
        void setHandleLongPressRelease(functionHandleButton function_handler);
        void setHandlePressRotateRelease(functionHandleButton function_handler);
        void setHandleHeldRotateRelease(functionHandleButton function_handler);
};

#endif // VERSATILE_ROTARYENCODER_H_INCLUDED