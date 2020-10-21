;
; Opdracht8.asm
;
; Created: 21/10/2020 13:45:56
; Author : Richard Ringia & Robbin Kok
;

.include "m328Pdef.inc"
.def saveSR = r17
.def temp = r18
.org 0x0000

rjmp init

.org OC1Aaddr
rjmp TIMER1_COMP_ISR

init:
	ldi R16, high(RAMEND)
	out SPH, R16
	ldi R16, low(RAMEND)
	out SPL, R16

	ldi temp, high(1250)
	sts OCR1AH, temp
	ldi temp, low(1250)
	sts OCR1AL, temp
	
	ldi temp, (1 << CS12) | (1 << WGM12)
	sts TCCR1B, temp
	
	ldi temp, (1 << OCIE1A)
	sts TIMSK1, temp

	ser temp			; temp = $FF
	out DDRB, temp
	out PORTB, temp

	sei

loop:
	rjmp loop

TIMER1_COMP_ISR:
	in saveSR, SREG		; Slaat de SREG op
	in r16, PORTB		; Schrijft de inhoud van PORTB naar r16
	com r16				; invert alle bits van $00 <-> $FF
	out PORTB, r16		; schrijf de waarde van r16 naar PORTB
	out SREG, saveSR	; herstelt de SREG
	reti				; return from interupt
