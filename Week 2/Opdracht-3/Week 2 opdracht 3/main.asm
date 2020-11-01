;
; Week 2 opdracht 3.asm
;
; Created: 21/10/2020 14:19:36
; Author : robbi
;

.include "m328Pdef.inc"
.dseg
.org 0x100
dest: .byte 20
.cseg
ldi r16, 1
ldi r17, 3
ldi xh, high(dest)
ldi xl, low(dest)

loop:
	st x+, r16
	mul r16, r17
	mov r16, r0
	st x+, r16
	dec r16
	mov r18, r1
	cpi r18, 1
	brge end
	rjmp loop


end:
	rjmp end
