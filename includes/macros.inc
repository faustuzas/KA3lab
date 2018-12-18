; Print a string
PrintString MACRO string
	
	lea	dx, string
	mov	ah, 09h
	int	21h

ENDM

; CMP and JE
CMP_JE MACRO value1, value2, location
	
	cmp value1, value2
	je location

ENDM

; Add the string to write buffer
AddTheString MACRO string
	
	lea bx, string
	call AddString

ENDM

; Add to buffer a byte
AddToBuffer MACRO value

	mov [di], byte ptr value				; Add value
	inc di									; Jump to next value
	inc bytesToWrite						; Increase counter

ENDM

; Get wanted bits (without shr)
GetBits MACRO where, andTemplate, source
											; what - where to save
											; andTemplate - the binary template which bits do we need
											; source - register (mostly DL or DH)
	
	mov where, source
	and where, andTemplate

ENDM

; Get wanted bits (with shr)
GetBitsSHR MACRO where, andTemplate, shift, source
											; where - where to save
											; andTemplate - the binary template which bits do we need
											; shift shr shift value
											; source - register (mostly DL or DH)

	GetBits where, andTemplate, source
	shr where, shift

ENDM

; What reg or sreg to add
AnalizeSregOrReg MACRO DBS, value
											; DBS - strings database
											; value - sreg or reg

	push cx
	
	lea bx, DBS
	mov cl, 2h					; Multiply value
	
	mov al, [value]
	mul cl
	xlat						; AL = DS:[BX + unsigned AL]  
	
	AddToBuffer al
	
	mov al, [value]
	mul cl
	inc al						; Next symbol then before
	xlat						; AL = DS:[BX + unsigned AL]  
	
	AddToBuffer al
	
	pop cx

ENDM
