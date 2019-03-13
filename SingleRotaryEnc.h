#ifndef SINGLE_ROTARY_ROTARYENC_H
#define SINGLE_ROTARY_ROTARYENC_H

#if ARDUINO < 100

#include <WProgram.h>

#else

#include <Arduino.h>

#endif

class SingleRotaryEnc {

private:
		uint8_t clockPin;
		uint8_t dataPin;
		uint8_t buttonPin;

		void (*up)(void);

		void (*down)(void);

		void (*click)(void);

		void (*longPress)(void);


		boolean buttonPressed = 0;
		uint16_t buttonDebounce = 0xFFFF;
		uint32_t clickTime = 0;
		volatile uint8_t directionBuffer;
		volatile int32_t position = 0;
		volatile int32_t previousTime = 0;

		static SingleRotaryEnc *_instance;

		static void dataHasRisen();

		void dataRise();

public:
		SingleRotaryEnc(
						uint8_t clockPin,
						uint8_t dataPin,
						uint8_t buttonPin,
						void (*up)(void),
						void (*down)(void),
						void (*click)(void),
						void (*longPress)(void) = nullptr
		);

		void loop();

		int32_t getPosition();
};


#endif
