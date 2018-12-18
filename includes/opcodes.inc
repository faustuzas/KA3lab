
;*****************************************************
;******************* 1B Commands *********************
;*****************************************************

	bPUSH2 		EQU 	01010000b		; 0101 0 reg 	- reg -> stack
	bPUSH3 		EQU 	00000110b		; 000 sreg 110 	- sreg -> stack
	
	bPOP2 		EQU 	01011000b		; 0101 1 reg 	- stack -> reg
	bPOP3 		EQU 	00000111b		; 000 sreg 111 	- stack -> sreg
	
	bINC2 		EQU 	01000000b		; 0100 0 reg 	- reg
	
	bDEC2 		EQU 	01001000b		; 0100 1 reg 	- reg
	
	bRET1 		EQU 	11000011b		; 1100 0011 	- internal without stack alignment
	bRET3 		EQU 	11001011b		; 1100 1011 	- external without stack alignment
	
	bINT2 		EQU 	11001100b		; 1100 1100		- int 3
	bNOP 		EQU 	10010000b		; 1001 0000		- NOP
	

;*****************************************************
;******************* 2B Commands *********************
;*****************************************************
	
	bJMP1 		EQU 	11101011b		;				- internal near
	bLOOP 		EQU 	11100010b
	bLOOPE 		EQU 	11100001b
	bLOOPNE		EQU 	11100000b
	bINT1 		EQU 	11001101b
	
	; Conditional control
	
	bJA 		EQU 	01110111b
	bJAE 		EQU 	01110011b
	bJB 		EQU 	01110010b
	bJBE 		EQU 	01110110b
	bJE 		EQU 	01110100b
	bJCXZ 		EQU 	11100011b
	bJG 		EQU 	01111111b
	bJGE 		EQU 	01111101b
	bJL 		EQU 	01111100b
	bJLE 		EQU 	01111110b
	bJNE 		EQU 	01110101b
	bJNO 		EQU 	01110001b
	bJNP 		EQU 	01111011b
	bJP 		EQU 	01111010b
	bJO 		EQU 	01110000b
	bJNS 		EQU 	01111001b
	bJS 		EQU 	01111000b
	
	
;*****************************************************
;******************* 2-3B Commands *********************
;*****************************************************
	
	bMOV3 		EQU 	10110000b		; 1011 w reg	- reg <- immed. Op.
	bADD3 		EQU 	00000100b		; 0000 010w		- accum. + immed. Op.
	bSUB3 		EQU 	00101100b		; 0010 110w		- accum. - immed. Op.
	bCMP3 		EQU 	00111100b		; 0011 110w		- accum. + immed. Op.
	bXOR3 		EQU 	00110100b		; 0011 010w		- accum. + immed. Op.
	bAND3 		EQU 	00100100b		; 0010 010w		- accum. + immed. Op.
	
	
;*****************************************************
;******************* 2-4B Commands *********************
;*****************************************************
	
	; 1111 1111 (w=1) format with extended code
	
	bCALL2 		EQU 	010b			; 1111 1111	010	- internal nedirect
	bCALL4 		EQU 	011b			; 1111 1111	011	- external nedirect
	bJMP3 		EQU 	100b			; 1111 1111	100	- internal nedirect
	bJMP5 		EQU 	101b			; 1111 1111	101	- external nedirect
	bPUSH1 		EQU 	110b			; 1111 1111	110	- r/m -> stack
	bINC1 		EQU 	000b			; 1111 111w	000	- r/m
	bDEC1 		EQU 	001b			; 1111 111w	001	- r/m
	
	; DIV and mul with 1111 0111 (w=1) format and extended code
	
	bMUL1 		EQU 	100b			; 1111 011w 100
	bDIV1 		EQU 	110b			; 1111 011w 110
	

	bPOP1 		EQU 	10001111b		; 1000 1111		- stack -> r/m
	
	bMOV6 		EQU 	10001100b		; 1000 11d0		- sreg <-> r/m
	bLEA		EQU 	10001101b
	bLDS		EQU 	11000101b
	bMOV1 		EQU 	10001000b		; 1000 10dw		- reg <-> r/m
	bADD1 		EQU 	00000000b		; 0000 00dw		- reg ~ r/m
	bSUB1 		EQU 	00101000b		; 0010 10dw		- reg ~ r/m
	bCMP1 		EQU 	00111000b		; 0011 10dw		- reg ~ r/m
	bXOR1 		EQU 	00110000b		; 0011 00dw		- reg ~ r/m
	bAND1 		EQU 	00100000b		; 0010 00dw		- reg ~ r/m


;*****************************************************
;******************* 3B Commands *********************
;*****************************************************
	
	bMOV4 		EQU 	10100000b		; 1010 000w		- accum. <- memory
	bMOV5 		EQU 	10100010b		; 1010 001w		- accum. + immed. Op.
	bCALL1 		EQU 	11101000b		; 				- internal direct
	bRET2 		EQU 	11000010b		; 				- internal with stack alignment
	bRET4 		EQU 	11001010b		; 				- external with stack alignment
	bJMP2 		EQU 	11101001b		; 				- internal direct
	
	
;*****************************************************
;******************* 3-6B Commands *********************
;*****************************************************
	
	bMOV2 		EQU 	11000110b		; 1100 011w		- r/m <- immed. Op. 
	
	; 1000 00sw format with extended code
	
	bADD2 		EQU 	000b			; 1000 00sw		- r/m + immed. Op. 
	bSUB2 		EQU 	101b			; 1000 00sw		- r/m - immed. Op.
	bCMP2 		EQU 	111b			; 1000 00sw		- r/m - immed. Op.
	bXOR2 		EQU 	110b			; 1000 00sw		- r/m - immed. Op.
	bAND2 		EQU 	100b			; 1000 00sw		- r/m - immed. Op.
	
	
;*****************************************************
;******************* 5B Commands *********************
;*****************************************************
	
	bCALL3 		EQU 	10011010b		;				- external direct
	bJMP4 		EQU 	11101010b		;				- external direct
	
	
;*****************************************************
;*********** Commands as strings**********************
;*****************************************************
	
	sMOV		DB		'mov '	, 0
	sPUSH		DB		'push '	, 0
	sPOP		DB		'pop '	, 0
	sADD		DB		'add '	, 0
	sINC		DB		'inc '	, 0
	sSUB		DB		'sub '	, 0
	sDEC		DB		'dec '	, 0
	sCMP		DB		'cmp '	, 0
	sMUL		DB		'mul '	, 0
	sDIV		DB		'div '	, 0
	sCALL		DB		'call '	, 0
	sRET		DB		'ret '	, 0
	sJMP		DB		'jmp '	, 0
	sLOOP		DB		'loop '	, 0
	sINT1		DB		'int '	, 0
	sINT2		DB		'int 3 ', 0
	sNOP		DB		'nop '	, 0
	sXOR		DB		'xor '	, 0
	sAND		DB		'and '	, 0
	sLEA 		DB		'lea '	, 0
	sLDS		DB		'lds '	, 0
	sLOOPE		DB		'loope ', 0
	sLOOPNE		DB		'loopne ', 0
	; Conditional control
	
	sJA			DB		'ja '	, 0			; JNBE
	sJAE		DB		'JAE '	, 0			; JNB / JNC
	sJB			DB		'JB '	, 0			; JNAE / jc
	sJBE		DB		'JBE '	, 0			; jna
	sJE			DB		'je '	, 0			; JZ
	sJCXZ		DB		'JCXZ '	, 0			
	sJG			DB		'JG '	, 0			; JNLE
	sJGE		DB		'JGE '	, 0			; JNL
	sJL			DB		'JL '	, 0			; JNGE
	sJLE		DB		'JLE '	, 0			; JNG
	sJNE		DB		'jne '	, 0			; JNZ
	sJNO		DB		'JNO '	, 0		
	sJNP		DB		'JNP '	, 0			; JPO
	sJP			DB		'JP '	, 0			; JPE
	sJO			DB		'JO '	, 0
	sJNS		DB		'JNS '	, 0
	sJS			DB		'JS '	, 0
	