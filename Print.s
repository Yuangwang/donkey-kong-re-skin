; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 
VAR		EQU	2		;this is number of local variables
DIV		EQU	0
Lead	EQU	4
	
    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
	PUSH	{R0,R1,R2,R3,R4,LR}
	CMP		R0, #0
	BEQ		PrintZero
	MOV		R1, #0
	MOV		R2, #0
	SUB 	SP, #8
	;PUSH	{R1,R2}			;allocation
	MOV		R11, SP			;now R11 has location of where local variables are
	MOV		R1, #10000
	MOV		R2, #10
	MUL		R1, R1, R1
	MUL		R1, R1, R2
	STR		R1, [R11, #DIV]
	MOV		R3, #0
	STR		R3, [R11, #Lead]
ODLoop
	LDR		R1, [R11, #DIV]
	CMP		R1, #0
	BEQ		LastCase
	MOV		R3, R0
	UDIV	R3, R1
	MUL		R4, R1, R3
	SUB		R0, R4
	PUSH	{R0,R1,R2,R3,R4,R11}
	MOV		R0, R3
	CMP		R0, #0		
	BEQ		CheckZero
	LDR		R1, [R11, #Lead]
	ADD		R1, #1
	STR		R1, [R11, #Lead]
	ADD		R0, #0X30
	BL		ST7735_OutChar
ODRest
	POP		{R0,R1,R2,R3,R4,R11}
	LDR		R1, [R11, #DIV]
	MOV		R2, #10
	UDIV	R1, R2
	STR 	R1, [R11, #DIV]
	B 		ODLoop

CheckZero
	LDR		R1, [R11, #Lead]
	CMP		R1, #0
	BEQ		ODRest
	PUSH	{R0,R11}
	MOV		R0, #0X30
	BL		ST7735_OutChar
	POP		{R0,R11}
	B 		ODRest


LastCase
	POP		{R0,LR}
	POP		{R0,R1,R2,R3,R4,LR}
	BX		LR

PrintZero
	PUSH	{R0,R11}
	MOV		R0, #0X30
	BL		ST7735_OutChar
	POP		{R0, R11}
	POP		{R0,R1,R2,R3,R4,LR}
	BX		LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
	PUSH	{R0-R3,R11,LR}
	MOV		R1, #0
	MOV		R2, #0
	PUSH	{R1,R2}			;allocation
	MOV		R11, SP			;now R11 has location of where local variables are
	MOV		R1, #10000
	STR		R1, [R11, #DIV]
	LDR		R1, [R11, #DIV]
	CMP		R0, R1
	BHS		TooBig

	BL		OutDig				;ones place
	
	PUSH	{R0-R3,R11,LR}
	MOV		R0, #0X2C			;decimal
	BL		ST7735_OutChar
	POP		{R0-R3,R11,LR}

	BL		OutDig				;tenths place
	
	BL		OutDig				;hundredths place

	BL		OutDig				;thousandths place

rest
	POP		{R1,R2}
	POP		{R0-R3,R11,LR}
	BX		LR

OutDig
	MOV		R2, #10
	UDIV	R1, R2
	STR		R1, [R11, #DIV]
	LDR		R1, [R11, #DIV]
	MOV 	R2, R0
	UDIV	R2, R1
	MUL		R3, R1, R2
	SUB		R0, R3
	PUSH	{R0-R3,R11,LR}
	MOV		R0, R2
	ADD		R0, #0X30
	BL		ST7735_OutChar
	POP		{R0-R3,R11,LR}
	BX		LR

TooBig	
	MOV		R0, #0X2A				;*
	BL		ST7735_OutChar
	MOV		R0, #0X2E				;.
	BL		ST7735_OutChar
	MOV		R0, #0X2A				;*
	BL		ST7735_OutChar
	MOV		R0, #0X2A				;*
	BL		ST7735_OutChar
	MOV		R0, #0X2A				;*
	BL		ST7735_OutChar
	B 		rest
;* * * * * * * * End of LCD_OutFix * * * * * * * *

    ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file
