PROC DisassemblyAll
	call DealWith1B
	CMP_JE recFlag, 1, DisassemblyAll__continue
  
	call DealWith2B
	CMP_JE recFlag, 1, DisassemblyAll__continue
	
	call DealWith2_3B
	CMP_JE recFlag, 1, DisassemblyAll__continue
  
	call DealWith2_4B
	CMP_JE recFlag, 1, DisassemblyAll__continue
		
	call DealWith3_6B
	CMP_JE recFlag, 1, DisassemblyAll__continue
	
	AddTheString unknownC			
	
  DisassemblyAll__continue:
	mov recflag, 0
	mov sregPreFlag, 0
	ret

DisassemblyAll ENDP

PROC DealWith1B

	GetBits bl, 11111000b, dl
	CMP_JE  bl, bPOP2, handlePOP2
	CMP_JE  bl, bDEC2, handleDEC2
	
	GetBits bl, 11100111b, dl
	CMP_JE bl, bPOP3, handlePOP3

	jmp DealWith1B__return

	handlePOP2:
	AddTheString sPOP
	jmp DealWith1B__reg

	handleDEC2:
	AddTheString sDEC
	jmp DealWith1B__reg

	handlePOP3:
	AddTheString sPOP
	jmp DealWith1B__sreg
	
  DealWith1B__reg:
	mov w, 1						; Reg always word here
	GetBits reg, 00000111b, dl
	call FindReg
	jmp DealWith1B__found
	
  DealWith1B__sreg:
	GetBitsSHR sreg, 00011000b, 3, dl
	call FindSreg
	jmp DealWith1B__found

  DealWith1B__found:	
	mov recFlag, 1

  DealWith1B__return:
	ret

DealWith1B ENDP

PROC DealWith2B
	CMP_JE dl, bLOOP, 	handleLOOP
	CMP_JE dl, bLOOPE, 	handleLOOPE
	CMP_JE dl, bLOOPNE, handleLOOPNE
			
	jmp DealWith2B__return

	handleLOOP:
	AddTheString sLOOP
	jmp DealWith2B__countJump

	handleLOOPE:
	AddTheString sLOOPE
	jmp DealWith2B__countJump

	handleLOOPNE:
	AddTheString sLOOPNE
	jmp DealWith2B__countJump
	
  DealWith2B__countJump:
	call CountJump
	jmp DealWith2B__found
	
  DealWith2B__found:
	mov recFlag, 1
	
  DealWith2B__return:
	ret
	
DealWith2B ENDP

PROC DealWith2_3B

	GetBits bl, 11111110b, dl	
	CMP_JE bl, bAND3, handleAND3
	
	GetBits bl, 11110000b, dl
	CMP_JE bl, bMOV3, handleMOV3
	
	jmp DealWith2_3B__return

	handleAND3:
	AddTheString sAND
	jmp DealWith2_3B__analize2_3B1

	handleMOV3:
	AddTheString sMOV
	jmp DealWith2_3B__analize2_3B2
	
  DealWith2_3B__analize2_3B1:
	GetBits w, 00000001b, dl
	mov reg, 000b
	jmp DealWith2_3B__findOperand
	
  DealWith2_3B__analize2_3B2:
	GetBitsSHR w 00001000b, 3, dl
	GetBits reg, 00000111b, dl
	jmp DealWith2_3B__findOperand

	
  DealWith2_3B__findOperand:
	call FindReg
	AddTheString comma
	
	CMP_JE w, 0, DealWith2_3B__oneOperand

	call AddNext2B
	jmp DealWith2_3B__found
	
  DealWith2_3B__oneOperand:
	call AddNext1B
	
  DealWith2_3B__found:
	mov recFlag, 1
	
  DealWith2_3B__return:
	ret
	
DealWith2_3B ENDP

PROC DealWith2_4B
	
	mov bl, dl
	or bl, 00000001b
		
	CMP_JE bl, 0FFh, 	DealWith2_4B__flagFF
	CMP_JE dl, bPOP1, handlePOP1
	
	jmp DealWith2_4B__check2_4Bpart2
	
  DealWith2_4B__flagFF:
	mov bh, 1
	jmp DealWith2_4B__getNextByte
	
  handlePOP1:
	AddTheString sPOP
	mov bh, 2
	jmp DealWith2_4B__getNextByte	
	
  DealWith2_4B__continue2_4B2:
	mov bh, 3
	jmp DealWith2_4B__getNextByte
	
  DealWith2_4B__getNextByte:
	call GetNextByte
		
	GetBitsSHR bl, 		00111000b, 3, 	dh
	GetBitsSHR md, 		11000000b, 6, 	dh
	GetBitsSHR reg, 	00111000b, 3,		dh
	GetBitsSHR sreg, 	00011000b, 3, 	dh
	GetBitsSHR d, 		00000010b, 1, 	dl
	GetBits 	rm, 		00000111b, 	 		dh
	GetBits	 	w, 			00000001b, 	 		dl
	
	CMP_JE bh, 1, 	DealWith2_4B__checkFF
	CMP_JE bh, 2, 	DealWith2_4B__addRM2_4B
	CMP_JE bh, 3, 	DealWith2_4B__addRegAndRM
	
	DealWith2_4B__addRM2_4B:
	call FindRM
	jmp DealWith2_4B__found
		
	DealWith2_4B__addRegAndRM:	
	CMP d, 1
	JE DealWith2_4B__memToReg
	
	; d was 0 meaning r/m <- reg
	call FindRM
	AddTheString comma
	call FindReg
	jmp DealWith2_4B__found
	
	DealWith2_4B__memToReg:
	; d was 1 meaning reg <- r/m
	call FindReg
	AddTheString comma
	call FindRM
	jmp DealWith2_4B__found
	
  DealWith2_4B__checkFF:
	CMP_JE bl, bDEC1, 	handleDEC1
	
	jmp DealWith2_4B__check2_4Bpart2

	handleDEC1:
	AddTheString sDEC
	jmp DealWith2_4B__addRM2_4B

  DealWith2_4B__check2_4Bpart2:
	GetBits bl, 11111100b, dl
	
	CMP_JE dl, bLEA, 		handleLEA
	CMP_JE dl, bLDS, 		handleLDS
	CMP_JE bl, bAND1, 	handleAND1
	
	jmp DealWith2_4B__return

	handleLDS:
	AddTheString sLDS
	or dl, 00000010b
	jmp DealWith2_4B__continue2_4B2

	handleLEA:
	AddTheString sLEA
	or dl, 00000010b
	jmp DealWith2_4B__continue2_4B2

	handleAND1:
	AddTheString sAND
	jmp DealWith2_4B__continue2_4B2

 	DealWith2_4B__found:
	mov recFlag, 1

  DealWith2_4B__return:
	ret

DealWith2_4B ENDP

PROC DealWith3_6B

	GetBits bl, 11111100b, dl
	CMP_JE bl, 80h, DealWith3_6B__analize
		
	jmp DealWith3_6B__return
	
  DealWith3_6B__analize:
	GetBitsSHR s, 00000010b, 1, dl
	mov bh, 1
	call GetNextByte
	GetBitsSHR bl, 00111000b, 3, dh	; Get extended code bits
	
	CMP_JE bh, 1, DealWith3_6B__check
	CMP_JE bh, 2, DealWith3_6B__addOperands
	
  DealWith3_6B__check:
	CMP_JE bl, bAND2, handleAND2
	jmp DealWith3_6B__return		
	
	handleAND2:
	AddTheString sAND
	jmp DealWith3_6B__addOperands
	
  DealWith3_6B__addOperands:
	GetBitsSHR md,  11000000b, 6, 	dh
	GetBits rm, 		00000111b, 			dh
	GetBits w,  		00000001b, 			dl
	
	call FindRM
	AddTheString comma
	
	CMP_JE w, 0, 	DealWith3_6B__1Boperand
	CMP_JE s, 1, 	DealWith3_6B__1Boperand
					
	call AddNext2B
	jmp DealWith3_6B__found
	
  DealWith3_6B__1Boperand:
	call AddNext1B
	
  DealWith3_6B__found:
	mov recFlag, 1
  
  DealWith3_6B__return:
	ret
	
DealWith3_6B ENDP
