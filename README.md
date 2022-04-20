## **Versatile_RotaryEncoder Library V1.1.2** for Arduino
**Written by:** _Rui Seixas Monteiro_.

## Installation
Create a new folder called "Versatile_RotaryEncoder" under the folder named "libraries" in your Arduino sketchbook folder.
Create the folder "libraries" in case it does not exist yet. Place all the files in the "Versatile_RotaryEncoder" folder.

## Import
To use the library in your own sketch, select it from *Sketch > Import Library*.

## What is Versatile_RotaryEncoder
The Versatile_RotaryEncoder library allows the callback of up to 9 different functions representing the same number of different encoder events. These different functions can be associated with events like press rotate and long press among many others.

## Usage
### **How to include**
```Arduino
#include <Versatile_RotaryEncoder.h>

void setup(){

}

void loop(){

}
```

### **Library functions**
#### **`Versatile_RotaryEncoder(clk, dt, sw)`**
This function returns a pointer to an object of type `Versatile_RotaryEncoder` to be used as function calls and sets.

#### **`Versatile_RotaryEncoder.setHandleRotate( void (*)(int8_t rotation) )`**
This function sets the handle function for the rotation of the encoder.
Its only parameter is the handle function as `void (*)(int8_t rotation)` that would be used to process the rotation value.

#### **`Versatile_RotaryEncoder.setHandlePressRotate( void (*)(int8_t rotation) )`**
This function sets the handle function for the rotation of the encoder while being pressed.
Its only parameter is the handle function as `void (*)(int8_t rotation)` that would be used to process the rotation value.

#### **`Versatile_RotaryEncoder.setHandleHeldRotate( void (*)(int8_t rotation) )`**
This function sets the handle function for the rotation of the encoder after being long pressed while held.
Its only parameter is the handle function as `void (*)(int8_t rotation)` that would be used to process the rotation value.

#### **`Versatile_RotaryEncoder.setHandlePress( void (*)() )`**
This function sets the handle function for the button press of the encoder.
Its only parameter is the handle function as `void (*)()` that would be used to process the pressed encoder switch.

#### **`Versatile_RotaryEncoder.setHandlePressRelease( void (*)() )`**
This function sets the handle function for the button release of the encoder.
Its only parameter is the handle function as `void (*)()` that would be used to process the released encoder switch.

#### **`Versatile_RotaryEncoder.setHandleLongPress( void (*)() )`**
This function sets the handle function for the button long press of the encoder.
Its only parameter is the handle function as `void (*)()` that would be used to process the long pressed encoder switch.

#### **`Versatile_RotaryEncoder.setHandleLongPressRelease( void (*)() )`**
This function sets the handle function for the button long press released of the encoder.
Its only parameter is the handle function as `void (*)()` that would be used to process the release of the long pressed encoder switch.

#### **`Versatile_RotaryEncoder.setHandlePressRotateRelease( void (*)() )`**
This function sets the handle function for the button release of the encoder after being press rotated.
Its only parameter is the handle function as `void (*)()` that would be used to process the release encoder switch after being press rotated.

#### **`Versatile_RotaryEncoder.setHandleHeldRotateRelease( void (*)() )`**
This function sets the handle function for the button release of the encoder after being long press and then rotated.
Its only parameter is the handle function as `void (*)()` that would be used to process the release encoder switch after being held rotated.

#### **`Versatile_RotaryEncoder.ReadEncoder()`**
This functions reads the encoder and runs all Handle functions accordingly. It returns a `bool` with true whenever any handle function is called.

#### **`Versatile_RotaryEncoder.setReadIntervalDuration( byte )`**
By default every 2 ms the encoder is readed, you can set a diffrent value with this function.

#### **`Versatile_RotaryEncoder.setShortPressDuration( byte )`**
By default it's set 50 ms for the encoder switch debounce, you can set a diffrent value with this function.

#### **`Versatile_RotaryEncoder.setLongPressDuration( unsigned int )`**
By default it's set 1000 ms for the press be considered a long press, you can set a diffrent value with this function.

#### **`Versatile_RotaryEncoder.getRotary()`**
This functions returns a `short int` with a positive 1 or negative 1 accordingly to the rotation of the encoder.
The purpose of this function is to allow a more specific use of the library.

#### **`Versatile_RotaryEncoder.getButton()`**
This functions returns a `short unsigned int` with a value related to released, holdup, switchup, switchdown, pressed, holddown or held respectively.
The purpose of this function is to allow a more specific use of the library.

#### **`Versatile_RotaryEncoder.getEncoder()`**
This functions returns a `short unsigned int` with a value related to inactive, release, press, hold, rotate, pressrotate or heldrotate respectively.
The purpose of this function is to allow a more specific use of the library.

#### **`Versatile_RotaryEncoder.getEncoderBits()`**
This functions returns a `short unsigned int` representing the last rotary bits set by the encoder.
The purpose of this function is to allow a more specific use of the library.

#### **`Versatile_RotaryEncoder.getButtonBits()`**
This functions returns a `short unsigned int` representing the last button bits set by the encoder.
The purpose of this function is to allow a more specific use of the library.

### **Examples**
#### **Exaustive usage of all possible 9 functions**
```Arduino
#include <Versatile_RotaryEncoder.h>

// Set here your encoder reading pins (Ex.: EC11 with breakout board)
#define clk 17  // (A3)
#define dt 18   // (A4)
#define sw 19   // (A5)

// Functions prototyping to be handled on each Encoder Event
void handleRotate(int8_t rotation);
void handlePressRotate(int8_t rotation);
void handleHeldRotate(int8_t rotation);
void handlePress();
void handlePressRelease();
void handleLongPress();
void handleLongPressRelease();
void handlePressRotateRelease();
void handleHeldRotateRelease();

// Create a global pointer for the encoder object
Versatile_RotaryEncoder *versatile_encoder;

void setup() {
  
    Serial.begin(9600);
	versatile_encoder = new Versatile_RotaryEncoder(clk, dt, sw);

    // Load to the encoder all nedded handle functions here (up to 9 functions)
    versatile_encoder->setHandleRotate(handleRotate);
    versatile_encoder->setHandlePressRotate(handlePressRotate);
    versatile_encoder->setHandleHeldRotate(handleHeldRotate);
    versatile_encoder->setHandlePress(handlePress);
    versatile_encoder->setHandlePressRelease(handlePressRelease);
    versatile_encoder->setHandleLongPress(handleLongPress);
    versatile_encoder->setHandleLongPressRelease(handleLongPressRelease);
    versatile_encoder->setHandlePressRotateRelease(handlePressRotateRelease);
    versatile_encoder->setHandleHeldRotateRelease(handleHeldRotateRelease);

    Serial.println("Ready!");

}

void loop() {

    // Do the encoder reading and processing
    if (versatile_encoder->ReadEncoder()) {
        // Do something here whenever an encoder action is read
    }

}

// Implement your functions here accordingly to your needs

void handleRotate(int8_t rotation) {
	Serial.print("#1 Rotated: ");
    if (rotation > 0)
	    Serial.println("Right");
    else
	    Serial.println("Left");
}

void handlePressRotate(int8_t rotation) {
	Serial.print("#2 Pressed and rotated: ");
    if (rotation > 0)
	    Serial.println("Right");
    else
	    Serial.println("Left");
}

void handleHeldRotate(int8_t rotation) {
	Serial.print("#3 Held and rotated: ");
    if (rotation > 0)
	    Serial.println("Right");
    else
	    Serial.println("Left");
}

void handlePress() {
	Serial.println("#4 Pressed");
}

void handlePressRelease() {
	Serial.println("#5 Press released");
}

void handleLongPress() {
	Serial.println("#6 Long pressed");
}

void handleLongPressRelease() {
	Serial.println("#7 Long press released");
}

void handlePressRotateRelease() {
	Serial.println("#8 Press rotate released");
}

void handleHeldRotateRelease() {
	Serial.println("#9 Held rotate released");
}
```