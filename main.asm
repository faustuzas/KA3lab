INCLUDE includes/macros.inc
	
.MODEL SMALL
	
	rBufSize	EQU 128
	wBufSize	EQU 4096

.STACK 100h

.DATA    

	INCLUDE includes/data.inc
	INCLUDE includes/opcodes.inc
	INCLUDE includes/regm.inc
	
.CODE

  main:
	mov	ax, @data					
	mov	ds, ax
	
	call ReadFirstParameter
	call ReadSecondParameter

	call OpenReadFile
	call OpenWriteFile

  	main__readFile:
  	mov	cx, rBufSize				 			; CX - bytes we need to bytesRead from file
	lea	dx, rBuf					 			; Place where to bytesRead information
	call ReadFile					 			; Read from file
	mov bytesRead, ax					 		; How many bytes were bytesRead
	CMP_JE bytesRead, 0, main__closeFiles 		; 0? We're done reading
	
	mov bytesToWrite, 0							; How many bytes we need to write counter

	mov	cx, [bytesRead]							; CX - the loop counter
	mov	si, offset rBuf							; SI - source index
	mov	di, offset wBuf							; DI - destination index
	
  	main__loop:
  
	mov ax, [bytesToWrite]						; oldBytesAdded - needed to count how many bytes was added in one line 
	mov oldBytesAdded, ax						; and acoording to that add as needed tab's before machine code
	
  	call AddOffsetCS							; Just add the offset from CS first
	
	mov	dl, [si]								; First byte in DL
	mov bytes[0], dl							; Save first byte
	
	call SegmentChangePrefix					; Deal with segment change prefix first
	
	call DisassemblyAll							; Call the 'core'
	call PrintBytes								; Add the machine code at the end
	AddToBuffer 0Ah								; Add new line
	
	inc	si										; Jump to next byte
	inc offsetCS								; Increase offset

	loop main__loop
	
	mov	cx, bytesToWrite						; CX - bytes to write
	call WriteFile								; Write to file
	CMP_JE bytesRead, rBufSize, main__readFile	; If the full buffer was bytesRead then continue reading

  	main__closeFiles:
	call CloseWriteFile
	call CloseReadFile


	INCLUDE includes/files.inc
	INCLUDE includes/tools.inc
	INCLUDE includes/core.inc

	PROC TheEnd

		mov	ah, 4Ch		
		mov	al, 0				
		int	21h						
		
	TheEnd ENDP

  END main
