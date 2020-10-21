;
; Week 2 opdracht 3.asm
;
; Created: 21/10/2020 14:19:36
; Author : robbi
;

.include "m328Pdef.inc"
.def ANSL = r0
.def ANSH = r1
.def x_reg = r16
.def y_reg = r17
.def z_reg = r18
.def temp = r19

main:
	ldi x_reg, 3
	ldi y_reg, 1
	ldi z_reg, 1

loop:
	cpi z_reg, 255
	brsh end
	mul z_reg, x_reg
	mov z_reg, ANSL
	mov temp, z_reg
	sub temp, y_reg
	mov z_reg, temp
	rjmp loop


end:
	rjmp end
