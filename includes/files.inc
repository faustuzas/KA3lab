PROC ReadFirstParameter

	mov	bx, 0081h																			; Parameters are written in ES starting 129 (or 81h) byte

	mov ax, es:[bx]																		; Deal with no parameters first.
	CMP_JE al, 0Dh, ReadFirstParameter__showHelp			; If end symbol, we need to show help message

	inc bx
	xor si, si           															; Index for first reading parameter		
	
 	ReadFirstParameter__loop:
		mov	ax, es:[bx]																	; Write to ax first two bytes
		
		CMP_JE al, 0Dh,   ReadFirstParameter__showHelp	; If end symbol, we need to show help message (no second parameter)
		CMP_JE ax, 3F2Fh, ReadFirstParameter__showHelp	; Maybe "/?" - 3F = '?'; 2F = '/'
		CMP_JE al, ' ',   ReadFirstParameter__return 		; If space we need to bytesRead second parameter      		

		mov [rFileName + si], al    										; If didn't found "/?" or "space" we are still reading first parameter

		inc si                 													; Increase index
		inc	bx																					; Increase pointer to next symbol
	jmp	ReadFirstParameter__loop											; Loop
	
  ReadFirstParameter__return:
	mov [rFileName + si], 0    	 											; File name must end with zero
	ret
	
  ReadFirstParameter__showHelp:
	printString helpMsg
	call TheEnd

ReadFirstParameter ENDP

PROC ReadSecondParameter

  xor si, si              													; Reset index
  inc	bx                  													; Jump the space
	
  ReadSecondParameter__loop:
		mov	ax, es:[bx]																	; Write to ax first two bytes

		CMP_JE al, 0Dh,   ReadSecondParameter__return		; If end symbol, we finished reading second parameter
		CMP_JE al, ' ',   ReadSecondParameter__showHelp	; If space that means that second parameter is incorrect 

    mov [wFileName + si], al  											; If didn't found end symbol or "space" we are still reading second parameter

    inc si              														; Increase index
		inc	bx																					; Increase pointer to next symbols
	jmp	ReadSecondParameter__loop											; Loop
	
  ReadSecondParameter__return:
	mov [wFileName + si], 0     											; File name must end with zero
	ret
	
  ReadSecondParameter__showHelp:
	printString helpMsg
	call TheEnd	

ReadSecondParameter ENDP

PROC OpenReadFile

	mov	ah, 3Dh									; 3Dh open file 21h int function number
	mov	al, 00									; 00 - Open file for reading
	mov	dx, offset rFileName		; Filename location
	int	21h											; Open file
	jc	OpenReadFile__error			; Error? if CF
	mov	rFile, ax								; Save hadle
	
	ret
	
  OpenReadFile__error:
	PrintString errorOpenRFile
	call TheEnd
	
OpenReadFile ENDP

PROC OpenWriteFile

	mov	ah, 3Ch								; 3Ch rewrite file 21h int function number
	mov	cx, 0									; File attributes
	mov	dx, offset wFileName 	; Filename location
	int	21h										; Rewrite file
	jc	OpenWriteFile__error	; Error? if CF
	mov	wFile, ax							; Save hadle
	
	ret
	
  OpenWriteFile__error:
	PrintString errorOpenWFile
	call CloseReadFile				; We will never return here because closing bytesRead file is the last operation
	
OpenWriteFile ENDP

PROC ReadFile
							; CX - how many bytes to bytesRead is given before call
							; DX - place where to bytesRead information is given before call
							; To AX will be returned how many bytes were bytesRead

	push bx
	
	mov	ah, 3Fh						; 3Fh bytesRead from file 21h int function number
	mov	bx, rFile					; BX - handle
	int	21h								; Read from file
	jc	ReadFile__error		; Error? if CF

  ReadFile__return:
	pop bx
	ret

  ReadFile__error:
	PrintString errorReadFile
	mov ax, 0							; Check in AX that no bytes were bytesRead
	jmp	ReadFile__return
	
ReadFile ENDP

PROC WriteFile
								; CX - how many bytes we need to write
								; AX - how many bytes were written

	push	dx

	mov	ah, 40h						; 40h write to file 21h int function number
	lea dx, wBuf					; Place from which we write to file	
	mov	bx, wFile					; BX - handle
	int	21h								; Write to file
	jc	WriteFile__error	; Error? if CF
	cmp	cx, ax						; Not all information written?
	jne	WriteFile__notAllInfoWritten

  WriteFile__return:
	pop	dx
	ret

  WriteFile__notAllInfoWritten:
	PrintString errorWriteFile
	jmp	WriteFile__return
	
  WriteFile__error:
	PrintString errorWriteFile
	mov	ax, 0							; No information was written
	jmp	WriteFile__return
	
WriteFile ENDP

PROC CloseWriteFile

	mov	ah, 3Eh									; 3Eh close file 21h int function number
	mov	bx, wFile								; To BX write the handle
	int	21h											; Close file
	jc	CloseWriteFile__error		; Error? if CF
	
 	CloseWriteFile__return:
	ret
	
 	CloseWriteFile__error:
	PrintString errorCloseWFile
	jmp	CloseWriteFile__return
	
CloseWriteFile ENDP

PROC CloseReadFile

	mov	ah, 3Eh								; 3Eh close file 21h int function number
	mov	bx, rFile							; To BX write the handle
	int	21h										; Close file
	jc	CloseReadFile__error	; Error? if CF
	
	call TheEnd								; Closing the reading file is the last operation			
	
  CloseReadFile__error:
	PrintString errorCloseRFile
	call TheEnd
	
CloseReadFile ENDP
