
;*****************************************************
;***************** r/m  ******************************
;*****************************************************

	rmBxSi 		EQU 	000b
	rmBxDi		EQU 	001b
	rmBpSi		EQU 	010b
	rmBpDi		EQU 	011b
	rmSi 		EQU 	100b
	rmDi 		EQU 	101b
	rmTiesBp	EQU 	110b
	rmBx		EQU 	111b
	
	;----------------------- r/m strings
	
	sRmBxSi		DB		'[bx+si', 0
	sRmBxDi		DB		'[bx+di', 0
	sRmBpSi		DB		'[bp+si', 0
	sRmBpDi		DB		'[bp+di', 0
	sRmSi		DB		'[si', 0
	sRmDi		DB		'[di', 0
	sRmBp		DB		'[bp', 0
	sRmBx		DB		'[bx', 0
	
	;----------------------- reg strings
	
	sReg1BDB	DB		'al'
				DB		'cl'
				DB		'dl'
				DB		'bl'
				DB		'ah'
				DB		'ch'
				DB		'dh'
				DB		'bh'
				
	sReg2BDB	DB		'ax'
				DB		'cx'
				DB		'dx'
				DB		'bx'
				DB		'sp'
				DB		'bp'
				DB		'si'
				DB		'di'


;*****************************************************
;*********** Segment registers ***********************
;*****************************************************
	
	regES 		EQU 	00b
	regCS 		EQU 	01b
	regSS	 	EQU 	10b
	regDS		EQU 	11b
	
	;----------------------- sreg strings
				
	SRegDB		DB		'es'
				DB		'cs'
				DB		'ss'
				DB		'ds'
	
	