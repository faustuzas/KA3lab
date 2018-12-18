
; Only for testing commands

.MODEL SMALL
	
BSeg SEGMENT

	ORG	100h
	ASSUME ds:BSeg, cs:BSeg, ss:BSeg
	
Start:
	pop es						; pop sreg
	pop dx						; pop reg
	
	dec bx						; dec reg

	lea dx, myTest
	lea dx, [myTest+di]
		
	loope Start
	
	loop Start
	
	loopne Start

	and al, 0BAh				; and accum. - immed. op.

	and al, myTest				; and accum. - immed. op.
	
	and myTest, ah				; and accum. - immed. op.

	and dx, bx				; and accum. - immed. op.

	pop [bx]					; pop stack -> r/m
	lds dx, opa

	dec	myTest					; dec r/m
	lds dx, [opa+di]

	myTest		DB 	10h
	opa			DD 	10h

BSeg ENDS

END	Start		