extern printf

section .rodata
const1	dq	0.0
const2 	dq 	2.0

section .text
global f 

f:
	push 	ebp
	mov 	ebp, esp
	sub 	esp, 8
	
	finit
	
	fld		qword[const1]
	fld 	qword[const2]
	fld 	qword[ebp + 8]
	;fldpi
	
	fxch
	fdiv
	;fptan
	;fxch

	leave
	ret
