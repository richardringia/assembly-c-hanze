#include <avr/io.h>



int main(void) {
	DDRB = 0x00; // Set de waarde van DDRB naar read
	DDRD = 0xff; // Set de waarde van DDRD naar write
	
	while (1) { // Een oneindige loop maken
		int temp = PINB & 0b00000011; // Temp wordt de uitkomst van de AND met PINB en 0b00000011
		
		if(temp > 0) { // Kijkt of de waarde van temp groter is dan o
			PORTD = temp; // Set de waarde van temp naar PORTD
		}
	}
}
