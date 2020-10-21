;
; Fibonacci.asm
;
; Created: 24/09/2020 08:36:51
; Author : Robbin Kok & Richard Ringia
;

.include "m328Pdef.inc"
.def a = r16 ; Definieer a op register 16
.def b = r17 ; Definieer b op register 17
.def c = r18 ; Definieer c op register 18

main:
	ldi a, 3; a = 3
	ldi b, 7; b = 3

loop:
	tst a ; Test of a gelijk of kleiner is dan 0
	breq end ; Als a kleiner of gelijk is aan 0, dan takken we af naar end
	dec a ; Van a wordt er 1 afgehaald
	dec b ; Van b wordt er 1 afgehaald
	mov c, a ; C is de waarde van a

	cpi b, 6 ; Je kijkt of de waarde van b gelijk is aan 6
	brne else; Springt naar else
	sbc b, a ; Haalt de waarde van a van b af

	rjmp loop ; Laat de loop weer opnieuw beginnen

else:
	cpi b, 3; Je kijkt of je waarde van b gelijk is aan 3
	dec a ; Haalt weer 1 van a af

	rjmp loop ; Springt naar loop en laat hem daardoor weer opnieuw beginnen

end:
	rjmp end ; Spring naar end en maakt hierdoor een infinite loop
	
