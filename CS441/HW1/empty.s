.text
.global _start
_start:
	mov r7, #12
	mov r3, #4
	mov r0, #7
	LSL r7, r7, r3
	ADD r6, r7, #7

