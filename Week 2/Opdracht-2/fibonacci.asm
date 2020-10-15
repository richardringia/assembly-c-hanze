;
; Fibonacci.asm
;
; Created: 24/09/2020 08:36:51
; Author : Robbin Kok & Richard Ringia
;

.include "m328Pdef.inc"
.def f1 = r16 ; Definieer f1 op register 16
.def f2 = r17 ; Definieer f2 op register 17 
.def tmp = r18 ; Definieer temp op register 18

main:
	ldi f1, 0 ; F1 = 0
	ldi f2, 1 ; F2 = 1

loop:
	mov tmp, f1; De waarde van tmp word f1
	mov f1, f2; De waarde van f1 wordt f2
	add f2, tmp; Telt de waarde van tmp(f1) bij de waarde van f2 op
	cpi f2, 144 ; Vergelijkt de waarde van f2 met 144
	breq end ; Als de waarde gelijk is takken we af naar end
	jmp loop ; Laat de loop weer opnieuw beginnen

end:
	out DDRB, f2 ; Ze de waarde van f2 als output op DDRB (Data Direction Register B)
	jmp end ; Springt naar end en maakt hierdoor een infinite loop
