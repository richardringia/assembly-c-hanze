;
; keep_leds_on.asm
;
; Created: 28/09/2020 10:41:15
; Author : Richard Ringia (r.ringia@st.hanze.nl) and Robbin Kok (rb.kok@st.hanze.nl)
;
.cseg
.org 0
ldi r16, 0xff ; Registreer 255 op register 16
out DDRD, r16 ; Schrijf register 16 op output DDRD

loop:
	in r16, PINB ; Checkt wanneer er een input 1 wordt

	andi r16, 0b00000011 ; Kijken of de gegeven waarde een 0 of 1 moet zijn
	
	cpi r16, 1 ; Kijken of de input 1 is
	brge set_leds ; Wanneer er een 1 is wordt het naar de output gestuurd

	rjmp loop ; Opnieuw de loop uitvoeren

set_leds:
	out PORTD, r16 ; Schrijf de input naar de output

	rjmp loop ; opnieuw de loop uitvoeren
