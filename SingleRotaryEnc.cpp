#include "SingleRotaryEnc.h"

#define POLLING_TIMEOUT 25

SingleRotaryEnc::SingleRotaryEnc(
				uint8_t clockPin,
				uint8_t dataPin,
				uint8_t buttonPin,
				void (*up)(void),
				void (*down)(void),
				void (*click)(void)
) {
		this->clockPin = clockPin;
		this->dataPin = dataPin;
		this->buttonPin = buttonPin;
		this->up = up;
		this->down = down;
		this->click = click;

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

		if ((buttonDebounce | 0b1000000000000000) == 0b1100000000000000) {
				buttonDebounce = 0;
				directionBuffer = directionBuffer * 4;
				click();
		} else if (millis() - previousTime >= POLLING_TIMEOUT && (directionBuffer & 0b00000011) != 0) {
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

