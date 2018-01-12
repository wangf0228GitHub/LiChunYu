;*****************************************************************
	LIST P=PIC18F24K22,R=DEC
	#include <P18F24K22.INC>
	#include <Define.INC>
;*****************************************************************

extern ISR_EXIT
;-------
ISR_CCP:
	BCF	PIR1,CCP1IF		;
	BCF	T3CON,TMR3ON		;
	CLRF	TMR3L,A			;
	CLRF	TMR3H,A			;
	BSF	T3CON,TMR3ON		;

	BSF	f20_rxok,A		;

	BTFSS	f20_ONcal,A		;
	BRA	HD_BZ			;

	MOVF	CCPR1H,W,A		;
	MOVWF	TEMP0,A			;
	MOVF	CCPR1L,W,A		;
	MOVWF	TEMP1,A			;

	BSF	f21_over1,A		;

	MOVLW	0xE0			;0xE2
	SUBWF	TEMP1,F,A		;
	MOVLW	0x03			;
	SUBWFB	TEMP0,F,A		;994

	BNC	ISR_EXIT		;

;	MOVLW	0xFF			;
;	MOVWF	RAM_A4,BANKED		;
;adr_095C:
;	INCF	RAM_A4,F,BANKED		;如1025-994=32，
;	MOVLW	0x40			;994+30,也是这个效果，总和不变。
;	SUBWF	TEMP1,F,A		;
;	MOVLW	0x00			;
;	SUBWFB	TEMP0,F,A		;
;	BC	adr_095C		;

	RLCF	TEMP1,F,A		;
	RLCF	TEMP0,F,A		;
	RLCF	TEMP1,F,A		;
	RLCF	TEMP0,W,A		;
;;	MOVF	TEMP0,W,A		;
	MOVWF	RAM_A4,BANKED		;

;	BRA	ISR_EXIT		;
;
HD_BZ:
	BSF	f20_ONcal,A		;
	BCF	f21_over1,A		;
;2ms check
	BCF	T1CON,TMR1ON		;
	MOVLW	0x2F			;0xCB
	MOVWF	TMR1L,A			;
	MOVLW	0xF8			;0xF7
	MOVWF	TMR1H,A			;
	BSF	T1CON,TMR1ON		;

;	BCF	PIE1,TMR1IF,A		;ADD
;	BSF	PIE1,TMR1IE,A		;ADD
	BRA	ISR_EXIT		;