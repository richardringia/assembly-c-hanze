;
; DefaultProject.asm
;
; Created: 28/09/2020 10:41:15
; Author : Richard Ringia (r.ringia@st.hanze.nl) and Robbin Kok (rb.kok@st.hanze.nl)
;


; Replace with your application code
.dseg 
.org SRAM_START ; go to address SRAM_START
dest: .byte 20 ; array on address dest
 
.cseg
	rjmp start 
 
src: .db "hello world !" ; define string hello world
.equ length=13 ; definte length to 13
.def temp = r0 ; r0 is a temporary register
.def counter = r17 ; r17 will hold counter value
 
start:      
	ldi zh,high(2*src) ; Z-pointer wijst naar hoogste waarde van src keer 2
	ldi zl,low(2*src) ; Z-pointer wijst naar laagste waarde van src keer 2    
	ldi xh,high(dest)  ; X-pointer wijst naar hoogste waarde van de array dest
	ldi xl,low(dest)  ; X-pointer wijst naar laagste waarde van de array dest 
	clr counter ; Clear de counter
loop:      	
	lpm ; Load data memory
	inc zl ; Increment de waarde van Z1     
	st x+,temp     ; Store indirect en post incrment 
	inc counter ; Increment de counter 
	cpi counter, length     ;Compered de counter met de lengte
	brlt loop ; Springt deze uit de loop (exit clause)
end:      
	rjmp end ; Springt naar het einde