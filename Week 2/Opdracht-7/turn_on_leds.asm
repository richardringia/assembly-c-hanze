;
; DefaultProject.asm
;
; Created: 28/09/2020 10:41:15
; Author : Richard Ringia (r.ringia@st.hanze.nl) and Robbin Kok (rb.kok@st.hanze.nl)
;
.cseg
.org 0
ldi r16, 0xff ; registreer 255 op register 16
out DDRD, r16 ; schrijf register 16 op output DDRD

loop:
	in r16, PINB ; checkt wanneer eer een input 1 wordt
	out PORTD, r16 ; schrijf de input naar de output

	rjmp loop ; opnieuw de loop uitvoeren