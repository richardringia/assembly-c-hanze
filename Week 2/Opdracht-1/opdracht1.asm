.include "m328Pdef.inc"
.def a = r16 ; definieer a
.def b = r17 ; definieer b
.def c = r18 ; definieer c

main:
	ldi a, 3; a = 3
	ldi b, 7; b = 3

loop:
	tst a
	breq end 
	dec a ; van a wordt er 1 afgehaald
	dec b ; van b wordt er 1 afgehaald
	mov c, a ; c is de waarde van a

	cpi b, 6 ; je kijkt of de waarde van b gelijk is aan 6
	brne else; springt naar else
	sbc b, a ; haalt de waarde van a van b af

	rjmp loop

else:
	cpi b, 3; je kijkt of je waarde van b gelijk is aan 3
	dec a ; haalt weer 1 van a af

	rjmp loop

end:
	rjmp end
	
