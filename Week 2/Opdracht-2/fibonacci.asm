;
; Fibonacci.asm
;
; Created: 24/09/2020 08:36:51
; Author : Robbin Kok & Richard Ringia
;

.include "m328Pdef.inc"
.def f1 = r16;
.def f2 = r17;
.def tmp = r18;
main:
	ldi f1, 0;
	ldi f2, 1;

loop:
	mov tmp, f1; de waarde van tmp word f1
	mov f1, f2; de waarde van f1 wordt f2
	add f2, tmp; telt de waarde van tmp(f1) bij de waarde van f2 op
	cpi f2, 144
	breq end
	jmp loop

end:
	out DDRB, f2
	jmp end
