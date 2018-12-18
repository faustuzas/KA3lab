; Get next byte, either from buffer or from file if needed
GetNextByte PROC
				; DH - save location
		
	inc offsetCS									; Increase offset
	
	CMP_JE cx, 1, GetNextByte__getTheByteFromFile	; Do we have a byte in the buffer? If no, get it from file
	
	inc si											; Jump to next index
	dec cx											; Decrease loop counter
	mov	dh, [si]									; Save second byte in DH
	jmp GetNextByte__return
	
  	GetNextByte__getTheByteFromFile:
	push dx											; In DL there's still may be our byte before.

	mov	cx, 1										; CX - bytes we need to bytesRead from file
	lea dx, eByte									; Place where to bytesRead information
	call ReadFile									; Read from file
	
	pop dx
	
	mov dh, [eByte]
	
  	GetNextByte__return:
	call CollectBytes
	ret

GetNextByte ENDP

; Add 1 byte as hex to the buffer
AddNext1B PROC
	
	call GetNextByte
	mov bh, 1						; Using bl as a temp flag to say that we need 0 before A-F
	call AddBin2Hex					; Add DH to the buffer
	AddToBuffer 'h'					; Add 'h' at the end
	
	ret

AddNext1B ENDP

; Add 2 bytes as hex to the buffer (always in reverse order)
AddNext2B PROC
	
	call GetNextByte
	xchg dh, dl						; Free dh, and put needed byte in dl
	call GetNextByte				; Get next byte to dh

	mov bh, 1						; Using bl as a temp flag to say that we need 0 before A-F
	call AddBin2Hex					; Add DH to the buffer
	
	mov bh, 0						; Using bh as a temp flag to say that we don't need 0 before A-F
	mov dh, dl						; Don't need old DH value at this point
	call AddBin2Hex					; Add DH to the buffer
	
	AddToBuffer 'h'					; Add 'h' at the end
	ret

AddNext2B ENDP

; Add hex value from DL to the buffer
PROC AddBin2Hex
				; DH - the bin code
				; BH - temp flag to indicate do we need 0 before hex (A-F) 
	
	push ax
	
	CMP_JE bh, 0, AddBin2Hex__skipZero
	
	GetBits bl, 11110000b, dh		; Get first hex
	cmp bl, 10010000b				; Compare with 90
	jna AddBin2Hex__skipZero
	AddToBuffer '0'					; Add zero before hex value, if A or above
	
  	AddBin2Hex__skipZero:
    lea bx, hexTable			
	
	GetBitsSHR al, 11110000b, 4, dh ; Get 1111 0000			
	xlat							; AL = DS:[BX + unsigned AL]  
	AddToBuffer al					; Add the hex value to write buffer		
	
	GetBits al, 00001111b, dh  		; Get 0000 1111			
	xlat							; AL = DS:[BX + unsigned AL]  
	AddToBuffer al					; Add the hex value to write buffer	
	
	pop ax
	
	ret

AddBin2Hex ENDP

; Add string from BX to the buffer
PROC AddString
				; BX - the string
  
	push si
	push dx
	
	xor si, si						; Local SI for index'ing
	
  	AddString__loop:
	mov dl, [bx+si]					; Move one symbol to DL
		
	CMP_JE dl, 0, AddString__return	; End of string?
	
	AddToBuffer dl
	inc si        					; Get next symbol

	jmp AddString__loop
	
  	AddString__return:
	pop dx
	pop si
	ret
	
AddString ENDP

; Find segment register
PROC FindSreg
				; sreg - the sreg code
	
	AnalizeSregOrReg SRegDB, sreg
    ret
	
FindSreg ENDP


; Find register
FindReg PROC
				; reg - the reg code
				; w - the w bit

	CMP_JE w, 0, FindReg__add1Breg
	CMP_JE w, 1, FindReg__add2Breg
	
	FindReg__add1Breg:								; Add 1B reg
	AnalizeSregOrReg sReg1BDB, reg
	jmp FindReg__return
	
 	FindReg__add2Breg:								; Add 2B reg
	AnalizeSregOrReg sReg2BDB, reg
	jmp FindReg__return
	

  	FindReg__return:
    ret
	
FindReg ENDP

; Find r/m
FindRM  PROC
				; md - the mod bits
	
	CMP_JE md, 11b, FindRM__mod11
	jne FindRM__notMod11
		
	FindRM__mod11:

	mov bl, rm
	mov bh, reg							; Save reg value for later restoring
	mov reg, bl							; Bassically say that reg = r/m
	call FindReg
	mov reg, bh							; Restore old reg value
	
	jmp FindRM__return


  	FindRM__notMod11:

	call AddSegmentChange				; Add segment change if needed
	
	CMP_JE rm, rmBxSi, 	 FindRM__rmBxSi
	CMP_JE rm, rmBxDi, 	 FindRM__rmBxDi
	CMP_JE rm, rmBpSi, 	 FindRM__rmBpSi
	CMP_JE rm, rmBpDi, 	 FindRM__rmBpDi
	CMP_JE rm, rmSi, 	 FindRM__rmSi
	CMP_JE rm, rmDi, 	 FindRM__rmDi
	CMP_JE rm, rmTiesBp, FindRM__rmTiesBp
	CMP_JE rm, rmBx, 	 FindRM__rmBx
	

	FindRM__rmBxSi:
	AddTheString sRmBxSi
	jmp FindRM__checkWhatMod
	
	FindRM__rmBxDi:
	AddTheString sRmBxDi
	jmp FindRM__checkWhatMod
	
	FindRM__rmBpSi:
	AddTheString sRmBpSi
	jmp FindRM__checkWhatMod
	
	FindRM__rmBpDi:
	AddTheString sRmBpDi
	jmp FindRM__checkWhatMod
	
	FindRM__rmSi:
	AddTheString sRmSi
	jmp FindRM__checkWhatMod
	
	FindRM__rmDi:
	AddTheString sRmDi
	jmp FindRM__checkWhatMod
	
	FindRM__rmTiesBp:
	cmp md, 00b							; Direct adress?
	jne FindRM__addSrmBp
	
	; If direct adress
	AddToBuffer '['
	call AddNext2B
	jmp FindRM__addBracketsEnd
	
  	FindRM__addSrmBp:
	AddTheString sRmBp
	jmp FindRM__checkWhatMod
	
	FindRM__rmBx:
	AddTheString sRmBx
	jmp FindRM__checkWhatMod
	
	
	; Check mod to add the offset
	
  	FindRM__checkWhatMod:
	CMP_JE md, 00b, FindRM__addBracketsEnd		; 0B offset
	CMP_JE md, 01b, FindRM__mod01				; 1B offset
	CMP_JE md, 10b, FindRM__mod10				; 2B offset
	
	FindRM__mod01:
	AddToBuffer '+'
	call AddNext1B						; 1B offset
	jmp FindRM__addBracketsEnd
	
	FindRM__mod10:
	AddToBuffer '+'
	call AddNext2B						; 2B offset
	jmp FindRM__addBracketsEnd
	
	
  	FindRM__addBracketsEnd:
   	AddToBuffer ']'
	
  	FindRM__return:
    ret
	
FindRM ENDP

; Add Prefix
AddOffsetCS PROC

	push cx
	
	mov cx, [offsetCS]					; Save offset in cx
	
	mov bh, 0							; Using bh as a temp flag to say that we don't need 0 before A-F
	mov dh, ch							; First will add first two hex values
	call AddBin2Hex						; Add DH to the buffer
	
	mov bh, 0							; Using bh as a temp flag to say that we don't need 0 before A-F
	mov dh, cl							; Then will add second two hex values
	call AddBin2Hex						; Add DH to the buffer
	
	pop cx
	
	AddToBuffer ':'
	AddToBuffer 09h						; Tab
	ret
	
AddOffsetCS ENDP

; Deal with segment change prefix
SegmentChangePrefix PROC

	GetBits bl, 11100111b, dl			; Middle bits are sreg
	
	CMP_JE bl, 00100110b, SegmentChangePrefix__segmentChange
	jne SegmentChangePrefix__returnSegmentChange
	
  	SegmentChangePrefix__segmentChange:
 
	GetBitsSHR csreg, 00011000b, 3, dl	; Middle bits are sreg
	mov sregPreFlag, 1					; Set flag that we have segment change
	call GetNextByte					; Get next byte
	mov dl, dh							; Put it in dl
	
  	SegmentChangePrefix__returnSegmentChange:
	ret
	
SegmentChangePrefix ENDP

; Add segment prefix change if needed
AddSegmentChange PROC

	CMP_JE sregPreFlag, 0, AddSegmentChange__noSegmentChange
	
	mov bl, csreg
	mov bh, sreg						; Save sreg value for later restoring
	mov sreg, bl						; Bassically say that sreg = csreg
	call FindSreg
	AddToBuffer ':'
	mov sreg, bh						; Restore old sreg value
	
  	AddSegmentChange__noSegmentChange:
	ret
	
AddSegmentChange ENDP

; Collect Bytes
CollectBytes PROC
	
	push si
	
	mov si, workingByte					; Make current working byte as index
	mov [bytes + si], dh				; Assign the byte

	pop si
	
	inc workingByte						; Increase working byte
	ret
	
CollectBytes ENDP

; Print Bytes and the end of line
PrintBytes PROC
	
	push si
	push cx
	
	mov ax, [bytesToWrite]
	sub ax, oldBytesAdded				; Count how many bytes were added in current line
	
	mov cx, 8							; How much MAX tabs can be added
	sub al, 10							; Get rid of 10 bytes (one line length is always >= 10)
	
	PrintBytes__loopToCountTabs:
	
	sub al, 4							; Hard to explain, needs a paper work
	cmp al, 0F0h						; Check if not below zero (it can be -4 = FC as min)
	ja PrintBytes__addTabs				; Jumping out of the loop CX will hold the number of tabs value
	
	loop PrintBytes__loopToCountTabs	; Loop
		

  	PrintBytes__addTabs:
	AddToBuffer 09h						; Add needed tabs
	loop PrintBytes__addTabs
	
	xor si, si
	mov cx, workingByte					; How many bytes to add
	
  	PrintBytes__printBytes:
	mov dh, [bytes + si]
	mov bh, 0							; Using bl as a temp flag to say that we don't need 0 before A-F
	call AddBin2Hex						; Add DH to the buffer
	AddToBuffer ' '
	inc si								; Jump to next byte
	
	loop PrintBytes__printBytes			; Loop

	pop cx
	pop si
	mov workingByte, 1					; Reset working byte
	ret
	
PrintBytes ENDP

; Count the jump location
CountJump PROC

	call GetNextByte
	xchg dl, dh						; Put that byte in dl
	mov ax, [offsetCS]
	
	GetBits	bl, 10000000b, dl		; Get first bit to check if negative offset
	CMP_JE bl, 10000000b, CountJump__negativeOffset
	
	mov dh, 00h						; Positive - extend with 00h
	jmp CountJump__continueCountJump

  	CountJump__negativeOffset:
	mov dh, 0FFh					; Negative - extend with 0FFh
	
  	CountJump__continueCountJump:
	inc al							; Add 1 byte more because our offsetCS at this point is not exactly correct. (It increases 1B at the end of loop)
	add ax, dx						; Add the jump offset
	
	mov bh, 0						; Using bl as a temp flag to say that we don't need 0 before A-F
	mov dh, ah
	call AddBin2Hex					; Add DH to the buffer
	
	mov bh, 0						; Using bl as a temp flag to say that we don't need 0 before A-F
	mov dh, al
	call AddBin2Hex					; Add DH to the buffer
	
	ret

CountJump ENDP
