#include "SingleRotaryEnc.h"

#define POLLING_TIMEOUT 25
#define LONG_PRESS_TIME 500

SingleRotaryEnc::SingleRotaryEnc(
				uint8_t clockPin,
				uint8_t dataPin,
				uint8_t buttonPin,
				void (*up)(void),
				void (*down)(void),
				void (*click)(void),
				void (*longPress)(void)
) {
		this->clockPin = clockPin;
		this->dataPin = dataPin;
		this->buttonPin = buttonPin;
		this->up = up;
		this->down = down;
		this->click = click;
		this->longPress = longPress;

		pinMode(clockPin, INPUT);
		pinMode(dataPin, INPUT);
		pinMode(buttonPin, INPUT);
		_instance = this;
		attachInterrupt(digitalPinToInterrupt(dataPin), &SingleRotaryEnc::dataHasRisen, RISING);
}

SingleRotaryEnc *SingleRotaryEnc::_instance;

void SingleRotaryEnc::dataHasRisen() {
		_instance->dataRise();
}

void SingleRotaryEnc::dataRise() {
		boolean clock = digitalRead(clockPin);
		boolean data = digitalRead(dataPin);

		noInterrupts();
		if (data) {
				data == clock ? (directionBuffer = directionBuffer * 4 + 0b01) : (directionBuffer = directionBuffer * 4 + 0b10);
				previousTime = millis();
		} else {
				directionBuffer = directionBuffer * 4;
		}
		interrupts();
}

void SingleRotaryEnc::loop() {

		buttonDebounce = buttonDebounce * 2 | digitalRead(buttonPin);

		if ((buttonDebounce | 0x8000) == 0xC000) {
				buttonPressed = 1;
				buttonDebounce = 0;
				clickTime = millis();
		} else if (buttonPressed && buttonDebounce == 0x7fff) {
				if (longPress != nullptr && millis() - clickTime >= LONG_PRESS_TIME) {
						longPress();
				} else {
						click();
				}
				buttonPressed = 0;
				clickTime = millis();
		} else if (!buttonPressed && millis() - previousTime >= POLLING_TIMEOUT && (directionBuffer & 0b00000011) != 0) {
				if ((directionBuffer & 0b00001111) == 0b00000001 || (directionBuffer & 0b00001111) == 0b00000101) {
						up();
						position++;
				} else if ((directionBuffer & 0b00001111) == 0b00000010 || (directionBuffer & 0b00001111) == 0b00001010) {
						down();
						position--;
				}
				directionBuffer = directionBuffer * 4;
		}

}

int32_t SingleRotaryEnc::getPosition() {
		return position;
}

