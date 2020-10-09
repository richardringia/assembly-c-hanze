#include <avr/io.h>
#include <stdint.h>
#define F_CPU 16E6
#include <util/delay.h>

#define HIGH 0x1
#define LOW  0x0

const uint8_t data = 0;
const uint8_t clock = 1;
const uint8_t strobe = 2;

uint8_t counting(void);

void write(uint8_t pin, uint8_t val)
{
	if (val == LOW) {
		PORTB &= ~(_BV(pin)); // clear bit
		} else {
		PORTB |= _BV(pin); // set bit
	}
}

void shiftOut (uint8_t val)
{
	uint8_t i;
	for (i = 0; i < 8; i++)  {
		write(clock, LOW);   // bit valid on rising edge
		write(data, val & 1 ? HIGH : LOW); // lsb first
		val = val >> 1;
		write(clock, HIGH);
	}
}

void sendCommand(uint8_t value)
{
	write(strobe, LOW);
	shiftOut(value);
	write(strobe, HIGH);
}


uint8_t counting()
{
	/0/  /1/   /2/  /3/  /4/  /5/  /6/  /7/   /8/  /9/
	uint8_t digits[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f };

	static uint8_t digit = 0;

	sendCommand(0x40); // auto-increment address
	write(strobe, LOW);
	shiftOut(0xc0); // set starting address = 0
	for(uint8_t position = 0; position < 8; position++)
	{
		shiftOut(digits[digit]);
		_delay_ms(100);
		shiftOut(0x00);
	}

	write(strobe, HIGH);

	digit = (digit + 1) % 10;
	return (digit == 0);
}

void setup() {
	DDRB=0xff; // set port B as output
	
	
}

int main() {
	typedef enum {COUNTING_MODE=0} mode_t;
	mode_t mode = COUNTING_MODE;
	uint8_t i = 0;

	setup();

	while (1) {
		switch(mode)
		{
			case COUNTING_MODE:
				mode += counting(); // keep on counting until done
			break;
		}

		_delay_ms(200);
	}
	
	return (0);
}
