#include <Arduino.h>
#include <SingleRotaryEnc.h>

#define CLK_PIN  2
#define DATA_PIN 3
#define BUTTON_PIN 4

SingleRotaryEnc *rotaryEnc;

void up() {
		Serial.print(F("CW: "));
		Serial.println(rotaryEnc->getPosition());
}

void down() {
		Serial.print(F("CCW: "));
		Serial.println(rotaryEnc->getPosition());
}

void click() {
		Serial.println(F("CLICK: "));
}

void longPress() {
		Serial.println(F("LONG PRESS: "));
}

////////////////////////////////////////////////////
void setup() {
		pinMode(CLK_PIN, INPUT);
		pinMode(DATA_PIN, INPUT);

		Serial.begin(115200);
		rotaryEnc = new SingleRotaryEnc(CLK_PIN, DATA_PIN, BUTTON_PIN, up, down, click, longPress);
}

////////////////////////////////////////////////////
void loop() {
		rotaryEnc->loop();
}