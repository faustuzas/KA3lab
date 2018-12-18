; Messages

	helpMsg			DB 'HELP: main.exe test.com res.asm$'
				
	errorOpenRFile	DB 10, 13, 'ERROR: Error in opening source file.', 			 10, 13, '$'
	errorOpenWFile	DB 10, 13, 'ERROR: Error in opening destination file.', 	 10, 13, '$'
	errorCloseRFile	DB 10, 13, 'ERROR: Error in closing source file.', 			 10, 13, '$'
	errorCloseWFile	DB 10, 13, 'ERROR: Error in closing destination file.', 	 10, 13, '$'
	
	errorReadFile	DB 10, 13, 'ERROR: Error when reading from source file.', 	 10, 13, '$'
	errorWriteFile 	DB 10, 13, 'ERROR: Error when writing to destination file.', 10, 13, '$'
	
	
; Files

	rFileName  		DB 64 DUP (?)           	; Read file name
	wFileName		DB 64 DUP (?)				; Write file name
	rBuf			DB rBufSize DUP (?)			; Read buffer
	wBuf			DB wBufSize DUP (?)			; Write buffer
	rFile			DW ?						; Read file handle
	wFile			DW ?						; Write file handle
	bytesRead		DW ?						; How many bytes were bytesRead
	bytesToWrite	DW  0						; Counter how many bytes to write
	
	
; Addressing byte 
	
	w           	DB  ?						; w bit
	d           	DB  ?						; d bit
	s				DB  ?						; s bit
	reg           	DB  ?						; reg bits
	sreg           	DB  ?						; sreg bits
	csreg			DB	?						; csreg change segment bits
	md           	DB  ?						; mod bits
	rm				DB  ?						; r/m bits
	
	
; Other data

	comma    		DB  ', ', 0					; Comma and space
	hexTable		DB 	'0123456789ABCDEF'		; Hex values
	eByte			DB 	0						; Extra byte, used than out of buffer to recognize command
	recFlag			DB  0						; Flag to keep track of recognized commands
	sregPreFlag		DB  0						; Flag to indicate do we have a segment change
	unknownC		DB	'Unknown Command', 0
	offsetCS		DW  0100h
	bytes			DB  6 DUP(0)				; Temp saving location
	workingByte		DW  1
	oldBytesAdded	DW 	0
	
	