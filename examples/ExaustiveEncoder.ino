#include <Arduino.h>
#include <Versatile_RotaryEncoder.h>

// Set here your encoder reading pins
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

    // Load to the encoder all nedded handle functions here
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
	Serial.print("Rotated: ");
    if (rotation > 0)
	    Serial.println("Right");
    else
	    Serial.println("Left");
}

void handlePressRotate(int8_t rotation) {
	Serial.print("Pressed and rotated: ");
    if (rotation > 0)
	    Serial.println("Right");
    else
	    Serial.println("Left");
}

void handleHeldRotate(int8_t rotation) {
	Serial.print("Held and rotated: ");
    if (rotation > 0)
	    Serial.println("Right");
    else
	    Serial.println("Left");
}

void handlePress() {
	Serial.println("Pressed");
}

void handlePressRelease() {
	Serial.println("Press released");
}

void handleLongPress() {
	Serial.println("Long pressed");
}

void handleLongPressRelease() {
	Serial.println("Long pressed released");
}

void handlePressRotateRelease() {
	Serial.println("Press rotate released");
}

void handleHeldRotateRelease() {
	Serial.println("Held rotate released");
}