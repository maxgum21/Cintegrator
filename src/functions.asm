section .data
a:		dq	0.000000
b:		dq	4.000000
num:		dd	3
func_arr:	dd	f1, f2, f3, df1, df2, df3
const1:		dq	4.000000
const2:		dq	2.000000
const3:		dq	7.000000
const4:		dq	0.200000
const5:		dq	-0.250000
const6:		dq	1.000000
const7:		dq	0.000000

section .text

global a
global b
global num
global func_arr
f1:
	push	ebp
	mov	ebp, esp

	fld	qword[const1]

	fld	qword[ebp + 8]

	fxch

	fdivp

	fptan

	fstp	st0
	fld	qword[const2]

	fxch

	fsubp

	fld	qword[const3]

	faddp

	leave
	ret

f2:
	push	ebp
	mov	ebp, esp

	fld	qword[const3]

	fld	qword[ebp + 8]

	fxch

	faddp

	leave
	ret

f3:
	push	ebp
	mov	ebp, esp

	fldpi

	fld	qword[const4]

	fxch

	fmulp

	fld	qword[const3]

	faddp

	leave
	ret

df1:
	push	ebp
	mov	ebp, esp

	fld	qword[const1]

	fld	qword[ebp + 8]

	fxch

	fdivp

	fcos

	fld	qword[const1]

	fld	qword[ebp + 8]

	fxch

	fdivp

	fcos

	fmulp

	fld	qword[const5]

	fxch

	fdivp

	leave
	ret

df2:
	push	ebp
	mov	ebp, esp

	fld	qword[const6]

	leave
	ret

df3:
	push	ebp
	mov	ebp, esp

	fld	qword[const7]

	leave
	ret

