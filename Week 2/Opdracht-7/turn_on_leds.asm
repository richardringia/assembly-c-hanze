;
; turn_leds_on.asm
;
; Created: 28/09/2020 10:41:15
; Author : Richard Ringia (r.ringia@st.hanze.nl) and Robbin Kok (rb.kok@st.hanze.nl)
;
.cseg
.org 0
ldi r16, 0xff ; Registreer 255 op register 16
out DDRD, r16 ; Schrijf register 16 op output DDRD

loop:
	in r16, PINB ; Checkt wanneer eer een input 1 wordt
	out PORTD, r16 ; Schrijf de input naar de output

	rjmp loop ; Opnieuw de loop uitvoeren
