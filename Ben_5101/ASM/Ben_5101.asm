;*****************************************************************
	LIST P=PIC18F24K22,R=DEC
	#include <P18F24K22.INC>
	#include <Define.INC>
;*****************************************************************
;-----------------------------------------------

;
;
;-----------------------------------------------
;-----------------------------------------------
;---------------------------------------
	ORG	0x000000		;
;---------------------------------------
	GOTO	START			;
;---------------------------------------
	ORG	0x000008		;
;---------------------------------------
	MOVFF	WREG,W_TMP		;
	MOVFF	STATUS,STATUS_TMP	;
	MOVFF	BSR,BSR_TMP		;
	GOTO	ISR			;
;---------------------------------------
	ORG	0x000018		;
;---------------------------------------
	MOVFF	WREG,W_TMP		;
	MOVFF	STATUS,STATUS_TMP	;
	MOVFF	BSR,BSR_TMP		;
	GOTO	ISR			;
;---------------------------------------
;---------------------------------------
;-------------------------------------------------
;---------------------------------------
ISR:
	BTFSS	PIE1,CCP1IE		;
	BRA	ISR_00			;
	BTFSS	PIR1,CCP1IF		;
	BRA	ISR_00			;
	BRA	ISR_CCP			;
;
ISR_00:
	BTFSS	PIE1,TMR1IE,A		;
	BRA	ISR_01			;
	BTFSS	PIR1,TMR1IF,A		;
	BRA	ISR_01			;
	BRA	ISR_TIME1		;
;
ISR_01:
	BTFSS	INTCON,RBIE,A		;
	BRA	ISR_02			;
	BTFSS	INTCON,RBIF,A		;
	BRA	ISR_02			;
	BRA	ISR_chg			;
;
ISR_02:
	BTFSS	PIE2,HLVDIE,A		;
	BRA	ISR_EXIT		;
	BTFSS	PIR2,HLVDIF,A		;
	BRA	ISR_EXIT		;
	BRA	ISR_VolLow		;
;

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

	RLCF	TEMP1,F,A		;?
	RLCF	TEMP0,F,A		;?
	RLCF	TEMP1,F,A		;?
	RLCF	TEMP0,W,A		;?
	MOVWF	RAM_A4,BANKED		;?

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
	BRA	ISR_EXIT		;
;-------
ISR_TIME1:

	BCF	PIE1,TMR1IE,A		;
	BCF	PIE1,TMR1IF,A		;

	BTFSS	f20_ONbuzh,A		;打开捕捉后才进行超时检测，
	BRA	ISR_EXIT		;
	BTFSS	f20_ONcal,A		;打开计算后，才进行检查，超时后关闭捕捉
	BRA	ISR_EXIT		;

	BCF	PIR1,CCP1IF		;
	CLRF	CCP1CON,A		;
	BSF	f20_rxok,A		;收到标志
	BSF	f21_over1,A		;超时后，外部查询标志

	BRA	ISR_EXIT		;
;-------
ISR_chg:
	MOVF	PORTB,W,A		;
	CLRF	IOCB,A			;
	BCF	INTCON,RBIF,A		;
	BCF	INTCON,RBIE,A		;
	BCF	INTCON,PEIE,A		;
	BCF	INTCON,GIE,A		;

	BSF	RAM_CB,5,BANKED		;?

	BRA	ISR_EXIT		;
;-------
ISR_VolLow:
	BSF	BAT_CL			;
	BCF	PIR2,HLVDIF		;
	BCF	PIE2,HLVDIE		;
	BCF	INTCON,PEIE,A		;
	BCF	INTCON,GIE,A		;

;-------


;--------------
ISR_EXIT:
	MOVFF	BSR_TMP,BSR		;
	MOVFF	STATUS_TMP,STATUS	;
	MOVFF	W_TMP,WREG		;

	RETFIE				;
;---------------------------------------



;---------------------------------------
START:
	CALL	INITIAL			;
;---------------------------------------
M_LOOP:
	GOTO	POW_CAR
	CLRWDT				;
	GOTO	POW_BAT			;
	CALL	CHK_SUPPLY		;
;
	BTFSS	f20_power,A		;
	GOTO	POW_BAT			;
	GOTO	POW_CAR			;

	GOTO	M_LOOP			;
;---------------------------------------

;---------------------------------------
POW_BAT:
	MOVLW	B'11110000'		;S4,S1,S2,S3
;	MOVLW	B'00110000'		;
	ANDWF	PORTB,W,A		;
	MOVWF	KEY_VALU,A		;
;	BSF	KEY_VALU,7,A		;
;	BSF	KEY_VALU,6,A		;
;
;	MOVLW	B'10100000'		;S1,S3
;	XORWF	KEY_VALU,W,A		;
;	BNZ	DOOR_1			;

;	BSF	BAT_CL			;
;	CALL	EEtoIIC			;
;DOOR_1:
;	MOVLW	B'11000000'		;S2,S3
;	XORWF	KEY_VALU,W,A		;
;	BNZ	DOOR_E			;

;	BSF	BAT_CL			;

;	CALL	IICtoEE			;
;DOOR_E:
;
	MOVLW	B'10010000'		;11000000

	XORWF	KEY_VALU,W,A		;7456
	BNZ	adr_110F		;

	MOVLW	.250			;
	CALL	sub_11AB		;还没有握手，不用发送复位信号
	BTFSS	STATUS,Z,A		;
	GOTO	adr_11A2_WaitReset		;

	MOVLW	0x94			;
	CALL	EE_WT_CHK_AddrBYW_FFor0		;
	CALL	LED_FLASH		;
adr_110C:
	BRA	adr_11A2_WaitReset		;
;
adr_110F:
	BCF	f21_flgS3,A		;

	MOVLW	0x94			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVF	TEMP_EE,F,A		;
	MOVLW	0x20			;
	BTFSS	STATUS,Z,A		;
	MOVLW	0x21			;

	BTFSS	KEY_VALU,5,A		;S2
	BRA	adr_1131_UnlockKeyProc		;

	MOVLW	0x23			;
	BTFSS	KEY_VALU,6,A		;S1---4
	BRA	adr_1131_UnlockKeyProc		;

	MOVLW	0x2B			;
	BTFSS	KEY_VALU,7,A		;S4
	BRA	adr_1131_UnlockKeyProc		;

	BTFSC	KEY_VALU,4,A		;S3---6
	GOTO	adr_11A2_WaitReset		;

	MOVLW	.13			;
	CALL	sub_11AB		;还没有握手，不用发送复位信号
	BTFSS	STATUS,Z,A		;
	GOTO	adr_11A2_WaitReset		;

	MOVLW	0x22			;
adr_1131_UnlockKeyProc:
	MOVWF	RAM_85,BANKED		;
	BSF	BAT_CL			;
	BSF	TST_RA6			;;;;;;

	CALL	sub_1AF5_CheckRomData		;
	MOVWF	YK_STA,A		;
	MOVWF	Temp_2,A		;

	MOVF	YK_STA,W,A		;
	BNZ	adr_1167		;

	MOVLW	0X00			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVWF	RAM_CD,BANKED		;

	ANDLW	0XC0			;
	BTFSC	STATUS,Z		;
	BSF	RAM_CA_4,BANKED		;

	BTFSC	RAM_CA_4,BANKED		;
	BRA	adr_114C		;

	MOVLW	0X43			;
	MOVWF	RAM_85,BANKED		;
adr_114C:
	CALL	sub_1680_Check55and5fdata_reC5		;
	MOVF	RAM_C5,W,BANKED		;
	MOVWF	X_P,A			;
	CALL	sub_1B48_Load8EEto93to9A_byX_P		;
adr_1154:
	MOVF	RAM_BF,F,BANKED		;
	BZ	adr_1161		;

	MOVLW	0X93			;
	CALL	sub_0C11_HashCalc_byWisAddr		;
	DECFSZ	RAM_BF,F,BANKED		;
	BRA	adr_1154		;
adr_1161:
	CALL	sub_11CF		;

	BTFSC	RAM_CD,2,BANKED		;
	BRA	WAIT_RF_N		;
adr_1167:
	CALL	sub_1CD1_RFTx_00_00_00_80_21_00___Len18		;RF SEND
	BRA	adr_116A		;
;------
;内部EE不正确时
WAIT_RF_N:

;------
adr_116A:
;PUBLIC
	CALL	CHECK_CHG		;

	BTFSC	f21_RST,A	;若线圈供电则复位		;
	GOTO	INSERT_RST_1		;

	MOVLW	.7			;
	CALL	DELAY_N_10MS		;

	CALL	CHECK_CHG		;
	BTFSC	f21_RST,A			;
	GOTO	INSERT_RST_1		;

	CALL	TX_IR_A_Tx_88_88_88_88_08______len18			;

	CALL	CHECK_CHG		;
	BTFSC	f21_RST,A			;
	GOTO	INSERT_RST_1		;

	MOVLW	0x2B			;判断开锁键值视为为0x2B
	XORWF	RAM_85,W,BANKED		;
	BZ	adr_1184		;

	CALL	CHK_VOL			;
adr_1184:
	BCF	BAT_CL			;

	BTFSC	RAM_CB,4,BANKED		;以下是按键长按的工作
	BRA	chk_SWandIN		;

	BTFSS	KEY_VALU,6,A		;S1----4
	BRA	adr_119F		;

	BTFSS	KEY_VALU,5,A		;S2
	BRA	adr_119F		;

	BTFSS	KEY_VALU,4,A		;S3---6
	BRA	adr_ttt			;

	BTFSC	KEY_VALU,7,A		;S4
	BRA	chk_SWandIN		;
adr_ttt:
	MOVLW	.30			;
	CALL	DUR_CHG			;
	BCF	f20_temp0,A		;
	BTFSC	f21_RST,A			;
	BSF	f20_temp0,A		;save

	BCF	f21_RST,A			;
	BTFSC	f21_swChg,A		;
	BSF	f21_RST,A			;

	BCF	f21_RST,A			;
	BTFSC	f20_temp0,A		;
	BSF	f21_RST,A			;recover

	BTFSC	f21_RST,A			;
	GOTO	INSERT_RST_1		;

	BTFSC	f21_swChg,A		;
	GOTO	chk_SWandIN		;

	MOVLW	.5			;修正RF IR间隔
	CALL	DELAY_N_10MS		;
	BSF	RAM_CB,4,BANKED		;

	MOVLW	0x22			;
	BTFSC	KEY_VALU,4,A		;
	MOVLW	0x29			;

	BRA	adr_1131_UnlockKeyProc		;

adr_119F:
	CALL	TX_IR_S			;
;-------
chk_SWandIN:
	BRA	TEST_PW			;
;-------
WAIT_CHG_0:
	CALL	CHECK_CHG		;
	BTFSC	f21_RST,A			;
	BRA	HAV_chg			;

	BTFSS	RAM_CB,5,BANKED		;
	BRA	WAIT_CHG_0		;

HAV_chg:
	BCF	f21_RST,A			;
	SLEEP
	RESET				;
;-------
adr_11A2_WaitReset:
	BRA	TEST_PW			;

	CALL	sub_11B7_KeyQuDou		;
	BTFSS	RAM_CB,5,BANKED		;
	BRA	adr_11A2_WaitReset		;

	SLEEP
	RESET				;
;-------
INSERT_RST_1:
	BCF	TST_RA6			;;;;;;

	GOTO	START			;
;-------
TEST_PW:
WAIT_B:
	CLRWDT				;

	BTFSS	PW_IN			;
	BRA	WAIT_B			;

	RESET				;
	GOTO	M_LOOP			;
;-------
;---------------------------------------
;---------------------------------------
DUR_CHG:
	MOVWF	RAM_C1,BANKED		;
DUR_CHG_LP:
	CALL	CHECK_CHG		;
	BNZ	DUR_CHG_E		;

	DECFSZ	RAM_C1,F,BANKED		;
	BRA	DUR_CHG_LP		;
DUR_CHG_E:
	RETURN				;
;---------------------------------------
CHECK_CHG:
	BCF	f21_RST,A			;
	CALL	CHK_SUPPLY		;
	BTFSC	f20_power,A		;
	BSF	f21_RST,A			;

	BCF	f21_swChg,A		;
	MOVLW	0xF0			;
	ANDWF	PORTB,W,A		;
	XORWF	KEY_VALU,W,A		;
	BZ	CHECK_CHG_E		;
;
	BSF	RAM_CB,4,BANKED		;
	BSF	f21_swChg,A		;

	MOVF	PORTB,W,A		;
	MOVLW	B'11110000'		;
	MOVWF	IOCB,A			;
	BCF	INTCON,RBIF,A		;
	BSF	INTCON,RBIE,A		;
	BSF	INTCON,PEIE,A		;
	BSF	INTCON,GIE,A		;
;	MOVLW	B'11001000'
;	MOVWF	INTCON,A		;
CHECK_CHG_E:
	RETURN				;
;---------------------------------------

;---------------------------------------
sub_11AB:
	MOVWF	RAM_C1,BANKED		;
adr_11AD:
	CALL	sub_11B7_KeyQuDou		;
	BNZ	adr_11B6		;

	DECFSZ	RAM_C1,F,BANKED		;
	BRA	adr_11AD		;
adr_11B6:
	RETURN				;
;---------------------------------------
sub_11B7_KeyQuDou:;若按键则开电平变化中断
	CALL	CHK_SUPPLY		;
	BTFSC	f20_power,A		;
	RESET				;

	MOVLW	0xF0			;
	ANDWF	PORTB,W,A		;
	XORWF	KEY_VALU,W,A		;
	BZ	adr_11CE		;
;
	BSF	RAM_CB,4,BANKED		;

	MOVF	PORTB,W,A		;
	MOVLW	B'11110000'		;
	MOVWF	IOCB,A			;
	BCF	INTCON,RBIF,A		;
	BSF	INTCON,RBIE,A		;
	BSF	INTCON,PEIE,A		;
	BSF	INTCON,GIE,A		;
;	MOVLW	B'11001000'
;	MOVWF	INTCON,A		;

;
adr_11CE:
	RETURN				;
;---------------------------------------
CHK_SUPPLY:
	BTFSC	PW_IN			;
	BSF	f20_power,A		;

	MOVLW	.2			;
	CALL	DELAY_N_10MS		;

	BTFSS	PW_IN			;
	BCF	f20_power,A		;

	RETURN
;---------------------------------------
DELAY_N_10MS:
	MOVWF	CNT2,A			;
	RETURN
RE_T1:
	CALL	INI_T1			;
	RETURN
DELAY_N10_LP:
	RETURN
	BTFSC	f20_rxok,A		;捕捉打开时才会用到
	BRA	DELAY_N10_END		;

	BTFSS	PIR1,TMR1IF,A		;
	BRA	DELAY_N10_LP		;
	DECFSZ	CNT2,F,A		;
	BRA	RE_T1			;

DELAY_N10_END:
	RETURN
;---------------------------------------
INI_T1:
	BCF	T1CON,TMR1ON		;
	MOVLW	0xEF			;
	MOVWF	TMR1L,A			;
	MOVLW	0xD8			;
	MOVWF	TMR1H,A			;
	BSF	T1CON,TMR1ON		;
	BCF	PIE1,TMR1IE,A		;
	BCF	PIR1,TMR1IF,A		;
	RETURN
;---------------------------------------


;---------------------------------------
LED_FLASH:
	BSF	LED			;
	MOVLW	.30			;
	CALL	DELAY_N_10MS		;

	BCF	LED			;
	MOVLW	.30			;
	CALL	DELAY_N_10MS		;

	BSF	LED			;
	MOVLW	.30			;
	CALL	DELAY_N_10MS		;
	BCF	LED			;
	MOVLW	.30			;
	CALL	DELAY_N_10MS		;

	RETURN
;---------------------------------------
sub_1297_whileLEDFlash:
adr_1297:
	CALL	LED_FLASH		;
	BRA	adr_1297		;
;---------------------------------------
;***************************************
;***************************************
;---------------------------------------
POW_CAR:
	CALL	sub_1AF5_CheckRomData		;
	BZ	adr_12C8_Tx2425FrameData		;写过I2c后的初始化
;
	MOVLW	0x4C			;
	XORWF	RAM_CA,W,BANKED		;
	BZ	adr_12C3		;

	MOVLW	0x0C	;死循环		;0C-F4
	XORWF	RAM_CA,W,BANKED		;
	BZ	adr_12B5		;

	MOVLW	0x21			;
	XORWF	RAM_CA,W,BANKED		;
	BZ	adr_12B2		;
;----
	GOTO	sub_1784		;no return
;----
adr_12B2:
	GOTO	sub_17B4		;NO return
;----
adr_12B5:
	CALL	sub_1944		;
	BNZ	adr_12B5		;

	MOVF	RAM_83,F,BANKED		;
	BNZ	adr_12B5		;

	MOVLW	0x4C			;
	MOVWF	X_P,A			;
	CALL	sub_1B07_I2C_addr9e_dataWandnotW		;
;----
adr_12C3:
	CALL	sub_1A8E		;
	BRA	adr_1315_IRTX_1028_1033SSID		;
;-------
adr_12C8_Tx2425FrameData:
	CALL	sub_1381_Get2425TxParam		;RAM_C4,RAM_C3,RAM_C2,获取
adr_12CB_Tx2425Frame:
	CALL	sub_1330_Get2425TxFrame		;读取序号，并处理
	
	
	;测试代码
	;CALL	sub_1357_Get27ACKFrame
	;BRA 	adr_12C8_Tx2425FrameData
	
	
	
	MOVLW	.8			;
	MOVWF	X_P,A			;
	;;;;CALL	TX_IR_B_10_data81_LenBYX_P_add1;10 25(24) 7f 1f 2f cb 61 90 8f

	MOVLW	.10			;
	CALL	sub_1C30_RxFrame_pBuf80		;
	BTFSC	f21_over1,A		;
	BRA	adr_12CB_Tx2425Frame		;
;
	MOVLW	0x26			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_12EF_Rx26Proc		;
	

	MOVLW	0x3E			;
	XORWF	RAM_81,W,BANKED		;
	BNZ	adr_12E8		;

	CALL	sub_1F03		;
adr_12E8:
	MOVLW	0x7A			;
	XORWF	RAM_81,W,BANKED		;
	BNZ	adr_12CB_Tx2425Frame		;

	GOTO	sub_1E12_ProcCom7A		;NO return
adr_12EF_Rx26Proc:
	MOVF	RAM_82,W,BANKED		;
	BZ	adr_12CB_Tx2425Frame

	CALL	sub_1357_Get27ACKFrame		;
	MOVLW	.9			;
	MOVWF	X_P,A			;
	;;;;CALL	TX_IR_B_10_data81_LenBYX_P_add1			;

	BTFSC	RAM_CA_4,BANKED		;
	BRA	adr_1312		;学习过后的操作

	BSF	LED			;
	MOVLW	.0			;
	CALL	sub_1C30_RxFrame_pBuf80		;
	BTFSC	f21_over1,A		;
	BRA	adr_132D		;

	MOVLW	0x39			;
	XORWF	RAM_81,W,BANKED		;
	BNZ	adr_132D		;

	CALL	sub_1E83		;学习过程
	BNZ	adr_132C_StudyWillFlash_OtherReset		;为0表示学习成功

adr_1312:
	CALL	sub_152E		;
	RESET
;-------
adr_1315_IRTX_1028_1033SSID:
	MOVLW	0x09			;sub_1663_Tx10_33_SSID-拆分
	MOVWF	X_P,A			;
	LFSR	FSR0,0xF2		;RAM_F2
	CALL	sub_1B57_ReadEEorI2C_addrBYX_P_len8		;
;
adr_1318:
	MOVLW	0x28			;
	MOVWF	RAM_81,BANKED		;
	MOVLW	.1			;
	MOVWF	X_P,A			;
	;;;;CALL	TX_IR_B_10_data81_LenBYX_P_add1 ;//10-28
	MOVLW	.10			;
	CALL	DELAY_N_10MS		;

;	CALL	sub_1663_Tx10_33_SSID		;
	MOVLW	0x33			;
	MOVWF	RAM_81,BANKED		;10-33-I2C09~0D

	MOVF	RAM_F2,W,BANKED		;
	MOVWF	RAM_82,BANKED		;
	MOVF	RAM_F3,W,BANKED		;
	MOVWF	RAM_83,BANKED		;
	MOVF	RAM_F4,W,BANKED		;
	MOVWF	RAM_84,BANKED		;
	MOVF	RAM_F5,W,BANKED		;
	MOVWF	RAM_85,BANKED		;

	MOVLW	.5			;
	MOVWF	X_P,A			;
	;;;;CALL	TX_IR_B_10_data81_LenBYX_P_add1			;
;
	MOVLW	.10			;
	CALL	DELAY_N_10MS		;
	BRA	adr_1318		;adr_1315_IRTX_1028_1033SSID
;-------
adr_132C_StudyWillFlash_OtherReset:
	BTFSS	RAM_CA_4,BANKED		;
	RESET				;
;-------
adr_132D:
	GOTO	sub_1297_whileLEDFlash		;
;---------------------------------------
sub_1330_Get2425TxFrame:

	MOVLW	0X09			;
	MOVWF	X_P,A			;
	LFSR	FSR0,0X85		;RAM_85
	CALL	sub_1B57_ReadEEorI2C_addrBYX_P_len8		;加载序号，仅仅序号有用，其余的被覆盖

	MOVF	RAM_C2,W,BANKED		;
	MOVWF	RAM_82,BANKED		;
	MOVWF	RAM_8A,BANKED		;备用

	MOVF	RAM_C3,W,BANKED		;
	MOVWF	RAM_83,BANKED		;
	MOVWF	RAM_8B,BANKED		;备用

	MOVF	RAM_C4,W,BANKED		;
	MOVWF	RAM_84,BANKED		;
	MOVWF	RAM_8C,BANKED		;备用

	MOVLW	0x00			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVLW	0xC0			;
	ANDWF	TEMP_EE,W,A		;
	IORWF	RAM_84,W,BANKED		;
	MOVWF	RAM_84,BANKED		;
	MOVWF	RAM_8C,BANKED		;

	MOVLW	0x24			;
	BTFSS	RAM_CA,4,BANKED		;
	MOVLW	0x25			;
	MOVWF	RAM_81,BANKED		;
	MOVWF	RAM_8D,BANKED		;备用

	RETURN				;
;---------------------------------------
sub_1357_Get27ACKFrame:
	MOVLW	0x27			;
	MOVWF	RAM_81,BANKED		;
	LFSR	FSR0,0x89		;RAM_89
	MOVLW	0x08			;
	MOVWF	ADDRESS,A		;
	MOVLW	.8			;
	MOVWF	Temp_0,A		;
adr_135D:
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	XORWF	POSTDEC0,F,A		;
	DECF	ADDRESS,F,A		;
	DECFSZ	Temp_0,F,A		;
	BRA	adr_135D		;

	MOVLW	.4			;
	MOVWF	Temp_0,A		;
	LFSR	FSR0,0x8D		;RAM_8D，SUB_1330预留
	LFSR	FSR1,0x89		;RAM_89
adr_1369:
	MOVF	POSTDEC0,W,A		;
	XORWF	POSTDEC1,F,A		;
	DECFSZ	Temp_0,F,A		;
	BRA	adr_1369		;
;
	BTFSC	RAM_D2,7,BANKED		;
	BRA	adr_1528		;

	MOVLW	0x03			;
	ANDWF	RAM_C2,W,BANKED		;
	BNZ	adr_1525		;

	CALL	sub_13DE_block55or5f_verisFF_00		;
adr_1525:
	CALL	sub_1442_USEDEC		;
adr_1528:
	MOVLW	0X82			;
	CALL	sub_0C11_HashCalc_byWisAddr		;
;
	MOVLW	.8			;
	MOVWF	Temp_0,A		;
	LFSR	FSR0,0x9A		;RAM_9A
	LFSR	FSR1,0x89		;RAM_89
adr_1377:
	MOVF	POSTDEC0,W,A		;
	XORWF	POSTDEC1,F,A		;
	DECFSZ	Temp_0,F,A		;
	BRA	adr_1377		;

	RETURN				;
;---------------------------------------


	RETURN				;
;---------------------------------------

	RETURN				;
;---------------------------------------

;---------------------------------------
sub_1F03:

	RETURN				;
;---------------------------------------
sub_1E12_ProcCom7A:
	MOVLW	B'00001010'		;
	MOVWF	CCP1CON,A		;
	MOVLW	high(TE_500)		;
	MOVWF	CCPR1H,A		;
	MOVLW	low(TE_500)		;
	MOVWF	CCPR1L,A		;
	CALL	ON_CCP			;
;
	MOVF	RAM_82,W,BANKED		;
	MOVWF	A_P,A			;

	MOVF	A_P,W,A			;'00'
	BZ	adr_1E75_TX00_____		;IR_B

	BCF	f20_power,A		;
	DECF	A_P,F,A			;'01'
	BZ	adr_1E75_TX00_____		;IR_A

	BSF	RF_EN			;RF使能
	DECF	A_P,F,A			;'02'
	BZ	adr_1E75_TX00_____		;RF ON

	DECF	A_P,F,A			;'03'
	BZ	adr_1E73		;nothing wait

	MOVF	PORTC,W,A		;
	ANDLW	B'11000111'		;
	IORLW	B'00101000'		;
	MOVWF	PORTC,A			;
	DECF	A_P,F,A			;'04'
	BZ	adr_1E73		;nothing wait

	BCF	RF_EN			;RF关闭
	BSF	LED			;
	DECF	A_P,F,A			;'05'
	BZ	adr_1E73		;nothing wait

	BCF	LED			;
	DECF	A_P,F,A			;'06'
	BNZ	adr_1E44		;

	CALL	sub_1B3C_RAM83_88_WritetoEE98_9D		;
	MOVLW	0x01			;
	MOVWF	A_P,A			;
adr_1E44:
	BSF	f20_power,A		;
	BSF	f20_RDee,A		;
	MOVLW	0x98			;
	MOVWF	X_P,A			;
	DECF	A_P,F,A			;'07'
	BZ	adr_1E50		;

	BCF	f20_RDee,A		;

	MOVLW	0x9C			;
	MOVWF	X_P,A			;
	DECF	A_P,F,A			;'08'
	BNZ	adr_1E73		;
adr_1E50:
	LFSR	FSR0,0x83		;RAM_83
	CALL	sub_1B57_ReadEEorI2C_addrBYX_P_len8		;

	BTFSS	RAM_82,3,BANKED		;
	BRA	adr_1E65		;

	MOVLW	0x57			;ID=74
	MOVWF	RAM_83,BANKED		;
	CALL	CHK_VOL			;ADC
	MOVLW	0x00			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVWF	RAM_87,BANKED		;
adr_1E65:
	MOVLW	0x7B			;
	MOVWF	RAM_81,BANKED		;
	MOVLW	.10			;
	CALL	DELAY_N_10MS		;
	MOVLW	.8			;
	MOVWF	X_P,A			;
	CALL	TX_IR_B_10_data81_LenBYX_P_add1			;
;-------
adr_1E73:
	CLRWDT				;
WAIT_C:
	CLRWDT				;
	BTFSC	PW_IN			;
	BRA	WAIT_C			;
	RESET				;

	BRA	adr_1E73		;
;-------
adr_1E75_TX00_____:
	MOVLW	0x00			;
	CALL	sub_1D43_TxByte_byW		;
	BRA	adr_1E75_TX00_____		;
;-------
;---------------------------------------
sub_1E83:
	BSF	BAT_CL			;battery ON
	MOVLW	.1			;RAM_93-->RAM_8B
	CALL	sub_1ED3_LoadPSWto8b_92_formatBYW		;从EE的01地址开始读取8个数据到:RAM_8B,RAM_8C...RAM_92
	CALL	sub_1EE6		;read RF ID
	CALL	sub_1BE7_93to9A_FanXuPaiLie		;逆序

	MOVLW	.3			;
	MOVWF	Temp_0,A		;
	LFSR	FSR0,0x93		;RAM_93
	LFSR	FSR1,0x90		;RAM_90
adr_1E92:
	MOVF	POSTINC0,W		;
	XORWF	POSTINC1,F,A		;

	DECFSZ	Temp_0,F,A		;
	BRA	adr_1E92		;
;
	MOVLW	0x8B			;
	CALL	sub_0C11_HashCalc_byWisAddr		;

	MOVLW	.8			;
	MOVWF	Temp_0,A		;
	LFSR	FSR0,0x8B		;RAM_8B
	LFSR	FSR1,0x82		;RAM_82
adr_1EA2:
	MOVF	POSTINC0,W		;
	XORWF	POSTINC1,W		;
	BNZ	adr_1ED0		;

	DECFSZ	Temp_0,F,A		;
	BRA	adr_1EA2		;

	MOVLW	.2			;
	CALL	sub_1ED3_LoadPSWto8b_92_formatBYW		;
	CALL	sub_1EE6	;read RF ID

	MOVLW	0x8B			;
	CALL	sub_0C11_HashCalc_byWisAddr		;

;	CALL	sub_1B51_Read09to10_InFSR0 		;;;;;
	MOVLW	0x09			;
	MOVWF	X_P,A			;
	LFSR	FSR0,0x8F		;RAM_8F
	CALL	sub_1B57_ReadEEorI2C_addrBYX_P_len8		;
;
	LFSR	FSR0,0x8B		;RAM_8B
	MOVLW	.8			;
	MOVWF	Temp_0,A		;
	MOVLW	0x88			;
	MOVWF	ADDRESS,A		;
	CALL	EE_WRITE_M		;
;
	BSF	RAM_CA,4,BANKED		;
	MOVF	RAM_CA,W,BANKED		;
	CALL	sub_1B07_I2C_addr9e_dataWandnotW		;
	BSF	STATUS,Z,A		;
adr_1ED0:
	BCF	BAT_CL			;battery ON
	RETURN				;
;---------------------------------------
sub_152E:
	CALL	ON_VL			;掉电中断
	CALL	sub_1B26_I2C_90to93_wr0x01		;写同步值
;	MOVLW	0x90			;
;	MOVWF	ADDRESS,A		;
;	MOVLW	0x01			;
;	MOVWF	TEMP_EE,A		;
;	MOVLW	.4			;
;	MOVWF	Temp_0,A		;
;CNTR_WR:
;;	CALL	EE_WRITE		;
;	CALL	IIC_WRITE		;

;	INCF	ADDRESS,F,A		;
;	DECFSZ	Temp_0,F,A		;
;	BRA	CNTR_WR			;
;
	CALL	sub_1672_GetDataBlock_555f_4149_2d37		;
	MOVLW	0x03			;
	ANDWF	RAM_C9,W,BANKED		;
	BZ	adr_1543		;0x55 0x5F如果全匹配

	MOVF	RAM_BF,F,BANKED;
	BNZ	adr_1543		;且当前次数的低两位均为0，刚好4次

	CALL	sub_1490_UpdateStepDatas;用于容错,每使用4次更新数据存储区
adr_1543:
	BCF	HLVDCON,HLVDEN		;
	BTFSS	BAT_CL,A		;
	BRA	adr_1555		;

adr_1548:
	BCF	BAT_CL			;
	MOVLW	.10			;
	CALL	DELAY_N_10MS		;
	BTFSS	PW_IN			;
	BRA	adr_1548		;
	RESET				;

adr_1555:
	CALL	sub_1672_GetDataBlock_555f_4149_2d37		;
	CALL	sub_1490_UpdateStepDatas		;;用于容错,每使用4次更新数据存储区
	CALL	sub_1562_Adjust41and4BData		;
	CALL	sub_15BE_Adjust2Dand37Data		;
	RETURN				;
;---------------------------------------

;---------------------------------------
sub_1381_Get2425TxParam:
	CALL	sub_1672_GetDataBlock_555f_4149_2d37		;
	COMF	RAM_C9,W,BANKED		;
	MOVWF	Temp_0,A		;

	MOVLW	0x03			;
	ANDWF	Temp_0,W,A		;
	BNZ	adr_13A2		;0x55 0x5F至少有1个匹配
;
					;RAM_C9 bit1,bit0 is '11',0x55 0x5F 两个数据段全不匹配
	MOVLW	0x30			;
	ANDWF	Temp_0,W,A		;
	BNZ	adr_1394		;0x41 0x4B至少有1个匹配

;					;RAM_C9 bit5,bit4 is '11',0x41 0x4B 两个数据段全不匹配
	CALL	ON_VL			;掉电中断

adr_1394:
	CALL	sub_1490_UpdateStepDatas		;;用于容错,每使用4次更新数据存储区
	BCF	HLVDCON,HLVDEN		;
	BTFSC	BAT_CL,A		;
	RESET				;
;adr_139F:
	CALL	sub_1672_GetDataBlock_555f_4149_2d37		;
;
adr_13A2:
	CALL	sub_13EB_Get69to70_FirstNFF		;读取运算次数,RAM_D2,RAM_C2,RAM_BF
	MOVF	RAM_C5,W,BANKED		;匹配的首地址，0x55,0x5F,如全不匹配是0x73.
	MOVWF	X_P,A			;
	CALL	sub_1B48_Load8EEto93to9A_byX_P		;读出EEPROM数据放到RAM_93以后的8个RAM
	MOVF	RAM_C2,F,BANKED		;0x69开始的8个数计算取出的有效数
	BNZ	adr_13D3		;

	CALL	ON_VL			;掉电中断
	CALL	sub_1442_USEDEC		;0x69起始的数据段:最低计数器单元进行减1操作

	MOVLW	0x4A			;
	BTFSC	RAM_D3_1,BANKED		;匹配数据段的最后一个单元置FF,
	MOVLW	0x54			;目的此处匹配的数据，下次不匹配
	CALL	EE_WT_CHK_AddrBYW_FFor0		;

	MOVF	RAM_C3,W,BANKED		;读出的53或49单元的数
	BNZ	adr_13C4		;;;BNN重点检测

;	CALL	sub_1BC5		;
	MOVLW	0x36			;
	BTFSS	RAM_C4,0,BANKED		;
	BRA	adr_13C2		;
	MOVLW	0x87			;
	CALL	EE_WT_CHK_AddrBYW_FFor0		;
	MOVLW	0x40			;
adr_13C2:
	CALL	EE_WT_CHK_AddrBYW_FFor0		;

adr_13C4:
	CALL	sub_13DE_block55or5f_verisFF_00		;
	MOVLW	0x80			;
	MOVWF	RAM_D2,BANKED		;
	BCF	HLVDCON,HLVDEN		;
	BTFSC	BAT_CL,A		;
	RESET				;
	BRA	adr_13DD		;

adr_13D3:
	MOVF	RAM_BF,W,BANKED		;
	BZ	adr_13DD		;;;BN重点检测

	CLRF	RAM_C0			;
	CALL	sub_14F0_HashCalc_NisC0_BF;循环滚步bf*c0次，每4次发生一次10——33——SSID

adr_13DD:
	RETURN				;
;---------------------------------------
sub_13DE_block55or5f_verisFF_00:
	MOVF	RAM_C5,W,BANKED		;已读出的数据匹配的首地址
	BNZ	adr_13E4		;

	MOVLW	0x73			;
adr_13E4:
	ADDLW	0x09			;0x7C,RAM_C5 + 09H
	CALL	EE_WT_CHK_AddrBYW_FFor0		;

	RETURN				;
;---------------------------------------
ON_VL:
	BCF	PIR2,HLVDIF		;
	BSF	PIE2,HLVDIE		;
	BSF	HLVDCON,HLVDEN		;

	BSF	INTCON,PEIE,A		;
	BSF	INTCON,GIE,A		;
	RETURN				;
;---------------------------------------


;---------------------------------------
sub_13EB_Get69to70_FirstNFF:;获得I2C中69到70第一个非FF的内容,内容在C2中，D2中为其&FC，BF中为其&03
	MOVLW	0X08			;
	MOVWF	X_P,A			;
adr_13ED:
	MOVLW	0X68			;
	ADDWF	X_P,W,A			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVLW	0XFF			;
	XORWF	TEMP_EE,W,A		;从EE的70单元查找到第一个非FF数值时跳出
	BNZ	adr_13FC		;

	DECFSZ	X_P,F,A			;
	BRA	adr_13ED		;

	MOVLW	0X01			;
	MOVWF	X_P,A			;
	MOVLW	0X00			;
	MOVWF	TEMP_EE,A		;
	BRA	adr_142F		;

adr_13FC:
	MOVF	TEMP_EE,W,A		;非FF的值
	MOVWF	RAM_BF,BANKED		;
	MOVF	X_P,W,A			;指向此值的指针
	MOVWF	RAM_D4,BANKED		;存储有效数据的指针,用于减1操作时写回数据

	DCFSNZ	X_P,F,A			;指针减1，并判断是否为01
	BRA	adr_142B		;

	MOVLW	0X68			;减之前X_P非01
	ADDWF	X_P,W,A			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVLW	0XFF			;再查找非FF的数值
	XORWF	TEMP_EE,W,A		;
	BNZ	adr_1419		;

	MOVLW	0X07			;查找到第2个
	XORWF	X_P,W,A			;
	BNZ	adr_142B		;

	MOVLW	0X01			;
	MOVWF	X_P,A			;
	MOVLW	0X69			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVLW	0XFF			;
	XORWF	TEMP_EE,W,A		;
	BNZ	adr_1424		;
	BRA	adr_142B		;
;
adr_1419:
	MOVF	TEMP_EE,W,A		;
	MOVWF	RAM_BF,BANKED		;
	MOVF	X_P,W,A			;
	MOVWF	RAM_D4,BANKED		;

	INCF	X_P,F,A			;
	MOVLW	0X09			;
	XORWF	X_P,W,A			;
	BNZ	adr_1424		;

	MOVLW	0X01			;
	MOVWF	X_P,A			;
adr_1424:
	MOVLW	0X68			;
	ADDWF	X_P,W,A			;
;	MOVWF	ADDRESS,A		;
;	MOVLW	0XFF			;
;	MOVWF	TEMP_EE,A		;
;	CALL	EE_WRITE		;
	CALL	EE_WT_CHK_AddrBYW_FFor0		;
adr_142B:
	MOVF	RAM_BF,W,BANKED		;

adr_142F:
	MOVWF	RAM_C2,BANKED		;
	CALL	sub_1435		;
;
	MOVF	X_P,W,A			;
	RETURN				;
;---------------------------------------
sub_1435:
	MOVLW	0XFC			;11111100
	ANDWF	RAM_C2,W,BANKED		;
	MOVWF	RAM_D2,BANKED		;

	MOVLW	0X03			;00000011
	ANDWF	RAM_C2,W,BANKED		;
	MOVWF	RAM_BF,BANKED		;

	RETURN				;
;---------------------------------------
sub_1490_UpdateStepDatas:;用于容错,每使用4次更新数据存储区
	MOVLW	0x03			;
	ANDWF	RAM_C9,W,BANKED		;
	BZ	adr_14B2		;0x55 0x5F如果全匹配，跳出函数

	MOVLW	0x30			;
	ANDWF	RAM_C9,W,BANKED		;
	SUBLW	0x30			;
	BNZ	adr_14A1		;0x41 0x4B至少一串匹配

	CALL	sub_1562_Adjust41and4BData		;0x41 0x4B全不匹配
adr_14A1:
	MOVLW	0x03			;
	ANDWF	RAM_C9,W,BANKED		;
	SUBLW	0x03			;
	BNZ	adr_14AD		;0x55 0x5F至少一串匹配

	BRA	adr_14B1		;0x55 0x5F全不匹配
adr_14AD:
	MOVLW	0x04			;
	SUBWF	RAM_D2,F,BANKED		;
adr_14B1:
	CALL	sub_14B1_Adjust55and5FData		;
adr_14B2:
	RETURN				;
;---------------------------------------
sub_14B1_Adjust55and5FData:
	MOVF	RAM_D2,F,BANKED		;
	BNN	adr_14CB		;
;负数
	MOVLW	0x30			;
	ANDWF	RAM_C9,W,BANKED		;
	BZ	adr_14BE		;0x41 0x4B全匹配

	CALL	sub_1562_Adjust41and4BData		;全匹配时不进行借位操作
adr_14BE:
	MOVLW	0x7C			;
	MOVWF	RAM_D2,BANKED		;

	MOVLW	0x41			;
	BTFSS	RAM_D3_1,BANKED		;
	MOVLW	0x4B			;
	BRA	adr_14D2		;
;0x7F>= N >=0x00
adr_14CB:
	MOVLW	0x4B			;
	BTFSS	RAM_D3_1,BANKED		;
	MOVLW	0x41			;
;共用
adr_14D2:
	MOVWF	X_P,A			;
	CALL	sub_1B48_Load8EEto93to9A_byX_P		;读出8个数据用于以下运算的基准

	MOVF	RAM_D2,F,BANKED		;
	BZ	adr_14E0		;

	MOVF	RAM_D2,W,BANKED		;
	MOVWF	RAM_BF,BANKED		;
	CLRF	RAM_C0,BANKED		;
	CALL	sub_14F0_HashCalc_NisC0_BF		;
adr_14E0:
	MOVLW	0x55			;
	BTFSS	RAM_D3_0,BANKED		;用于查询那个数据段需要修正
	MOVLW	0x5F			;

	MOVWF	RAM_C5,BANKED		;
	CALL	sub_1B74_FanXie93to9A_D2_xorVer_addrC5		;
	CALL	sub_1680_Check55and5fdata_reC5		;

	RETURN				;
;---------------------------------------
sub_1562_Adjust41and4BData:
	MOVLW	0x30			;
	ANDWF	RAM_C9,W,BANKED		;
	BZ	adr_15BD		;0x41 0x4B全部匹配跳出

	MOVLW	0xC0			;
	ANDWF	RAM_C9,W,BANKED		;
	SUBLW	0xC0			;
	BNZ	adr_1573		;0x2D,0x37至少一串匹配

	CALL	sub_15BE_Adjust2Dand37Data		;0x2D,0x37全不匹配
adr_1573:
	DECF	RAM_C3,F,BANKED		;0x41 or 0x4B 段读出的计数值
	BNN	adr_1593		;为正数跳转
;
	MOVLW	0x1F			;从1F递减至00至1F
	ANDWF	RAM_C3,F,BANKED		;

	MOVLW	0xC0			;借位处理
	ANDWF	RAM_C9,W,BANKED		;
	BZ	adr_1587		;0x2D,0x37全匹配

	CALL	sub_15BE_Adjust2Dand37Data		;
adr_1587:
	;CALL	sub_1663_Tx10_33_SSID		;IR send:33 8F 90 61 CB
	MOVLW	0x2D			;
	BTFSS	RAM_C4,0,BANKED		;
	MOVLW	0x37			;
	BRA	adr_159D		;
;无借位
adr_1593:
;	CALL	sub_1663_Tx10_33_SSID		;IR send:33 8F 90 61 CB
	MOVLW	0x2D			;
	BTFSC	RAM_C4,0,BANKED		;
	MOVLW	0x37			;

adr_159D:
	MOVWF	X_P,A			;
	CALL	sub_1B48_Load8EEto93to9A_byX_P		;读出需要修正段的前8个数据
	MOVF	RAM_C3,W,BANKED		;计数器减1后写回
	MOVWF	RAM_9B,BANKED		;
	BZ	adr_15B0		;

	MOVLW	0x80			;
	MULWF	RAM_9B,BANKED		;
	MOVF	PRODH,W,A		;
	MOVWF	RAM_C0,BANKED		;
	MOVF	PRODL,W,A		;
	MOVWF	RAM_BF,BANKED		;
	CALL	sub_14F0_HashCalc_NisC0_BF		;计算次数是当前值乘以0X80
adr_15B0:
	MOVLW	0x41			;
	BTFSC	RAM_C3,0,BANKED		;
	MOVLW	0x4B			;
	CALL	sub_1B7E_WriteBlock_93_9C_AddrBYW		;计算出+9单元数据重写使数据段匹配
	CALL	sub_16B8_CheckEE41andEE49_reC3		;再次检测0x41 0x4B匹配
adr_15BD:
	RETURN				;
;---------------------------------------
sub_15BE_Adjust2Dand37Data:
	MOVLW	0xC0			;
	ANDWF	RAM_C9,W,BANKED		;
	BZ	adr_162D		;

	MOVF	RAM_C4,W,BANKED		;
	BZ	adr_162A		;

	BTFSS	RAM_C9,6,BANKED		;
	BRA	adr_15DF		;

	CALL	sub_1663_Tx10_33_SSID		;
	MOVLW	0x7E			;
	MOVWF	X_P,A			;
	CALL	sub_1B48_Load8EEto93to9A_byX_P		;

	MOVLW	0x86			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVLW	0x7F			;
	ANDWF	TEMP_EE,W,A		;
	MOVWF	RAM_9B,BANKED		;
	MOVLW	0x2D			;
	CALL	sub_1B7E_WriteBlock_93_9C_AddrBYW;
adr_15DF:
	BTFSS	RAM_C9,7,BANKED		;
	BRA	adr_162A		;
	;CALL	sub_1663_Tx10_33_SSID		;
	CLRF	RAM_9B,BANKED		;
	MOVLW	0x37			;
	CALL	sub_162E		;有Bug
	MOVLW	0x7E			;
	CALL	sub_162E		;
	DECF	RAM_C4,F,BANKED		;
	BTFSS	RAM_9B,0,BANKED		;
	BRA	adr_15FB		;

	MOVLW	0x37			;
	MOVWF	X_P,A			;
	CALL	sub_1B48_Load8EEto93to9A_byX_P		;
adr_15FB:
	MOVF	RAM_9B,W,BANKED		;
	BNZ	adr_1605		;

	CALL	sub_1643		;
	;CALL	sub_1663_Tx10_33_SSID		;
adr_1605:
	INCF	RAM_9B,F,BANKED		;
	MOVLW	0x7F			;
	ANDWF	RAM_9B,W,BANKED		;
	SUBWF	RAM_C4,W,BANKED		;
	BNZ	adr_1611		;

	MOVF	RAM_C4,W,BANKED		;
	MOVWF	RAM_9B,BANKED		;
adr_1611:
	MOVLW	0x10			;
	MOVWF	RAM_C0,BANKED		;
	CLRF	RAM_BF,BANKED		;
	CALL	sub_14F0_HashCalc_NisC0_BF		;
	MOVLW	0x7E			;
	BTFSC	RAM_9B,0,BANKED		;
	MOVLW	0x37			;
	CALL	sub_1B7E_WriteBlock_93_9C_AddrBYW		;
	;CALL	sub_1663_Tx10_33_SSID		;
	BTFSC	RAM_9B,7,BANKED		;
	BRA	adr_1605		;
adr_162A:
 	CALL	sub_1742_Check2Dand37data_reC4		;
adr_162D:
	RETURN				;
;---------------------------------------
sub_162E:
	MOVWF X_P,BANKED		;	;;;;RAM_D1,BANKED		;
	CALL	sub_1BB6_XOR_xptoxp10_reW		;
	BNZ	adr_1642		;

	MOVLW	0x08			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	SUBLW	0XFF			;
	BZ	adr_1642		;

	MOVF	RAM_9B,W,BANKED		;
	SUBWF	TEMP_EE,W,A		;
	BNC	adr_1642		;Lower

	MOVF	TEMP_EE,W,A		;
	MOVWF	RAM_9B,BANKED		;
adr_1642
	RETURN				;
;---------------------------------------
sub_1643:;写入地址有bug
	MOVLW	0x03			;
	MOVWF	X_P,A			;
	MOVLW	0x24			;
	MOVWF	A_P,A			;
adr_1647:
	MOVF	RAM_C4,W,BANKED		;
	SUBWF	A_P,W,A			;
	BNC	adr_1650		;

	MOVLW	0x0C			;
	SUBWF	A_P,F,A			;
	DECFSZ	X_P,F,A			;
	BRA	adr_1647		;
adr_1650:
	MOVLW	0x80			;
	IORWF	A_P,W,A			;
	MOVWF	RAM_9B,BANKED		;
	MOVLW	0x08			;
	MULWF	X_P,A			;
	MOVLW	0x0D			;
	ADDWF	PRODL,W,A		;
	MOVWF	X_P,A			;
	CALL	sub_1B48_Load8EEto93to9A_byX_P		;
	CALL	sub_1B7E_WriteBlock_93_9C_AddrBYW		;写入地址有bug

	RETURN				;
;---------------------------------------

;---------------------------------------
sub_1663_Tx10_33_SSID:
	RETURN
	MOVLW	0x33			;
	MOVWF	RAM_81,BANKED		;

	MOVLW	0x09			;
	MOVWF	X_P,A			;
	LFSR	FSR0,0x82		;RAM_82
	CALL	sub_1B57_ReadEEorI2C_addrBYX_P_len8		;

	MOVLW	.5			;
	MOVWF	X_P,A			;
	CALL	TX_IR_B_10_data81_LenBYX_P_add1			;

	RETURN				;
;---------------------------------------
;---------------------------------------
sub_14F0_HashCalc_NisC0_BF:;循环滚步bf*c0次，每4次发生一次10——33——SSID
adr_14F0:
	MOVLW	0x04			;
	MOVWF	Temp_4,A		;
adr_14F4:
	CALL	sub_150C		;
	BNC	adr_150B		;

	MOVLW	0x93			;
	CALL	sub_0C11_HashCalc_byWisAddr		;
	DECFSZ	Temp_4,F,A		;
	BRA	adr_14F4		;

	MOVLW	0x04			;
	MOVWF	Temp_4,A		;
	;;;;CALL	sub_1663_Tx10_33_SSID		;
	BRA	adr_14F0		;
adr_150B:
	RETURN				;
;---------------------------------------
sub_150C:
	DECF	RAM_BF,F,BANKED		;
	MOVLW	0x00			;
	SUBWFB	RAM_C0,F,BANKED		;
	RETURN				;
;---------------------------------------
sub_1B74_FanXie93to9A_D2_xorVer_addrC5:
	MOVF	RAM_D2,W,BANKED		;
	MOVWF	RAM_9B,BANKED		;
	MOVF	RAM_C5,W,BANKED		;
	CALL	sub_1B7E_WriteBlock_93_9C_AddrBYW		;
	RETURN				;
;---------------------------------------

;---------------------------------------
sub_1B7E_WriteBlock_93_9C_AddrBYW:;将加载好93到9A数据倒序，结合9B得到校验，写入到W所指示的地址段中，而后再倒序 
	MOVWF	RAM_D1,BANKED		;
	MOVLW	0x08			;
	ADDWF	RAM_D1,W,BANKED		;
	MOVWF	X_P,A			;0x69+0x08=0x71
	CALL	sub_1BE7_93to9A_FanXuPaiLie		;逆序
	CALL	sub_1B9D_XOR_93to9A_reA_P		;计算+9单元的校验值
	MOVF	A_P,W,A			;
	XORWF	RAM_9B,W,BANKED		;
	MOVWF	RAM_9C,BANKED		;

	LFSR	FSR0,0X93		;RAM_93
	MOVLW	.10			;
	MOVWF	Temp_0,A		;
	MOVF	RAM_D1,W,BANKED		;
	MOVWF	ADDRESS,A		;
	CALL	EE_WRITE_M		;

	CALL	sub_1BE7_93to9A_FanXuPaiLie		;逆序
	RETURN				;
;---------------------------------------
;---------------------------------------
sub_1BE7_93to9A_FanXuPaiLie:
	MOVLW	.4			;
	MOVWF	Temp_0,A		;
	LFSR	FSR0,0x93		;RAM_93
	LFSR	FSR1,0x9A		;RAM_9A
	CALL	sub_1BEF		;
	RETURN				;
;---------------------------------------
sub_1BEF:
adr_1BEF:
	MOVF	INDF0,W,A		;
	MOVWF	Temp_1,A		;
	MOVF	INDF1,W,A		;
	MOVWF	POSTINC0,A		;
	MOVF	Temp_1,W,A		;
	MOVWF	POSTDEC1,A		;

	DECFSZ	Temp_0,F,A		;
	BRA	adr_1BEF		;
	RETURN				;
;---------------------------------------


;---------------------------------------
sub_1B48_Load8EEto93to9A_byX_P:
	LFSR	FSR0,0X9A		;RAM_9A
	CALL	LOAD_8EE_F		;

;	LFSR	FSR0,0X93		;RAM_93
;	CALL	sub_1B57_ReadEEorI2C_addrBYX_P_len8		;load 8 EEPROM DATA
;	CALL	sub_1BE7_93to9A_FanXuPaiLie		;loaded data 反序
	RETURN				;
;---------------------------------------
LOAD_8EE_F:
	MOVF	X_P,W,A			;
	MOVWF	ADDRESS,A		;

	MOVLW	0X08			;
	MOVWF	Temp_5,A		;
LOAD_8EE_F_LP:
	BTFSC	f20_RDee,A		;
	BRA	rdee_1			;
	BRA	rdee_0			;
rdee_1:
	CALL	EE_READ			;
	BRA	rdee_F			;
rdee_0:
	CALL	EE_READ
;	CALL	IIC_READ_byTempEE_Tempbb		;
rdee_F:
	MOVWF	POSTDEC0,A		;

	INCF	ADDRESS,F,A		;
	DECFSZ	Temp_5,F,A		;
	BRA	LOAD_8EE_F_LP		;

	BCF	f20_RDee,A		;

	RETURN				;
;---------------------------------------
sub_1B51_Read09to10_InFSR0:
	MOVLW	0x09			;
	MOVWF	X_P,A			;
;	LFSR	FSR0,0X87		;RAM_87
	CALL	sub_1B57_ReadEEorI2C_addrBYX_P_len8		;

	RETURN				;
;---------------------------------------

;---------------------------------------
LOAD_8EE_Z:

sub_1B57_ReadEEorI2C_addrBYX_P_len8:
	MOVF	X_P,W,A			;
	MOVWF	ADDRESS,A		;

	MOVLW	0X08			;
	MOVWF	Temp_1,A		;
adr_1B5F:
	BTFSC	f20_RDee,A		;
	BRA	RDee_1			;
	BRA	RDee_0			;
RDee_1:
	CALL	EE_READ			;
	BRA	RDee_F			;
RDee_0:
	CALL	EE_READ
;	CALL	IIC_READ_byTempEE_Tempbb		;
RDee_F:
	MOVWF	POSTINC0,A		;

	INCF	ADDRESS,F,A		;
	DECFSZ	Temp_1,F,A		;
	BRA	adr_1B5F		;

	BCF	f20_RDee,A		;

	RETURN				;
;---------------------------------------

;---------------------------------------
sub_1AF5_CheckRomData:
	MOVLW	0X9E			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVWF	Temp_0,A		;
	MOVLW	0X9F			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVWF	Temp_1,A		;

	CALL	INI_24C			;

	MOVF	Temp_1,W,A		;
	ADDWF	Temp_0,W,A		;
	BNZ	sub_1AF5_CheckRomData_END		;

	MOVLW	0X7F			;
	ANDWF	Temp_0,W,A		;
	MOVWF	RAM_CA,BANKED		;
	MOVLW	0XEE			;
	ANDWF	RAM_CA,W,BANKED		;
	MOVWF	Temp_0,A		;
	MOVLW	0X04			;
	XORWF	Temp_0,W,A		;

sub_1AF5_CheckRomData_END:
	RETURN				;
;------------------------------------
INI_24C:
	MOVLW	0xFF			;
	XORWF	Temp_0,W,A		;
	BNZ	INI_24C_END		;

	MOVLW	0xFF			;
	XORWF	Temp_1,W,A		;
	BNZ	INI_24C_END		;

	BSF	BAT_CL			;
;---
	MOVLW	0x9E			;
	MOVWF	ADDRESS_L,A		;
	MOVLW	0x9E			;
	MOVWF	ADDRESS,A		;

	CALL	EE_READ			;
	MOVWF	Temp_0,A		;
	CALL	IIC_WRITE		;

	MOVLW	0x9F			;
	MOVWF	ADDRESS_L,A		;
	MOVLW	0x9F			;
	MOVWF	ADDRESS,A		;

	CALL	EE_READ			;
	MOVWF	Temp_1,A		;
	CALL	IIC_WRITE		;
;---
;	MOVLW	0x9E			;0x80
;	MOVWF	ADDRESS_L,A		;
;	MOVLW	0x9E			;0x80
;	MOVWF	ADDRESS,A		;
;	MOVLW	.2			;32
;	MOVWF	CNT0,A			;
;INI_24C_0:
;	CALL	EE_READ			;
;	CALL	IIC_WRITE		;

;	INCF	ADDRESS_L,F,A		;
;	INCF	ADDRESS,F,A		;
;	DECFSZ	CNT0,F,A		;
;	BRA	INI_24C_0		;

	BCF	BAT_CL			;

INI_24C_END:
	RETURN				;
;------------------------------------
sub_1B07_I2C_addr9e_dataWandnotW:
	MOVWF	RAM_8B,BANKED		;
	MOVF	RAM_8B,W,BANKED		;
	SUBLW	0x00			;
	MOVWF	RAM_8C,BANKED		;

	MOVLW	0x9E			;
	MOVWF	ADDRESS,A		;
	LFSR	FSR0,0x8B		;RAM_8B
	MOVLW	.2			;
	MOVWF	Temp_0,A		;
	CALL	EE_WRITE_M		;
	RETURN				;
;------------------------------------
sub_1BAF:
	MOVWF	X_P,A			;
;	MOVWF	RAM_D1,BANKED		;
 	MOVLW	0X08			;
	ADDWF	X_P,W,A			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVF	TEMP_EE,F,A		;
	BN	sub_1BAF_END		;N

	CALL	sub_1BB6_XOR_xptoxp10_reW		;
sub_1BAF_END:
	RETURN				;
;---------------------------------------
sub_1BB6_XOR_xptoxp10_reW:
	CALL	sub_1B48_Load8EEto93to9A_byX_P		;
	CALL	sub_1B9D_XOR_93to9A_reA_P		;

	MOVLW	0X08			;
	ADDWF	X_P,W,A			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVWF	Temp_2,A		;
	INCF	ADDRESS,F,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVWF	Temp_3,A		;

	MOVF	A_P,W,A			;
	XORWF	Temp_2,W,A		;
	XORWF	Temp_3,W,A		;
	RETURN				;
;---------------------------------------
;---------------------------------------
sub_1B9D_XOR_93to9A_reA_P:
	LFSR	FSR0,0X93		;RAM_93
	MOVF	POSTINC0,W,A		;
	XORWF	POSTINC0,W,A		;
	XORWF	POSTINC0,W,A		;
	XORWF	POSTINC0,W,A		;
	XORWF	POSTINC0,W,A		;
	XORWF	POSTINC0,W,A		;
	XORWF	POSTINC0,W,A		;
	XORWF	INDF0,W,A		;
	MOVWF	A_P,A			;
	RETURN				;
;---------------------------------------
;sub_1BE7_93to9A_FanXuPaiLie:
;	MOVLW	0X08			;
;	MOVWF	Temp_2,A		;counter
;	MOVLW	0X93			;
;	MOVWF	Temp_3,A		;pointer

;	CALL	sub_1BEF		;
;	RETURN				;
;------------------------------------
;sub_1BEF:
;	DECF	Temp_2,W,A		;
;	ADDWF	Temp_3,W,A		;
;	MOVWF	Temp_4,A		;pointer +N-1
;	RRNCF	Temp_2,F,A		;

;adr_1BFA:
;	MOVF	Temp_3,W,A		;
;	MOVWF	FSR0L,A			;
;	MOVF	INDF0,W,A		;
;	MOVWF	RAM_BE,BANKED		;

;	MOVF	Temp_4,W,A		;
;	MOVWF	FSR0L,A			;
;	MOVF	INDF0,W,A		;
;	MOVWF	RAM_BC,BANKED		;TEMP
;	MOVF	Temp_3,W,A		;
;	MOVWF	FSR0L,A			;
;	MOVF	RAM_BC,W,BANKED		;
;	MOVWF	INDF0,A			;

;	MOVF	Temp_4,W,A		;
;	MOVWF	FSR0L,A			;
;	MOVF	RAM_BE,W,BANKED		;
;	MOVWF	INDF0,A			;

;	INCF	Temp_3,F,A		;
;	DECF	Temp_4,F,A		;

;	DECFSZ	Temp_2,F,A		;
;	BRA	adr_1BFA		;
;	RETURN				;
;---------------------------------------
sub_1C30_RxFrame_pBuf80:    ;帧数据起始延时在CNT2中
	MOVWF	CNT2,A			;

	BSF	f20_ONbuzh,A		;捕捉开始
	BCF	f20_ONcal,A		;清除计算标志
	BCF	f20_rxok,A		;

	CLRF	RAM_A4,BANKED		;

	BCF	PIR2,TMR3IF		;
	BCF	T3CON,TMR3ON		;
	CLRF	TMR3L,A			;
	CLRF	TMR3H,A			;
	BSF	T3CON,TMR3ON		;

	BSF	f21_over1,A		;

	MOVLW	B'00000100'		;
	MOVWF	CCP1CON,A		;
	BCF	PIR1,CCP1IF		;
	BSF	PIE1,CCP1IE		;
	BSF	INTCON,PEIE,A		;
	BSF	INTCON,GIE,A		;

	MOVF	CNT2,W,A		;
	CALL	DELAY_N_10MS		;
	BTFSC	f21_over1,A		;
	BRA	adr_1C73_exitCCP		;
RX_HEAD:
	CALL	sub_1C8F_waitRxNibbleByte_inW_RAM_A4		;
	BTFSC	f21_over1,A		;
	BRA	adr_1C73_exitCCP;
	BZ	RX_HEAD			;

	CALL	sub_1C7A_RxByte_inW_RAM_A3		;
	BTFSC	f21_over1,A		;
	BRA	adr_1C73_exitCCP		;

	MOVF	RAM_A3,W,BANKED		;
	MOVWF	RAM_81,BANKED		;
	CALL	sub_1C9B_GetNeedRxLen_retCNT0		;
	MOVF	CNT0,F,A		;
	BZ	adr_1C73_exitCCP		;

	LFSR	FSR0,0x82		;RAM_82
adr_1C64:
	CALL	sub_1C7A_RxByte_inW_RAM_A3		;
	BTFSC	f21_over1,A		;
	BRA	adr_1C73_exitCCP		;
	MOVF	RAM_A3,W,BANKED		;
	MOVWF	POSTINC0,A		;

	DECFSZ	CNT0,F,A		;
	BRA	adr_1C64		;
adr_1C73_exitCCP:
	BCF	f20_ONbuzh,A		;捕捉开始
	BCF	f20_ONcal,A		;清除计算标志
	BCF	f20_rxok,A		;

	BCF	PIR1,CCP1IF		;
	BCF	PIE1,CCP1IE		;
	BCF	INTCON,PEIE,A		;
	BCF	INTCON,GIE,A		;
	CLRF	CCP1CON,A		;

;	BCF	RFOUT_OP		;

	RETURN				;
;---------------------------------------
sub_1C7A_RxByte_inW_RAM_A3:
	CALL	sub_1C8F_waitRxNibbleByte_inW_RAM_A4		;
	BTFSC	f21_over1,A		;
	BRA	adr_1C8E		;

	MOVWF	RAM_A3,BANKED		;低半字节
;
	CALL	sub_1C8F_waitRxNibbleByte_inW_RAM_A4		;高半字节
	BTFSC	f21_over1,A		;
	BRA	adr_1C8E		;

	SWAPF	RAM_A4,W,BANKED		;
	IORWF	RAM_A3,F,BANKED		;
adr_1C8E:
	RETURN				;
;---------------------------------------
sub_1C8F_waitRxNibbleByte_inW_RAM_A4:
	BCF	f20_rxok,A		;
;	BCF	PIR2,TMR3IF		;
;	BSF	PIE1,CCP1IE		;
;	BCF	PIR1,CCP1IF		;
adr_1C95:
	CLRWDT				;
	BTFSS	f20_rxok,A		;
	BRA	adr_1C95		;

	MOVF	RAM_A4,W,BANKED		;
	RETURN				;
;---------------------------------------
sub_1C9B_GetNeedRxLen_retCNT0:
	MOVLW	.0			;
	MOVWF	CNT0,A			;

	MOVLW	0x08			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_1CD0		;

	MOVLW	0x0F			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_1CD0		;

	MOVLW	0x6A			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_1CD0		;

	MOVLW	0x71			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_1CD0		;
;
	MOVLW	.2			;
	MOVWF	CNT0,A			;

	MOVLW	0x73			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_1CD0		;

	MOVLW	0x79			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_1CD0		;
;
	MOVLW	.7			;
	MOVWF	CNT0,A			;

	MOVLW	0x7A			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_1CD0		;
;
	MOVLW	.8			;
	MOVWF	CNT0,A			;

	MOVLW	0x26			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_1CD0		;

	MOVLW	0x63			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_1CD0		;

	MOVLW	0x3E			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_1CD0		;

	MOVLW	0x39			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_1CD0		;
;
	MOVLW	.9			;
	MOVWF	CNT0,A			;
;
adr_1CD0:
	RETURN				;
;---------------------------------------
sub_0C11_HashCalc_byWisAddr:
	;;;;RETURN
	MOVWF	RAM_BB,BANKED		;

	LFSR	FSR0,0XA3		;RAM_A3

	MOVLW	UPPER(tab_0CB7)		;
	MOVWF	TBLPTRU,A		;
	MOVLW	HIGH(tab_0CB7)		;
	MOVWF	TBLPTRH,A		;
	MOVLW	LOW(tab_0CB7)		;
	MOVWF	TBLPTRL,A		;

	MOVLW	0X10			;
	MOVWF	Temp_0,A		;
adr_0C15:
	TBLRD*+				;
	MOVF	TABLAT,W,A		;
	MOVWF	POSTINC0,A		;
	DECFSZ	Temp_0,F,A		;
	BRA	adr_0C15		;

	CALL	sub_0AFE		;

	MOVLW	0X00			;
	CALL	sub_0CDF		;
	CALL	sub_0A9C		;
	CALL	sub_0AFE		;

	MOVLW	0X04			;
	CALL	sub_0CDF		;
	CALL	sub_09FC		;

	MOVLW	UPPER(TAB_0ADE)		;
	MOVWF	TBLPTRU,A		;
	MOVLW	HIGH(TAB_0ADE)		;
	MOVWF	TBLPTRH,A		;
	MOVLW	LOW(TAB_0ADE)		;
	MOVWF	TBLPTRL,A		;

	MOVLW	0X00			;
	MOVWF	RAM_BC,BANKED		;
	MOVLW	0X00			;
	MOVWF	RAM_BD,BANKED		;
	CALL	sub_0CFA		;

	MOVLW	0X8E			;
	MOVWF	RAM_BC,BANKED		;
;	MOVLW	0X10			;
;	MOVWF	RAM_BD,BANKED		;
	CALL	sub_0CFA		;

	MOVLW	0X08			;
	MOVWF	Temp_0,A		;
	LFSR	FSR0,0XA3		;RAM_A3
	LFSR	FSR1,0XB3		;RAM_B3
adr_0C40:
	MOVF	POSTINC0,W,A		;计算结果转存
	MOVWF	POSTINC1,A		;
	DECFSZ	Temp_0,F,A		;
	BRA	adr_0C40		;
;
	MOVLW	UPPER(tab_0CC7)		;
	MOVWF	TBLPTRU,A		;
	MOVLW	HIGH(tab_0CC7)		;
	MOVWF	TBLPTRH,A		;
	MOVLW	LOW(tab_0CC7)		;
	MOVWF	TBLPTRL,A		;

	LFSR	FSR0,0XA3		;RAM_A3
	MOVLW	0X10			;
	MOVWF	Temp_0,A		;
adr_0C49:
	TBLRD*+				;
	MOVF	TABLAT,W,A		;
	MOVWF	POSTINC0,A		;
	DECFSZ	Temp_0,F,A		;
	BRA	adr_0C49		;
;
	CALL	sub_0B2F		;

	MOVLW	0X00			;
	CALL	sub_0CDF		;
	CALL	sub_0A9C		;
	CALL	sub_0B2F		;

	MOVLW	0X04			;
	CALL	sub_0CDF		;
	CALL	sub_09FC		;

	MOVLW	UPPER(TAB_0ADE)		;
	MOVWF	TBLPTRU,A		;
	MOVLW	HIGH(TAB_0ADE)		;
	MOVWF	TBLPTRH,A		;
	MOVLW	LOW(TAB_0ADE)		;
	MOVWF	TBLPTRL,A		;

	MOVLW	0X1C			;
	MOVWF	RAM_BC,BANKED		;
	MOVLW	0X00			;
	MOVWF	RAM_BD,BANKED		;
	CALL	sub_0CFA		;

	MOVLW	0XAA			;
	MOVWF	RAM_BC,BANKED		;
;	MOVLW	0X10			;
;	MOVWF	RAM_BD,BANKED		;
	CALL	sub_0CFA		;
;
	MOVLW	0xFE			;
	ADDWF	RAM_BA,F,BANKED		;
	MOVLW	0xDC			;
	ADDWFC	RAM_B9,F,BANKED		;
	MOVLW	0xBA			;
	ADDWFC	RAM_B8,F,BANKED		;
	MOVLW	0x98			;
	ADDWFC	RAM_B7,F,BANKED		;

	MOVLW	0x89			;
	ADDWF	RAM_B6,F,BANKED		;
	MOVLW	0xAB			;
	ADDWFC	RAM_B5,F,BANKED		;
	MOVLW	0xCD			;
	ADDWFC	RAM_B4,F,BANKED		;
	MOVLW	0xEF			;
	ADDWFC	RAM_B3,F,BANKED		;
;
	MOVF	RAM_BB,W,BANKED		;
	MOVWF	FSR0L,A			;

	MOVF	RAM_B6,W,BANKED		;
	ADDWF	RAM_AA,W,BANKED		;
	MOVWF	POSTINC0,A		;

	MOVF	RAM_B5,W,BANKED		;
	ADDWFC	RAM_A9,W,BANKED		;
	MOVWF	POSTINC0,A		;

	MOVF	RAM_B4,W,BANKED		;
	ADDWFC	RAM_A8,W,BANKED		;
	MOVWF	POSTINC0,A		;

	MOVF	RAM_B3,W,BANKED		;
	ADDWFC	RAM_A7,W,BANKED		;
	MOVWF	POSTINC0,A		;
;
	MOVF	RAM_BA,W,BANKED		;
	ADDWF	RAM_AE,W,BANKED		;
	MOVWF	POSTINC0,A		;

	MOVF	RAM_B9,W,BANKED		;
	ADDWFC	RAM_AD,W,BANKED		;
	MOVWF	POSTINC0,A		;

	MOVF	RAM_B8,W,BANKED		;
	ADDWFC	RAM_AC,W,BANKED		;
	MOVWF	POSTINC0,A		;

	MOVF	RAM_B7,W,BANKED		;
	ADDWFC	RAM_AB,W,BANKED		;
	MOVWF	POSTINC0,A		;

	RETURN				;
;---------------------------------------


;---------------------------------------

;---------------------------------------

	RETURN				;
;---------------------------------------
sub_0AFE:
	MOVF	RAM_A6,W,BANKED		;
	XORWF	RAM_AA,W,BANKED		;
	ANDWF	RAM_B2,W,BANKED		;
	XORWF	RAM_AA,W,BANKED		;
	ADDWF	RAM_AE,F,BANKED		;

	MOVF	RAM_A5,W,BANKED		;
	XORWF	RAM_A9,W,BANKED		;
	ANDWF	RAM_B1,W,BANKED		;
	XORWF	RAM_A9,W,BANKED		;
	ADDWFC	RAM_AD,F,BANKED		;

	MOVF	RAM_A4,W,BANKED		;
	XORWF	RAM_A8,W,BANKED		;
	ANDWF	RAM_B0,W,BANKED		;
	XORWF	RAM_A8,W,BANKED		;
	ADDWFC	RAM_AC,F,BANKED		;

	MOVF	RAM_A3,W,BANKED		;
	XORWF	RAM_A7,W,BANKED		;
	ANDWF	RAM_AF,W,BANKED		;
	XORWF	RAM_A7,W,BANKED		;
	ADDWFC	RAM_AB,F,BANKED		;

	RETURN				;
;---------------------------------------
sub_0B2F:
	CALL	sub_0AFE		;

	MOVLW	0XE6			;
	ADDWF	RAM_AE,F,BANKED		;

	MOVLW	0X8B			;
	ADDWFC	RAM_AD,F,BANKED		;

	MOVLW	0XA2			;
	ADDWFC	RAM_AC,F,BANKED		;

	MOVLW	0X50			;
	ADDWFC	RAM_AB,F,BANKED		;

	RETURN				;
;---------------------------------------
sub_0CDF:
	ADDWF	RAM_BB,W,BANKED		;
;	MOVWF	X_P,A			;
	MOVWF	FSR0L,A			;

	MOVF	POSTINC0,W,A		;
	ADDWF	RAM_AE,F,BANKED		;

	MOVF	POSTINC0,W,A		;
	ADDWFC	RAM_AD,F,BANKED		;

	MOVF	POSTINC0,W,A		;
	ADDWFC	RAM_AC,F,BANKED		;

	MOVF	POSTINC0,W,A		;
	ADDWFC	RAM_AB,F,BANKED		;

	RETURN				;
;---------------------------------------




	RETURN				;
;---------------------------------------
sub_0CFA:
	MOVLW	0X10			;
	MOVWF	RAM_C1,BANKED		;

adr_0D02:
	MOVLW	0X00			;
	CPFSEQ	RAM_BC,BANKED		;
	BRA	adr_0D02_1		;
	CALL	sub_0B4B		;
	BRA	adr_0D02_E		;
adr_0D02_1
	MOVLW	0X1C			;
	CPFSEQ	RAM_BC,BANKED		;
	BRA	adr_0D02_2		;
	CALL	sub_0B67		;
	BRA	adr_0D02_E		;
adr_0D02_2
	MOVLW	0X8E			;
	CPFSEQ	RAM_BC,BANKED		;
	BRA	adr_0D02_3		;
	CALL	sub_0BD9		;
	BRA	adr_0D02_E		;
adr_0D02_3
	MOVLW	0XAA			;
	CPFSEQ	RAM_BC,BANKED		;
	BRA	adr_0D02_E		;
	CALL	sub_0BF5		;
;	BRA	adr_0D02_E		;
adr_0D02_E:
;
	MOVLW	0X06			;
	XORWF	RAM_BD,W,BANKED		;
	BZ	adr_0D11		;

	MOVLW	0X0C			;
	XORWF	RAM_BD,W,BANKED		;
	BZ	adr_0D10		;

	MOVLW	0X15			;
	XORWF	RAM_BD,W,BANKED		;
	BZ	adr_0D11		;

	MOVLW	0X18			;
	XORWF	RAM_BD,W,BANKED		;
	BZ	adr_0D10		;

	BRA	adr_0D13		;
adr_0D10:
	MOVLW	0X00			;
	BRA	adr_0D12		;
adr_0D11:
	MOVLW	0X04			;
adr_0D12:
	CALL	sub_0CDF		;
adr_0D13:
	TBLRD*+				;
	MOVF	TABLAT,W,A		;
	MOVWF	Temp_0,A		;

	MOVLW	0X00			;
	CPFSEQ	Temp_0,A		;
	BRA	FENZHI_1		;
	CALL	sub_09FC		;INDEX -0
	BRA	FENZHI_1_E		;
FENZHI_1
	MOVLW	0X82			;
	CPFSEQ	Temp_0,A		;
	BRA	FENZHI_2		;
	CALL	sub_0A7E		;
	BRA	FENZHI_1_E		;
FENZHI_2
	MOVLW	0X8F			;
	CPFSEQ	Temp_0,A		;
	BRA	FENZHI_3		;
	CALL	sub_0A8B		;
	BRA	FENZHI_1_E		;
FENZHI_3
	MOVLW	0X99			;
	CPFSEQ	Temp_0,A		;
	BRA	FENZHI_4		;
	CALL	sub_0A95		;
	BRA	FENZHI_1_E		;
FENZHI_4
	MOVLW	0XA0			;
	CPFSEQ	Temp_0,A		;
	BRA	FENZHI_5		;
	CALL	sub_0A9C		;
	BRA	FENZHI_1_E		;
FENZHI_5
	MOVLW	0XA7			;
	CPFSEQ	Temp_0,A		;
	BRA	FENZHI_6		;
	CALL	sub_0AA3		;
	BRA	FENZHI_1_E		;
FENZHI_6
	MOVLW	0XB4			;
	CPFSEQ	Temp_0,A		;
	BRA	FENZHI_7		;
	CALL	sub_0AB0		;
	BRA	FENZHI_1_E		;
FENZHI_7
	MOVLW	0XC4			;
	CPFSEQ	Temp_0,A		;
	BRA	FENZHI_8		;
	CALL	sub_0AC0		;
	BRA	FENZHI_1_E		;
FENZHI_8
	MOVLW	0XD1			;
	CPFSEQ	Temp_0,A		;
	BRA	FENZHI_9		;
	CALL	sub_0ACD		;
	BRA	FENZHI_1_E		;
FENZHI_9
	MOVLW	0XDB			;
	CPFSEQ	Temp_0,A		;
	BRA	FENZHI_N		;
	CALL	sub_0AD7		;
	BRA	FENZHI_1_E		;
FENZHI_N:
	CLRWDT				;
	BRA	FENZHI_N		;
FENZHI_1_E
	INCF	RAM_BD,F,BANKED		;

	DECFSZ	RAM_C1,F,BANKED		;
	BRA	adr_0D02		;

	RETURN				;
;---------------------------------------
sub_0B4B:
	CALL	sub_0B67		;

	MOVLW	0X99			;
	ADDWF	RAM_AE,F,BANKED		;

	MOVLW	0X79			;
	ADDWFC	RAM_AD,F,BANKED		;

	MOVLW	0X82			;
	ADDWFC	RAM_AC,F,BANKED		;

	MOVLW	0X5A			;
	ADDWFC	RAM_AB,F,BANKED		;

	RETURN				;
;---------------------------------------
sub_0B67:
	MOVF	RAM_A6,W,BANKED		;
	ANDWF	RAM_AA,W,BANKED		;
	MOVWF	RAM_BE,BANKED		;

	MOVF	RAM_A6,W,BANKED		;
	IORWF	RAM_AA,W,BANKED		;
	ANDWF	RAM_B2,W,BANKED		;
	IORWF	RAM_BE,W,BANKED		;
	ADDWF	RAM_AE,F,BANKED		;
;
	MOVF	RAM_A5,W,BANKED		;
	ANDWF	RAM_A9,W,BANKED		;
	MOVWF	RAM_BE,BANKED		;

	MOVF	RAM_A5,W,BANKED		;
	IORWF	RAM_A9,W,BANKED		;
	ANDWF	RAM_B1,W,BANKED		;
	IORWF	RAM_BE,W,BANKED		;
	ADDWFC	RAM_AD,F,BANKED		;
;
	MOVF	RAM_A4,W,BANKED		;
	ANDWF	RAM_A8,W,BANKED		;
	MOVWF	RAM_BE,BANKED		;

	MOVF	RAM_A4,W,BANKED		;
	IORWF	RAM_A8,W,BANKED		;
	ANDWF	RAM_B0,W,BANKED		;
	IORWF	RAM_BE,W,BANKED		;
	ADDWFC	RAM_AC,F,BANKED		;
;
	MOVF	RAM_A3,W,BANKED		;
	ANDWF	RAM_A7,W,BANKED		;
	MOVWF	RAM_BE,BANKED		;

	MOVF	RAM_A3,W,BANKED		;
	IORWF	RAM_A7,W,BANKED		;
	ANDWF	RAM_AF,W,BANKED		;
	IORWF	RAM_BE,W,BANKED		;
	ADDWFC	RAM_AB,F,BANKED		;
;
	RETURN				;
;---------------------------------------
sub_0BD9:
	CALL	sub_0BB0		;
	
	MOVLW	0XA1			;
	ADDWF	RAM_AE,F,BANKED		;

	MOVLW	0XEB			;
	ADDWFC	RAM_AD,F,BANKED		;

	MOVLW	0XD9			;
	ADDWFC	RAM_AC,F,BANKED		;

	MOVLW	0X6E			;
	ADDWFC	RAM_AB,F,BANKED		;

	RETURN				;
;---------------------------------------
sub_0BF5:
	CALL	sub_0BB0		;
	
	MOVLW	0X24			;
	ADDWF	RAM_AE,F,BANKED		;

	MOVLW	0XD1			;
	ADDWFC	RAM_AD,F,BANKED		;

	MOVLW	0X4D			;
	ADDWFC	RAM_AC,F,BANKED		;

	MOVLW	0X5C			;
	ADDWFC	RAM_AB,F,BANKED		;

	RETURN				;
;---------------------------------------
sub_0BB0:
	MOVF	RAM_B2,W,BANKED		;
	XORWF	RAM_A6,W,BANKED		;
	XORWF	RAM_AA,W,BANKED		;
	ADDWF	RAM_AE,F,BANKED		;

	MOVF	RAM_B1,W,BANKED		;
	XORWF	RAM_A5,W,BANKED		;
	XORWF	RAM_A9,W,BANKED		;
	ADDWFC	RAM_AD,F,BANKED		;

	MOVF	RAM_B0,W,BANKED		;
	XORWF	RAM_A4,W,BANKED		;
	XORWF	RAM_A8,W,BANKED		;
	ADDWFC	RAM_AC,F,BANKED		;

	MOVF	RAM_AF,W,BANKED		;
	XORWF	RAM_A3,W,BANKED		;
	XORWF	RAM_A7,W,BANKED		;
	ADDWFC	RAM_AB,F,BANKED		;

	RETURN				;
;---------------------------------------
sub_09FC:
	MOVF	RAM_AE,W,BANKED		;
	MOVWF	Temp_0,A		;

	MOVF	RAM_AA,W,BANKED		;
	MOVWF	RAM_AE,BANKED		;

	MOVF	RAM_A6,W,BANKED		;
	MOVWF	RAM_AA,BANKED		;

	MOVF	RAM_B2,W,BANKED		;
	MOVWF	RAM_A6,BANKED		;

	MOVF	RAM_AB,W,BANKED		;
	MOVWF	RAM_B2,BANKED		;

	MOVF	RAM_A7,W,BANKED		;
	MOVWF	RAM_AB,BANKED		;

	MOVF	RAM_A3,W,BANKED		;
	MOVWF	RAM_A7,BANKED		;

	MOVF	RAM_AF,W,BANKED		;
	MOVWF	RAM_A3,BANKED		;

	MOVF	RAM_AC,W,BANKED		;
	MOVWF	RAM_AF,BANKED		;

	MOVF	RAM_A8,W,BANKED		;
	MOVWF	RAM_AC,BANKED		;

	MOVF	RAM_A4,W,BANKED		;
	MOVWF	RAM_A8,BANKED		;

	MOVF	RAM_B0,W,BANKED		;
	MOVWF	RAM_A4,BANKED		;

	MOVF	RAM_AD,W,BANKED		;
	MOVWF	RAM_B0,BANKED		;

	MOVF	RAM_A9,W,BANKED		;
	MOVWF	RAM_AD,BANKED		;

	MOVF	RAM_A5,W,BANKED		;
	MOVWF	RAM_A9,BANKED		;

	MOVF	RAM_B1,W,BANKED		;
	MOVWF	RAM_A5,BANKED		;

	MOVF	Temp_0,W,A		;
	MOVWF	RAM_B1,BANKED		;

	RETURN				;
;---------------------------------------
sub_0A3D:
	MOVF	RAM_AD,W,BANKED		;   RAM_A9-->RAM_AD-->RAM_AF-->RAM_A3-->RAM_A7
	MOVWF	Temp_0,A		;-->RAM_AB-->RAM_B1-->RAM_A5-->RAM_A9

	MOVF	RAM_A9,W,BANKED		;
	MOVWF	RAM_AD,BANKED		;

	MOVF	RAM_A5,W,BANKED		;
	MOVWF	RAM_A9,BANKED		;

	MOVF	RAM_B1,W,BANKED		;
	MOVWF	RAM_A5,BANKED		;

	MOVF	RAM_AB,W,BANKED		;
	MOVWF	RAM_B1,BANKED		;

	MOVF	RAM_A7,W,BANKED		;
	MOVWF	RAM_AB,BANKED		;

	MOVF	RAM_A3,W,BANKED		;
	MOVWF	RAM_A7,BANKED		;

	MOVF	RAM_AF,W,BANKED		;
	MOVWF	RAM_A3,BANKED		;

	MOVF	Temp_0,W,A		;
	MOVWF	RAM_AF,BANKED		;
;
	MOVF	RAM_AE,W,BANKED		;
	MOVWF	Temp_0,A		;

	MOVF	RAM_AA,W,BANKED		;
	MOVWF	RAM_AE,BANKED		;

	MOVF	RAM_A6,W,BANKED		;
	MOVWF	RAM_AA,BANKED		;

	MOVF	RAM_B2,W,BANKED		;
	MOVWF	RAM_A6,BANKED		;

	MOVF	RAM_AC,W,BANKED		;
	MOVWF	RAM_B2,BANKED		;

	MOVF	RAM_A8,W,BANKED		;
	MOVWF	RAM_AC,BANKED		;

	MOVF	RAM_A4,W,BANKED		;
	MOVWF	RAM_A8,BANKED		;

	MOVF	RAM_B0,W,BANKED		;
	MOVWF	RAM_A4,BANKED		;

	MOVF	Temp_0,W,A		;
	MOVWF	RAM_B0,BANKED		;

	RETURN				;
;---------------------------------------
;$82
sub_0A7E:
	CALL	sub_09F0		;
	CALL	sub_09F0		;
	CALL	sub_09F0		;
	CALL	sub_09FC		;
	RETURN				;
;---------------------------------------
;$8F
sub_0A8B:
	CALL	sub_09F0		;
	CALL	sub_09F0		;
	CALL	sub_09FC		;
	RETURN				;
;---------------------------------------
;$99
sub_0A95:
	CALL	sub_09F0		;
	CALL	sub_09FC		;
	RETURN				;
;---------------------------------------
;$A0
sub_0A9C:
	CALL	sub_09E3		;
	CALL	sub_09FC		;
	RETURN				;
;---------------------------------------
;$A7
sub_0AA3:
	CALL	sub_09E3		;
	CALL	sub_09E3		;
	CALL	sub_09E3		;
	CALL	sub_09FC		;
	RETURN				;
;---------------------------------------
;$B4
sub_0AB0:
	CALL	sub_09E3		;
	CALL	sub_09E3		;
	CALL	sub_09E3		;
	CALL	sub_09E3		;
	CALL	sub_09FC		;
	RETURN				;
;---------------------------------------
;$C4
sub_0AC0:
	CALL	sub_09F0		;
	CALL	sub_09F0		;
	CALL	sub_09F0		;
	CALL	sub_0A3D		;
	RETURN				;
;---------------------------------------
;$D1
sub_0ACD:
	CALL	sub_09F0		;
	CALL	sub_09F0		;
	CALL	sub_0A3D		;
	RETURN				;
;---------------------------------------
;$DB
sub_0AD7:
	CALL	sub_09F0		;
	CALL	sub_0A3D		;
	RETURN				;
;---------------------------------------


;---------------------------------------
sub_09E3:				;all shift left 1
	RLCF	RAM_AB,W,BANKED		;
;	BTG	STATUS,C,A		;
;	MOVF	RAM_AB,W,BANKED		;
;	SUBLW	0X7F			;
	RLCF	RAM_AE,F,BANKED		;
	RLCF	RAM_AD,F,BANKED		;
	RLCF	RAM_AC,F,BANKED		;
	RLCF	RAM_AB,F,BANKED		;
	RETURN				;
;---------------------------------------
sub_09F0:				;all shift right 1
	RRCF	RAM_AE,W,BANKED		;
	RRCF	RAM_AB,F,BANKED		;
	RRCF	RAM_AC,F,BANKED		;
	RRCF	RAM_AD,F,BANKED		;
	RRCF	RAM_AE,F,BANKED		;
	RETURN				;
;---------------------------------------
sub_11CF:
	MOVLW	0X03			;
	ANDWF	RAM_C2,W,BANKED		;
	ADDLW	0X90			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVWF	RAM_86,BANKED		;
;	CALL	EE_WRITE		;write 'FF'
	MOVLW	0X93			;
	CALL	sub_0C11_HashCalc_byWisAddr		;
;
	MOVLW	0X09			;
	MOVWF	X_P,A			;
	LFSR	FSR0,0X87		;RAM_87
	CALL	sub_1B57_ReadEEorI2C_addrBYX_P_len8		;
;
	MOVLW	0X08			;
	MOVWF	Temp_0,A		;
	MOVLW	0X01			;
	MOVWF	ADDRESS,A		;
	LFSR	FSR0,0X93		;RAM_93
	LFSR	FSR1,0X8B		;RAM_8B
adr_11D0:
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	XORWF	POSTINC0,W,A		;
	MOVWF	POSTINC1,A		;

	INCF	ADDRESS,F,A		;
	DECFSZ	Temp_0,F,A		;
	BRA	adr_11D0		;
;
	MOVLW	0X00			;
	BTFSC	RAM_CA_4,BANKED		;
	BRA	adr_11F8		;

;	MOVLW	0X06			;
	CALL	sub_1ED3_LoadPSWto8b_92_formatBYW		;

adr_11F8:
	IORWF	RAM_93,W,BANKED		;
	XORWF	RAM_8B,F,BANKED		;

	MOVF	RAM_86,W,BANKED		;
	XORWF	RAM_91,F,BANKED		;

	MOVF	RAM_85,W,BANKED		;
	XORWF	RAM_92,F,BANKED		;
;
	MOVLW	0X03			;
	ANDWF	RAM_C2,W,BANKED		;
	ADDLW	0X90			;
	MOVWF	ADDRESS,A		;
	INCF	RAM_86,W,BANKED		;
	MOVWF	TEMP_EE,A		;
;
	MOVLW	0X8B			;
	CALL	sub_0C11_HashCalc_byWisAddr		;cal OVER write

;	CALL	EE_WRITE		;
	CALL	IIC_WRITE		;

;
	BTFSS	RAM_CA_4,BANKED		;
	BRA	adr_121B		;

	MOVLW	0X08			;
	MOVWF	Temp_0,A		;
	LFSR	FSR0,0X93		;RAM_93
	LFSR	FSR1,0X8B		;RAM_8B
adr_1212:
	MOVF	POSTINC0,W,A		;
	XORWF	POSTINC1,F,A		;

	DECFSZ	Temp_0,F,A		;
	BRA	adr_1212		;

adr_121B:
	RETURN				;
;---------------------------------------
sub_1ED3_LoadPSWto8b_92_formatBYW:;W传入参数,把01~09的密码读入到8B+W到92+W中，然后把从93开始的W个数放到8B中，倒序：93->8B,92->8a
	MOVWF	RAM_BE,BANKED		;

	ADDLW	0x8B			;RAM_8B +
	MOVWF	FSR0L,A			;
	CLRF	FSR0H,A			;

	MOVLW	0X01			;从EE的01地址开始读取8个数据到:
	MOVWF	X_P,A			;RAM_8B+,RAM_8C,RAM_8D,RAM_8E,
	CALL	sub_1B57_ReadEEorI2C_addrBYX_P_len8		;RAM_8F,RAM_90,RAM_91,RAM_92,
;
	MOVF	RAM_BE,W,BANKED		;
	ADDLW	0x92			;RAM_92 +
	MOVWF	FSR1L,A			;
	CLRF	FSR1H,A			;

	MOVF	RAM_BE,W,BANKED		;
	ADDLW	0x8A			;RAM_8A +
	MOVWF	FSR2L,A			;
	CLRF	FSR2H,A			;

	MOVF	RAM_BE,W,BANKED		;
	MOVWF	Temp_0,A		;
;
adr_1EDE:
	MOVF	POSTDEC1,W,A		;
	MOVWF	POSTDEC2,A		;(RAM_92 +)-->(RAM_8A +)
	DECFSZ	Temp_0,F,A		;
	BRA	adr_1EDE		;

	RETURN				;
;---------------------------------------
sub_1EE6:
	MOVLW	.4			;
	MOVWF	Temp_0,A		;
	LFSR	FSR0,0x8B		;RAM_8B,RAM_8C,RAM_8D,RAM_8E,
	MOVLW	0X09			;
	MOVWF	ADDRESS,A		;
adr_1EE8:
	CALL	EE_READ			;
	;;;;CALL	IIC_READ_byTempEE_Tempbb		;读取ID:8F9061CB，
	XORWF	POSTINC0,F,A		;
	INCF	ADDRESS,F,A		;
	DECFSZ	Temp_0,F,A		;
	BRA	adr_1EE8		;

	CALL	sub_1EFA_LoadI2C00_b76toW_b10		;
	XORWF	RAM_8F,F,BANKED		;
	RETURN				;
;---------------------------------------
sub_1EFA_LoadI2C00_b76toW_b10:;将I2C00地址的数据的高两位放到W中的低两位，如80->02
	MOVLW	0X00			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
	;CALL	IIC_READ_byTempEE_Tempbb		;
	MOVLW	0xC0			;
	ANDWF	TEMP_EE,F,A		;
	BCF	STATUS,C		;
	RLCF	TEMP_EE,F,A		;
	RLCF	TEMP_EE,F,A		;
	RLCF	TEMP_EE,F,A		;
	MOVF	TEMP_EE,W,A		;
	RETURN				;
;---------------------------------------
sub_1672_GetDataBlock_555f_4149_2d37:
	MOVLW	0xF3			;b'11110011'
	MOVWF	RAM_C9,BANKED ;C9中应为数据段有效标志位		;
	CALL	sub_1680_Check55and5fdata_reC5	;检查55数据及5f数据是否合规，若不合规，则使用73地址数据,使用的数据地址在C5中	;读取运算次数RAM_BF,RAM_D2,RAM_C2,RAM_C5,0x5F,0x55,0x73
	CALL	sub_16B8_CheckEE41andEE49_reC3		;return -->RAM_C3
	CALL	sub_1742_Check2Dand37data_reC4		;return -->RAM_C4

	RETURN				;
;---------------------------------------
;---------------------------------------
sub_1680_Check55and5fdata_reC5:
	CALL	sub_13EB_Get69to70_FirstNFF		;读取运算次数,RAM_D2,RAM_C2,RAM_BF
	MOVLW	0x73			;备用数据，以下两组都不合格时采用，
	MOVWF	RAM_C5,BANKED		;

	BSF	RAM_C9_0,BANKED		;
	BSF	RAM_C9_1,BANKED		;

	MOVLW	0x55			;检查当前EE地址之后的10个是否异或结果为0，
	CALL	sub_1BAF		;+8的EE不能大于7F.
	BNZ	adr_16A1		;

	BCF	RAM_C9_0,BANKED		;
;	MOVLW	0x5D			;
;	MOVWF	ADDRESS,A		;
;	CALL	EE_READ			;
	MOVF	Temp_2,W,A		;0x55+8,以上调用的时候已经读出，可用
	XORWF	RAM_D2,W,BANKED		;
	BNZ	adr_16A1		;

	BCF	RAM_D3_0,BANKED		;0x55开始的数据段匹配
	MOVLW	0x55			;
	MOVWF	RAM_C5,BANKED		;
adr_16A1:
	MOVLW	0x5F			;检查当前EE地址之后的10个是否异或结果为0，
	CALL	sub_1BAF		;+8的EE不能大于7F.
	BNZ	adr_16B7

	BCF	RAM_C9_1,BANKED		;
;	MOVLW	0x67			;
;	MOVWF	ADDRESS,A		;
;	CALL	EE_READ			;
	MOVF	Temp_2,W,A		;0x5F+8,以上调用的时候已经读出，可用
	XORWF	RAM_D2,W,BANKED		;
	BNZ	adr_16B7		;

	BSF	RAM_D3_0,BANKED		;0x5F开始的数据段匹配
	MOVLW	0x5F			;
	MOVWF	RAM_C5,BANKED		;
adr_16B7:
	RETURN				;
;---------------------------------------

;---------------------------------------
sub_16B8_CheckEE41andEE49_reC3:
	CLRF	RAM_C3,BANKED		;
	BSF	RAM_D3_1,BANKED		;
	BSF	RAM_C9_4,BANKED		;
	BSF	RAM_C9_5,BANKED		;

	MOVLW	0x53			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVWF	Temp_0,A		;
	MOVLW	0x49			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVWF	Temp_1,A		;

	MOVLW	0x41			;检查当前EE地址之后的10个是否异或结果为0，
	CALL	sub_1BAF		;+8的EE不能大于7F.
	BNZ	adr_16E2		;

	BCF	RAM_D3_1,BANKED		;0x41开始的数据段匹配

	BCF	RAM_C9_4,BANKED		;
	MOVF	Temp_1,W,A		;EE_49
	BNZ	adr_16E2		;

	MOVLW	0x4B			;检查当前EE地址之后的10个是否异或结果为0，
	CALL	sub_1BAF		;+8的EE不能大于7F.
	BNZ	adr_1704		;

	BCF	RAM_C9_5,BANKED		;
	MOVLW	0x1F			;
	XORWF	Temp_0,W,A		;EE_53
	BZ	adr_1704		;
	BRA	adr_170B		;

adr_16E2:
	MOVLW	0x4B			;
	CALL	sub_1BAF		;
	BNZ	adr_1704		;

	BCF	RAM_C9_5,BANKED		;

	BTFSC	RAM_D3_1,BANKED		;
	BRA	adr_170B		;

	MOVF	Temp_0,W,A		;EE_53
	BNZ	adr_16FC		;

	MOVLW	0x1F			;
	XORWF	Temp_1,W,A		;EE_49
	BNZ	adr_1704		;
	BRA	adr_170B		;
adr_16FC:
	MOVF	Temp_1,W,A		;EE_49
	SUBWF	Temp_0,W,A		;EE_53
	BC	adr_170B		;;;
;---
adr_1704:
	BCF	RAM_D3_1,BANKED		;0x41开始的数据段匹配
	MOVF	Temp_1,W,A		;EE_49
	MOVWF	X_P,A			;
	BRA	adr_1710		;
;---
adr_170B:
	BSF	RAM_D3_1,BANKED		;0x4B开始的数据段匹配
	MOVF	Temp_0,W,A		;EE_53
	MOVWF	X_P,A			;
;---
adr_1710:
	COMF	RAM_C9,W,BANKED		;
	ANDLW	0x30			;b'00110000'
	BNZ	adr_173F		;

	MOVF	Temp_1,W,A		;EE_49
	MOVWF	RAM_D4,BANKED		;
	BNZ	adr_1725		;

	MOVF	Temp_0,W,A		;EE_53
	MOVWF	X_P,A			;
	XORLW	0x1F			;
	BZ	adr_173F		;
adr_1725:
	MOVF	Temp_0,W,A		;EE_53
	MOVWF	X_P,A			;
	BNZ	adr_1733		;

	MOVF	RAM_D4,W,BANKED		;
	MOVWF	X_P,A			;
	MOVLW	0x1F			;
	SUBWF	X_P,W,A			;
	BZ	adr_173F		;

	MOVF	Temp_0,W,A		;EE_53
	MOVWF	X_P,A			;
adr_1733:
	MOVF	RAM_D4,W,BANKED		;
	SUBWF	X_P,W,A			;
	BZ	adr_1734		;
	BC	adr_173C		;Branch if Higher
;
adr_1734:
	MOVF	Temp_0,W,A		;EE_53
	MOVWF	X_P,A			;
	BRA	adr_173F		;
;
adr_173C:
	MOVF	Temp_1,W,A		;EE_49
	MOVWF	X_P,A			;
;
adr_173F:
	MOVF	X_P,W,A			;
	MOVWF	RAM_C3,BANKED		;

	RETURN				;
;---------------------------------------
sub_1742_Check2Dand37data_reC4:
	CLRF	RAM_C4,BANKED		;
	BSF	RAM_D3_2,BANKED		;
	BSF	RAM_C9_6,BANKED		;
	BSF	RAM_C9_7,BANKED		;

	MOVLW	0x35			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVWF	Temp_0,A		;EE_35
	MOVLW	0x3F			;
	MOVWF	ADDRESS,A		;
	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVWF	Temp_1,A		;EE_3F

	MOVLW	0x2D			;检查当前EE地址之后的10个是否异或结果为0，
	CALL	sub_1BAF		;+8的EE不能大于7F.
	BNZ	adr_175A		;

	BCF	RAM_D3_2,BANKED		;
	BCF	RAM_C9_6,BANKED		;

	MOVF	Temp_0,W,A		;EE_35
	MOVWF	RAM_C4,BANKED		;
adr_175A:
	MOVLW	0x37			;
	CALL	sub_1BAF		;
	BNZ	adr_176E		;

	BCF	RAM_C9_7,BANKED		;
	MOVF	RAM_C4,W,BANKED		;
	SUBWF	Temp_1,W,A		;EE_3F
	BNC	adr_176E		;;;;

	MOVF	Temp_1,W,A		;EE_3F
	MOVWF	RAM_C4,BANKED		;
	BSF	RAM_D3_2,BANKED		;
adr_176E:
	MOVLW	0xC0			;
	ANDWF	RAM_C9,W,BANKED		;
	SUBLW	0xC0			;
	BNZ	adr_1783		;

	MOVF	Temp_0,W,A		;EE_35
	MOVWF	X_P,A			;
	MOVF	Temp_1,W,A		;EE_3F
	SUBWF	Temp_0,W,A		;EE_35

	BNC	adr_1781		;;;

	MOVF	Temp_1,W,A		;EE_3F
	MOVWF	X_P,A			;
adr_1781:
	MOVF	X_P,W,A			;
	MOVWF	RAM_C4,BANKED		;
adr_1783:
	RETURN				;
;---------------------------------------
sub_1CD1_RFTx_00_00_00_80_21_00___Len18:
	BSF	RF_EN			;
	MOVLW	0x00			;
	MOVWF	RAM_81,BANKED		;
	MOVWF	RAM_82,BANKED		;
	MOVWF	RAM_83,BANKED		;
	MOVWF	RAM_84,BANKED		;
	BSF	RAM_84,7,BANKED		;

	MOVLW	.18			;
	MOVWF	CNT0,A			;
	LFSR	FSR0,0X81		;RAM_81
	CALL	sub_1D11_TxDataBuf		;

adr_1CE3:
;	CLRWDT				;
	BTFSS	PIR1,CCP1IF		;
	BRA	adr_1CE3		;



	MOVF	PORTC,W,A		;
	ANDLW	B'11000111'		;
	IORLW	B'00101000'		;
	MOVWF	PORTC,A			;
	NOP				;
	NOP				;
	BCF	PIR1,CCP1IF		;
	MOVLW	B'00000000'		;
	MOVWF	CCP1CON,A		;
	NOP				;
	NOP				;
	MOVF	PORTC,W,A		;
	ANDLW	B'11000111'		;
	IORLW	B'00011000'		;
	MOVWF	PORTC,A			;

	BCF	RF_EN			;

	RETURN				;
;---------------------------------------
TX_IR_A_Tx_88_88_88_88_08______len18:
;	MOVLW	0x10			;
;	BTFSC	f20_power,A		;
;	BRA	adr_1D04_TxFrame_lenBYX_P_buf80		;
;	CALL	INI_BJQ			;

	MOVLW	0x88			;
	MOVWF	RAM_80,BANKED		;
	MOVWF	RAM_81,BANKED		;
	MOVWF	RAM_82,BANKED		;
	MOVWF	RAM_83,BANKED		;
	MOVLW	0x08			;
	MOVWF	RAM_84,BANKED		;

	MOVLW	.18			;
	MOVWF	X_P,A			;
;
adr_1D04_TxFrame_lenBYX_P_buf80:
	INCF	X_P,W,A			;
	MOVWF	CNT0,A			;
	LFSR	FSR0,0x80		;RAM_80
;
	return
	
	
	MOVLW	B'00001010'		;
	MOVWF	CCP1CON,A		;
	MOVLW	high(TE_100)		;
	MOVWF	CCPR1H,A		;
	MOVLW	low(TE_100)		;
	MOVWF	CCPR1L,A		;
	CALL	ON_CCP			;
;
	CALL	sub_1D11_TxDataBuf		;
	CALL	sub_1D91_TxNibbleByte_BUF_B		;

	CLRF	CCP1CON,A		;

	RETURN				;
;---------------------------------------
TX_IR_B_10_data81_LenBYX_P_add1:
	MOVLW	0x10			;
	MOVWF	RAM_80,BANKED		;
	BRA	adr_1D04_TxFrame_lenBYX_P_buf80		;
;---------------------------------------
TX_IR_S:
	BTFSS	RAM_CD,1,BANKED		;
	BSF	RAM_CB,6,BANKED		;

	MOVLW	0x70			;
	MOVWF	RAM_BF,BANKED		;
	MOVLW	0x02			;
	MOVWF	RAM_C0,BANKED		;

	MOVLW	B'00001010'		;
	MOVWF	CCP1CON,A		;
	MOVLW	high(TE_100)		;
	MOVWF	CCPR1H,A		;
	MOVLW	low(TE_100)		;
	MOVWF	CCPR1L,A		;
	CALL	ON_CCP			;
adr_122C:
	MOVLW	0x30			;
	MOVWF	BUF_A,A			;
	CALL	sub_1D80_IRTxByte		;
	CALL	sub_1D91_TxNibbleByte_BUF_B		;

	CALL	sub_11B7_KeyQuDou		;
	BNZ	TX_IR_S_END		;

	MOVLW	B'00001010'		;
	MOVWF	CCP1CON,A		;
	MOVLW	high(TE_16600)		;
	MOVWF	CCPR1H,A		;
	MOVLW	low(TE_16600)		;
	MOVWF	CCPR1L,A		;
	CALL	ON_CCP			;

	CALL	sub_150C		;
	BC	adr_122C		;

TX_IR_S_END:
	RETURN				;
;---------------------------------------
sub_1D11_TxDataBuf:
;


;
	BTFSS	RF_EN			;
	BRA	adr_1D35		;

	CALL	INI_BJQ			;
	CALL	ON_CCP			;

	CALL	sub_1D42_TxByte0		;
	CALL	sub_1D42_TxByte0		;
	CALL	sub_1D42_TxByte0		;

	CALL	sub_1D42_TxByte0		;
	CALL	sub_1D42_TxByte0		;
	CALL	sub_1D42_TxByte0		;

	CALL	sub_1D42_TxByte0		;
	CALL	sub_1D42_TxByte0		;
	CALL	sub_1D42_TxByte0		;
adr_1D35:
	MOVF	POSTINC0,W,A		;
	CALL	sub_1D43_TxByte_byW		;

	DECFSZ	CNT0,F,A		;
	BRA	adr_1D35		;

	RETURN				;
;---------------------------------------
sub_1D42_TxByte0:
	MOVLW	0X00			;
sub_1D43_TxByte_byW:
	MOVWF	BUF_A,A			;

	BTFSS	RF_EN			;
	BRA	TX_IR			;

	CALL	sub_1D4D_RFTXByte		;
	RETURN				;
;
TX_IR:
	CALL	sub_1D80_IRTxByte		;
	RETURN				;
;---------------------------------------
sub_1D4D_RFTXByte:
	MOVLW	.8			;
	MOVWF	CNT1,A			;
RFsendLP:
	MOVF	PORTC,W,A		;
	ANDLW	B'11000111'		;
	MOVWF	BUF_0,A			;

	RRCF	BUF_A,F,A		;
	MOVLW	B'00011000'		;0
	BTFSC	STATUS,C,A		;
	MOVLW	B'00101000'		;1
	IORWF	BUF_0,W,A		;
adr_1D5B:
;	CLRWDT				;
	BTFSS	PIR1,CCP1IF		;
	BRA	adr_1D5B		;
	MOVWF	PORTC,A			;

	CALL	ON_CCP			;
	MOVLW	B'00110000'		;
	XORWF	PORTC,W,A		;
adr_1D67:
	CLRWDT				;
	BTFSS	PIR1,CCP1IF		;
	BRA	adr_1D67		;
	MOVWF	PORTC,A			;

	CALL	ON_CCP			;

	DECFSZ	CNT1,F,A		;
	BRA	RFsendLP		;

;	MOVLW	0x22			;
;	XORWF	RAM_85,W,BANKED		;
;	BNZ	adr_1D7F		;
;
;	BTFSC	SW_3,A			;S3
;	RESET				;

adr_1D7F:
	RETURN				;
;
ON_CCP:
	RETURN
	
	
	BCF	T3CON,TMR3ON		;
;	CLRF	TMR3L,A			;
	MOVLW	.3			;
	MOVWF	TMR3L,A			;
	CLRF	TMR3H,A			;

	BSF	T3CON,TMR3ON		;
	BCF	PIR1,CCP1IF		;
	RETURN				;

INI_BJQ:
	MOVLW	B'00001010'		;
	MOVWF	CCP1CON,A		;
	MOVLW	high(TE_500)		;0x01
	MOVWF	CCPR1H,A		;
	MOVLW	low(TE_500)		;0xF4
	MOVWF	CCPR1L,A		;
	RETURN				;
;---------------------------------------
sub_1D80_IRTxByte:
	MOVLW	0X0F			;
	ANDWF	BUF_A,W,A		;
	MOVWF	BUF_B,A			;
	CALL	sub_1D91_TxNibbleByte_BUF_B		;

	SWAPF	BUF_A,W,A		;
	ANDLW	0X0F			;
	MOVWF	BUF_B,A			;
	CALL	sub_1D91_TxNibbleByte_BUF_B		;

	RETURN				;
;---------------------------------------
sub_1D91_TxNibbleByte_BUF_B:
adr_1D97:
;	CLRWDT				;
	BTFSS	PIR1,CCP1IF		;
	BRA	adr_1D97		;

	BTFSS	f20_power,A		;
	BRA	ON_BAT			;

	CALL	sub_1DA1		;
	RETURN				;
;
ON_BAT:
	CALL	sub_1DB8		;
	RETURN				;
;---------------------------------------
;车载供电时
sub_1DA1:
	BCF	IR_B			;
	CALL	DELAY_30US		;
	BSF	IR_B			;

	MOVLW	0x40			;
	MULWF	BUF_B,A			;
	MOVF	PRODH,W			;
	MOVWF	CCPR1H,A		;
	MOVF	PRODL,W			;
	MOVWF	CCPR1L,A		;

	MOVLW	0xE2			;
	ADDWF	CCPR1L,F,A		;
	MOVLW	0x03			;
	ADDWFC	CCPR1H,F,A		;
	MOVLW	B'00001010'		;
	MOVWF	CCP1CON,A		;
	CALL	ON_CCP_IRB		;

	RETURN				;

ON_CCP_IRB:
	BCF	T3CON,TMR3ON		;
;	CLRF	TMR3L,A			;
	MOVLW	.9			;
	MOVWF	TMR3L,A			;
	CLRF	TMR3H,A			;

	BSF	T3CON,TMR3ON		;
	BCF	PIR1,CCP1IF		;
	RETURN				;
;---------------------------------------
;电池供电时
sub_1DB8:
	MOVLW	.7			;
	MOVWF	CNT1,A			;

	MOVF	PORTC,W,A		;
	MOVWF	BUF_0,A			;
	MOVLW	B'00000001'		;
	XORWF	BUF_0,W,A		;
	MOVWF	BUF_1,A			;
	BTFSC	RAM_CB,6,BANKED		;
	BRA	sub_1DB8_1		;
	BRA	sub_1DB8_0		;
sub_1DB8_0:
	CALL	sub_1DD8		;
	BRA	sub_1DB8_2		;
sub_1DB8_1:
	CALL	sub_1DE5		;
;
sub_1DB8_2:

	MOVLW	0X80			;128us
	MULWF	BUF_B,A			;
	MOVF	PRODH,W			;
	MOVWF	CCPR1H,A		;
	MOVF	PRODL,W			;
	MOVWF	CCPR1L,A		;

	BCF	STATUS,C,A		;
	RLCF	CCPR1L,F,A		;*2
	RLCF	CCPR1H,F,A		;

	MOVLW	0x88			;
	ADDWF	CCPR1L,F,A		;
	MOVLW	0x04			;1160us
	ADDWFC	CCPR1H,F,A		;
	MOVLW	B'00001010'		;
	MOVWF	CCP1CON,A		;
	CALL	ON_CCP_IRA		;

	RETURN				;

ON_CCP_IRA:
	BCF	T3CON,TMR3ON		;
;	CLRF	TMR3L,A			;
	MOVLW	.12			;
	BTFSC	RAM_CB,6,BANKED		;
	MOVLW	.32			;
	MOVWF	TMR3L,A			;
	CLRF	TMR3H,A			;

	BSF	T3CON,TMR3ON		;
	BCF	PIR1,CCP1IF		;
	RETURN				;
;---------------------------------------
sub_1DD8:
LP_HDa:
	MOVF	BUF_1,W,A		;
	MOVWF	PORTC,A			;
	CALL	DELAY_U			;

	MOVF	BUF_0,W,A		;
	MOVWF	PORTC,A			;
	CALL	DELAY_U			;
	CALL	DELAY_U			;

	DECFSZ	CNT1,F,A		;
	BRA	LP_HDa			;

	RETURN				;
;---------------------------------------
sub_1DE5:
	DECF	CNT1,F,A		;
LP_HDa2:
	MOVF	BUF_1,W,A		;
	MOVWF	PORTC,A			;
	CALL	DELAY_U4		;

	MOVF	BUF_0,W,A		;
	MOVWF	PORTC,A			;
	CALL	DELAY_U			;
	CALL	DELAY_U			;
	CALL	DELAY_U			;
	CALL	DELAY_U4		;

	DECFSZ	CNT1,F,A		;
	BRA	LP_HDa2			;

	RETURN				;
;---------------------------------------
DELAY_U:
	CLRWDT				;
	NOP				;
	NOP				;
	NOP				;
DELAY_U4

	NOP				;
	NOP				;

	RETURN				;
;---------------------------------------
sub_1442_USEDEC:
	CALL	sub_13EB_Get69to70_FirstNFF;获得I2C中69到71第一个非FF的内容,内容在C2中，D2中为其&FC，BF中为其&03;
	DECF	RAM_C2,W,BANKED		;0x69起始的数据段进行减1操作
	BNN	adr_144E		;不为负则跳转,也就是值从7F递减到00时，置7F

	MOVLW	0x7F			;
adr_144E:
	MOVWF	TEMP_EE,A		;

	DECF	RAM_D4,W,BANKED		;存储有效数据的指针,用于减1操作时写回数据
	BNZ	adr_1457		;
	MOVLW	0x08			;存储指针递减，到00时置08
adr_1457:
	ADDLW	0x68			;
	MOVWF	ADDRESS,A		;
	CALL	EE_WRITE		;
;	CALL	IIC_WRITE		;

	MOVLW	0xFF			;
	MOVWF	TEMP_EE,A		;
	MOVF	RAM_D4,W,BANKED		;
	ADDLW	0x68			;
	MOVWF	ADDRESS,A		;
	CALL	EE_WRITE		;
;	CALL	IIC_WRITE		;

	RETURN				;
;---------------------------------------



;---------------------------------------
sub_1468_5F_73_69_I2CaddrProc:
;	BSF	f20_RDee,A		;

	CALL	sub_16B8_CheckEE41andEE49_reC3		;
	
	MOVLW	0x41			;0:0x41开始的数据段匹配
	BTFSC	RAM_D3_1,BANKED		;
	MOVLW	0x4B			;
	MOVWF	X_P,A			;
	CALL	sub_1B48_Load8EEto93to9A_byX_P		;读出需要修正段的前8个数据

	MOVLW	0x5D			;
	MOVWF	ADDRESS,A		;
	;CALL	IIC_READ_byTempEE_Tempbb		;
	CALL	EE_READ			;

	MOVLW	0x03			;
	IORWF	TEMP_EE,W,A		;
	MOVWF	tmp_cL,A		;

	MOVLW	0x04			;
	SUBWF	TEMP_EE,W,A		;
	MOVWF	RAM_9B,BANKED		;
	MOVWF	RAM_D2,BANKED		;

	MOVWF	RAM_BF,BANKED		;
CAL_TTT:
	DECF	RAM_BF,F,BANKED		;
	BNC	CAL_OUT			;

	MOVLW	0x93			;
	CALL	sub_0C11_HashCalc_byWisAddr		;
	BRA	CAL_TTT			;
CAL_OUT:
;
	MOVLW	0x5F			;
	MOVWF	RAM_C5,BANKED		;
	CALL	sub_1B74_FanXie93to9A_D2_xorVer_addrC5		;
;-------
	MOVF	RAM_D2,W,BANKED		;
	SUBWF	tmp_cL,W,A		;

	MOVWF	RAM_BF,BANKED		;
CAL_TTT2:
	DECF	RAM_BF,F,BANKED		;
	BNC	CAL_OUT2		;

	MOVLW	0x93			;
	CALL	sub_0C11_HashCalc_byWisAddr		;
	BRA	CAL_TTT2		;
CAL_OUT2:

	MOVF	tmp_cL,W,A		;
	MOVWF	RAM_9B,BANKED		;
	MOVWF	RAM_D2,BANKED		;

	MOVLW	0x73			;
	MOVWF	RAM_C5,BANKED		;
	CALL	sub_1B74_FanXie93to9A_D2_xorVer_addrC5		;
;-------
	LFSR	FSR0,0X93		;RAM_93
	MOVLW	.8			;
	MOVWF	Temp_0,A		;
LP_CLR:
	MOVLW	0XFF			;
	MOVWF	POSTINC0,A		;
	DECFSZ	Temp_0,F,A		;
	BRA	LP_CLR			;
;
	LFSR	FSR0,0X93		;RAM_93
	MOVLW	0x08			;
	MOVWF	tmp_PT,A		;
	MOVLW	0x7F			;
	MOVWF	TEMP2,A			;
BJ_EQL:
	MOVF	TEMP2,W,A		;
	XORWF	tmp_cL,W,A		;
	BZ	CALptOut		;

	MOVF	POSTINC0,W,A		;
	DECF	TEMP2,F,A		;

	DECFSZ	tmp_PT,F,A		;
	BRA	BJ_EQL			;

	MOVLW	0x08			;
	MOVWF	tmp_PT,A		;
	LFSR	FSR0,0X93		;RAM_93
	BRA	BJ_EQL			;
;
CALptOut:
	MOVF	tmp_cL,W,A		;
	MOVWF	INDF0,A			;
;
	LFSR	FSR0,0x93		;RAM_93
	MOVLW	.8			;
	MOVWF	Temp_0,A		;
	MOVLW	0x69			;
	MOVWF	ADDRESS,A		;
	CALL	EE_WRITE_M		;
;
;	MOVLW	0x70			;
;	MOVWF	ADDRESS,A		;
;	MOVLW	0xFF			;
;	MOVWF	TEMP_EE,A		;
;;	CALL	EE_WRITE		;
;	CALL	IIC_WRITE		;

;	MOVLW	0x71			;
;	MOVWF	ADDRESS,A		;
;;	CALL	EE_WRITE		;
;	CALL	IIC_WRITE		;

;	MOVLW	0x7D			;
;	MOVWF	ADDRESS,A		;
;;	CALL	EE_WRITE		;
;	CALL	IIC_WRITE		;
;	MOVLW	0x7E			;
;	MOVWF	ADDRESS,A		;
;;	CALL	EE_WRITE		;
;	CALL	IIC_WRITE		;
;	MOVLW	0x7F			;
;	MOVWF	ADDRESS,A		;
;;	CALL	EE_WRITE		;
;	CALL	IIC_WRITE		;
	RETURN				;
;---------------
	MOVLW	0x7F			;
	MOVWF	RAM_9A,BANKED		;

	MOVLW	.7			;
	MOVWF	Temp_0,A		;
	LFSR	FSR0,0x93		;RAM_93
adr_1470:
	MOVLW	0xFF			;
	MOVWF	POSTINC0,A		;

	DECFSZ	Temp_0,F,A		;
	BRA	adr_1470		;

	MOVLW	0xFF			;
	MOVWF	RAM_D2,BANKED		;
	MOVLW	0x69			;EE的69单元
	MOVWF	RAM_C5,BANKED		;
	CALL	sub_1B74_FanXie93to9A_D2_xorVer_addrC5		;
	MOVLW	0x68			;
	CALL	EE_WT_CHK_AddrBYW_FFor0		;
	MOVLW	0x72			;
	CALL	EE_WT_CHK_AddrBYW_FFor0		;上一个写10个数，最后一个通过此段写FF
	MOVLW	0x7C			;
	CALL	EE_WT_CHK_AddrBYW_FFor0		;
	RETURN				;
;---------------------------------------
CHK_VOL:
	BSF	LED			;
;WAIT_ST:
;	CLRWDT				;
;	BTFSS	VREFCON0,FVR1ST,A	;
;	BRA	WAIT_ST			;

	BSF	ADCON0,ADON,A		;
	BSF	ADCON0,GO,A		;
WAIT_AD:
;	CLRWDT				;
	BTFSC	ADCON0,GO_DONE,A	;
	BRA	WAIT_AD			;

	MOVF	ADRESH,W,A		;
	CALL	CHG_AD			;
	MOVWF	RAM_84,BANKED		;
	MOVWF	A_P,A			;

;	MOVF	ADRESH,W,A		;
;	MOVWF	Temp_0,A		;
;	MOVF	ADRESL,W,A		;
;	MOVWF	Temp_1,A		;

	BCF	ADCON0,ADON,A		;

;	MOVF	Temp_0,W,A		;
;	MOVWF	TEMP_EE,A		;
;	MOVLW	0xA0			;
;	MOVWF	ADDRESS,A		;
;	CALL	EE_WRITE		;

;	MOVF	Temp_1,W,A		;
;	MOVWF	TEMP_EE,A		;
;	MOVLW	0xA1			;
;	MOVWF	ADDRESS,A		;
;	CALL	EE_WRITE		;

	BTFSC	f20_power,A		;
	BRA	CHK_VOL_END		;

	MOVLW	0x95			;
	MOVWF	ADDRESS,A		;
;	CALL	EE_READ			;
	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVWF	X_P,A			;
	MOVF	X_P,F,A			;
	BNZ	adr_09BE		;

;	MOVLW	0x99			;
;	MOVWF	ADDRESS,A		;
;	CALL	EE_READ			;
	MOVLW	0x0A			;
	ADDWF	A_P,F,A			;
adr_09BE:
	MOVLW	0xFF			;
	MOVWF	X_P,A			;
;	MOVLW	0x98			;
;	MOVWF	ADDRESS,A		;
;	CALL	EE_READ			;
	MOVLW	0x81			;
	SUBWF	A_P,W,A			;
	BNC	adr_09C8		;>=2.19

	BCF	LED			;
	CLRF	X_P,A			;
adr_09C8:
	MOVLW	0x95			;
	MOVWF	ADDRESS,A		;
;	CALL	EE_READ			;
	CALL	IIC_READ_byTempEE_Tempbb		;
	XORWF	X_P,W,A			;
	BZ	adr_09D2		;

	MOVLW	0x95			;
	CALL	EE_WT_CHK_AddrBYW_FFor0		;
adr_09D2:
	MOVLW	B'00001010'		;
	MOVWF	CCP1CON,A		;
	MOVLW	high(TE_16000)		;
	MOVWF	CCPR1H,A		;
	MOVLW	low(TE_16000)		;
	MOVWF	CCPR1L,A		;
	CALL	ON_CCP			;

CHK_VOL_END:
;	CLRWDT				;
	BTFSS	PIR1,CCP1IF		; 
	BRA	CHK_VOL_END		;

	BCF	LED			;

	RETURN				;
;---------------------------------------
CHG_AD:
;	MOVLW	0x96			;
	MOVWF	Temp_0,A		;

	MOVLW	0x95			;
	SUBWF	Temp_0,W,A		;
	BNC	CHG_AD_0		;

	MOVLW	0xFE			;
	SUBWF	Temp_0,W,A		;
	BC	CHG_AD_1		;

	BRA	CHG_AD_2		;
CHG_AD_0:
	MOVLW	0x95			;
	MOVWF	Temp_0,A		;
	BRA	CHG_AD_2		;

CHG_AD_1:
	MOVLW	0xFB			;
	MOVWF	Temp_0,A		;
;	BRA	CHG_AD_2		;

CHG_AD_2:
	MOVLW	0xFF			;
	MOVWF	Temp_1,A		;
	MOVLW	0x95			;
	MOVWF	Temp_2,A		;
CHG_AD_3:
	MOVF	Temp_2,W,A		;
	SUBWF	Temp_0,W,A		;
	BNC	CHG_AD_4		;

	INCF	Temp_1,F,A		;
	MOVLW	0x03			;
	ADDWF	Temp_2,F,A		;
	BRA	CHG_AD_3		;
CHG_AD_4:
	MOVLW	0x64			;
	ADDWF	Temp_1,W,A		;
	RETURN				;
;---------------------------------------



;---------------------------------------
EE_WRITE_M:
EE_WRITE_M_LP:
	MOVF	POSTINC0,W,A		;
	MOVWF	TEMP_EE,A		;
	CALL	EE_WRITE		;
;	CALL	IIC_WRITE		;

	INCF	ADDRESS,F,A		;
	DECFSZ	Temp_0,F,A		;
	BRA	EE_WRITE_M_LP		;

	RETURN				;
;---------------------------------------
EE_WT_CHK_AddrBYW_FFor0:
	MOVWF	ADDRESS,A		;
	BCF	f21_eeFF,A		;

	CALL	EE_READ			;
;	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVLW	0xFF			;
	XORWF	TEMP_EE,W,A		;
;	BZ	EE_WT_CHK_AddrBYW_FFor0_0		;

	MOVLW	0x00			;
	BTFSS	STATUS,Z,A		;
	MOVLW	0xFF			;
	MOVWF	TEMP_EE,A		;
	CALL	EE_WRITE		;
;	CALL	IIC_WRITE		;

	RETURN				;

	BSF	f21_eeFF,A		;
	MOVLW	0xFF			;
	MOVWF	TEMP_EE,A		;
	CALL	EE_WRITE		;
;	CALL	IIC_WRITE		;

EE_WT_CHK_AddrBYW_FFor0_0:
	BTFSC	f21_eeFF,A		;
	BRA	EE_WT_CHK_AddrBYW_FFor0_1		;

	MOVLW	0x00			;
;	BTFSC	f21_eeFF,A		;
;	MOVLW	0xFF			;

	MOVWF	TEMP_EE,A		;
	CALL	EE_WRITE		;
;	CALL	IIC_WRITE		;
EE_WT_CHK_AddrBYW_FFor0_1:

	RETURN				;
;---------------------------------------
sub_1784:
	CALL	sub_17DC_waitRxFrame_cominA_P		;
	BTFSC	f21_over1,A		;
	BRA	adr_17B1		;

	MOVLW	0x02			;
	MOVWF	X_P,A			;
	MOVLW	0x0E			;
	XORWF	A_P,W,A			;
	BZ	adr_17A9		;

	MOVLW	0x03			;
	MOVWF	X_P,A			;
	MOVLW	0x00			;
	XORWF	A_P,W,A			;
	BZ	adr_17A9		;

	MOVLW	0x10			;
	MOVWF	X_P,A			;
	MOVLW	0x02			;
	XORWF	A_P,W,A			;
	BZ	adr_17A9		;
;
	MOVLW	0x10			;
	MOVWF	X_P,A			;
	MOVLW	0x35			;
	XORWF	A_P,W,A			;
	BZ	adr_17A0		;
;
	MOVLW	0x14			;
	MOVWF	X_P,A			;
	MOVLW	0x03			;
	XORWF	A_P,W,A			;
	BZ	adr_17A9		;

	MOVLW	0x7A			;
	XORWF	A_P,W,A			;
	BNZ	adr_17B1		;

	GOTO	sub_1E12_ProcCom7A		;NO return
;--
adr_17A0:
	CALL	JDG_ZO_RxIsAll00			;ADD

;	BSF	f_cmd35,A		;

;	MOVF	RAM_83,W,BANKED		;
;	IORWF	RAM_84,W,BANKED		;
;	IORWF	RAM_85,W,BANKED		;
;	IORWF	RAM_86,W,BANKED		;
;	IORWF	RAM_87,W,BANKED		;
;	IORWF	RAM_88,W,BANKED		;
;	IORWF	RAM_89,W,BANKED		;
;	IORWF	RAM_8A,W,BANKED		;
;	BCF	f_zero,A		;
;	BTFSC	STATUS,Z,A		;
;	BSF	f_zero,A		;
;--
adr_17A9:
	MOVF	X_P,W,A			;
	MOVWF	RAM_C4,BANKED		;

;	MOVLW	0x80			;
;	MOVWF	X_P,A			;待确定
	CALL	sub_1804_WriteDecode_hashcal_TxAnswer		;
	GOTO	sub_1854		;NO return--CALL
;----
adr_17B1:
	GOTO	sub_1297_whileLEDFlash		;
;---------------------------------------
sub_17B4:
	;;;;CALL	sub_17DC_waitRxFrame_cominA_P		;
	BTFSC	f21_over1,A		;
	BRA	adr_17D9_whileLEDFlash		;
;
	BSF	RAM_CB,5,BANKED		;命令字37时标志

	MOVLW	0x10			;
	MOVWF	X_P,A			;
	MOVLW	0x04			;
	XORWF	A_P,W,A			;
	BZ	adr_17C9_ProcCom37		;

	MOVLW	0x10			;add
	MOVWF	X_P,A			;
	MOVLW	0x37			;
	XORWF	A_P,W,A			;
	BZ	adr_17C9_ProcCom37		;

	MOVLW	0x7A			;查询指令
	XORWF	A_P,W,A			;
	BNZ	adr_17D9_whileLEDFlash		;

	GOTO	sub_1E12_ProcCom7A		;
adr_17C9_ProcCom37:
	MOVF	X_P,W,A			;
	MOVWF	RAM_C4,BANKED		;
	BSF	f20_RDee,A		;
	MOVLW	0x80			;
	MOVWF	X_P,A			;
	CALL	sub_1B48_Load8EEto93to9A_byX_P		;
	CALL	sub_1BE7_93to9A_FanXuPaiLie		;
	CALL	sub_1804_WriteDecode_hashcal_TxAnswer		;
	GOTO	sub_1854		;no return-CALL
;---
adr_17D9_whileLEDFlash:
	GOTO	sub_1297_whileLEDFlash		;
;---------------------------------------
JDG_ZO_RxIsAll00:;判断接收数据是否全部为00
	BSF	f_cmd35,A		;

	MOVF	RAM_83,W,BANKED		;
	IORWF	RAM_84,W,BANKED		;
	IORWF	RAM_85,W,BANKED		;
	IORWF	RAM_86,W,BANKED		;
	IORWF	RAM_87,W,BANKED		;
	IORWF	RAM_88,W,BANKED		;
	IORWF	RAM_89,W,BANKED		;
	IORWF	RAM_8A,W,BANKED		;
	BCF	f_zero,A		;
	BTFSC	STATUS,Z,A		;
	BSF	f_zero,A		;

	RETURN				;
;---------------------------------------
sub_17DC_waitRxFrame_cominA_P:
adr_17DC:
	MOVLW	.0			;
adr_17DD:
	BCF f21_over1
	return 
	CALL	sub_1C30_RxFrame_pBuf80		;
;	BTFSC	f21_over1,A		;
;	BRA	adr_17F0		;

	BTFSS	f21_over1,A		;
	BRA	adr_17DF		;

	MOVLW	.100			;
	CALL	sub_1C30_RxFrame_pBuf80		;
	BTFSC	f21_over1,A		;
	BRA	adr_17F0		;

adr_17DF:
	MOVF	RAM_81,W,BANKED		;
	MOVWF	RAM_C2,BANKED		;
	MOVWF	A_P,A			;

	MOVLW	0x0F			;
	XORWF	A_P,W,A			;
	BNZ	adr_17F0		;

;	CALL	sub_17F1		;
	CALL	sub_17F2_Answer_0x0F		;

	MOVLW	.50			;
	BRA	adr_17DD		;adr_17DC
adr_17F0:
	BCF	BAT_CL			;ADD

	RETURN				;
;---------------------------------------
;sub_17F1:
;	MOVLW	0x88			;
;	MOVWF	RAM_D1,BANKED		;
;	MOVLW	0x0D			;
;	MOVWF	RAM_C2,BANKED		;
;	CLRF	RAM_C3,BANKED		;
;	CALL	sub_1A25		;
;	BNZ	adr_1803		;
;
;	CALL	sub_1A7B		;
;adr_1803:
;	RETURN				;
;---------------------------------------
sub_17F2_Answer_0x0F:
	MOVLW	0x88			;
	MOVWF	RAM_D1,BANKED		;
	MOVLW	0x0D			;
	MOVWF	RAM_C2,BANKED		;
	CLRF	RAM_C3,BANKED		;

	MOVF	RAM_D1,W,BANKED		;
	MOVWF	X_P,A			;
	BSF	f20_RDee,A		;
	CALL	sub_1B48_Load8EEto93to9A_byX_P		;
;
	BTFSS	RAM_CB,6,BANKED		;
	BRA	adr_17F3		;
	CALL	sub_1BE7_93to9A_FanXuPaiLie		;loaded data 反序
adr_17F3:
	CALL	sub_19F7_Data83and8Bis93		;
	MOVLW	.10			;
	CALL	DELAY_N_10MS		;

	BSF	BAT_CL			;

	MOVLW	.10			;
	MOVWF	X_P,A			;
	CALL	TX_IR_B_10_data81_LenBYX_P_add1			;

	RETURN				;
;---------------------------------------
sub_1804_WriteDecode_hashcal_TxAnswer:
	MOVLW	.8			;
	MOVWF	Temp_0,A		;
	LFSR	FSR0,0x83		;RAM_83
	LFSR	FSR1,0x8B		;RAM_8B
adr_1806:
	MOVF	POSTINC0,W		;
	MOVWF	POSTINC1,A		;

	DECFSZ	Temp_0,F,A		;
	BRA	adr_1806		;

	BTFSC	RAM_CB,5,BANKED		;
	CALL	sub_0E34_mayWriteDecode_83to8B		;adr_1813
;
	BTFSS	f_cmd35,A		;
	BRA	adr_1816		;

	BCF	f_cmd35,A		;
	BTFSC	f_zero			;
	BRA	adr_1841		;
adr_1816:
;
	MOVF	RAM_C4,W,BANKED		;
	SUBWF	RAM_82,W,BANKED		;-RAM_C4
	BC	adr_1841		;

	MOVLW	0x08			;
	MULWF	RAM_82,BANKED		;

	MOVF	RAM_81,W,BANKED		;
	MOVWF	X_P,A			;

	MOVF	PRODL,W			;
	MOVWF	A_P,A			;

	MOVLW	0x88			;
	ADDWF	A_P,F,A			;
	MOVLW	0x0E			;
	XORWF	X_P,W,A			;
	BZ	adr_183C		;

	MOVLW	0x08			;
	SUBWF	A_P,F,A			;
	MOVLW	0x00			;
	XORWF	X_P,W,A			;
	BZ	adr_183C		;

	MOVLW	0x7E			;
	SUBWF	A_P,F,A			;
	MOVLW	0x68			;
	XORWF	X_P,W,A			;
	BZ	adr_183C		;

	MOVLW	0x02			;
	SUBWF	A_P,F,A			;
	MOVLW	0x03			;
	XORWF	X_P,W,A			;
	BNZ	adr_183C		;

	MOVLW	0x02			;
	SUBWF	A_P,F,A			;
adr_183C:
	BSF	BAT_CL			;;;;;;;;

	MOVF	A_P,W,A			;
	MOVWF	ADDRESS,A		;
	LFSR	FSR0,0x8B		;RAM_8B
	MOVLW	.8			;
	MOVWF	Temp_0,A		;
	CALL	EE_WRITE_M		;
adr_1841:
	MOVLW	0x83			;
	CALL	sub_0C11_HashCalc_byWisAddr		;
	CALL	sub_184A_TX_IR_B_10_05_82to8a_Len10		;

	RETURN				;
;---------------------------------------
sub_184A_TX_IR_B_10_05_82to8a_Len10:
	MOVLW	0x05			;
	MOVWF	RAM_81,BANKED		;
	MOVLW	0x0A			;.10
	MOVWF	X_P,A			;
	CALL	TX_IR_B_10_data81_LenBYX_P_add1			;
	RETURN				;
;---------------------------------------
sub_1854:
adr_1854:
	CALL	sub_18C4_CheckSameFrame_82isFF		;
	BTFSC	f21_over1,A		;
	BRA	adr_18C1_whileLEDFlash		;

	MOVLW	0xFF			;
	XORWF	RAM_82,W,BANKED		;
	BNZ	adr_1854		;
;
	MOVLW	0x06			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_186F		;

	MOVLW	0x68			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_186F		;

	MOVLW	0x35			;add
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_186F		;

	MOVLW	0x37			;add
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_186F		;

	CALL	sub_18E0		;检查收到数据的完整性
	BNZ	adr_18BA		;
adr_186F:
	MOVLW	.8			;
	MOVWF	Temp_0,A		;
	LFSR	FSR0,0x83		;RAM_83
adr_1871:
	MOVLW	0x00			;
	MOVWF	POSTINC0,A		;

	DECFSZ	Temp_0,F,A		;
	BRA	adr_1871		;

	MOVLW	0x0E			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_18BA		;

	MOVLW	0x68			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_18BA		;

	MOVLW	0x21			;
	MOVWF	X_P,A			;
	MOVLW	0x00			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_18B7		;

	MOVLW	0x27			;
	MOVWF	X_P,A			;
	MOVLW	0x06			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_18B7		;

	MOVLW	0x07			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_18B7		;

	MOVLW	0x0C			;
	MOVWF	X_P,A			;
	MOVLW	0x03			;
	XORWF	RAM_81,W,BANKED		;
	BZ	adr_18B7		;

	CALL	sub_1468_5F_73_69_I2CaddrProc		;
	CALL	sub_1672_GetDataBlock_555f_4149_2d37	;检查2d\37\41\4b\55\5f数据段是否合规，合规数据段分别在c3\c4\c5中	;
	MOVLW	0xF1			;
	ANDWF	RAM_C9,W,BANKED		;	
	BNZ	adr_18C1_whileLEDFlash		;

	MOVLW	0x98			;
	MOVWF	ADDRESS,A		;
;	CALL	EE_READ			;
	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVWF	A_P,A			;
	INCF	A_P,F,A			;
	BNZ	adr_18AF		;

	CALL	sub_1ACB		;写入电压检测值
	MOVLW	0x00			;
	MOVWF	RAM_83,BANKED		;
	MOVWF	RAM_84,BANKED		;
adr_18AF:
	CALL	sub_1AD7		;EE完成修正。
	BTFSS	RAM_CB,5,BANKED		;
	BRA	adr_18B7		;
	MOVLW	0x05			;
	MOVWF	X_P,A			;
adr_18B7:
	CALL	sub_1B07_I2C_addr9e_dataWandnotW		;写入遥控状态
adr_18BA:
	CALL	sub_184A_TX_IR_B_10_05_82to8a_Len10		;
	BSF	LED			;
adr_18BF:
	BCF	BAT_CL			;;;;;
;	RESET				;;;;;
	CLRWDT				;
	BRA	adr_18BF		;
adr_18C1_whileLEDFlash:
	GOTO	sub_1297_whileLEDFlash		;
;---------------------------------------
sub_18C4_CheckSameFrame_82isFF:
	MOVLW	.100			;
	CALL	sub_1C30_RxFrame_pBuf80		;
	BTFSC	f21_over1,A		;
	BRA	adr_18DF		;

	BSF	f21_over1,A		;
	MOVF	RAM_81,W,BANKED		;
	XORWF	RAM_C2,W,BANKED		;
	BNZ	adr_18DF		;

	BCF	f21_over1,A		;
	MOVLW	0xFF			;
	XORWF	RAM_82,W,BANKED		;
	BZ	adr_18DF		;

	CALL	sub_1804_WriteDecode_hashcal_TxAnswer		;
adr_18DF:
	RETURN				;
;---------------------------------------
sub_18E0:
	MOVF	ADDRESS,W,A		;
	MOVWF	X_P,A			;
	DECF	ADDRESS,F,A		;7F单元的02，也就是校验是02命令字时进行数据检测
	CALL	EE_READ			;
	;;;;CALL	IIC_READ_byTempEE_Tempbb		;
	MOVF	TEMP_EE,W,A		;
	XORWF	RAM_C2,W,BANKED		;RAM_81 buf
	BNZ	adr_192A		;

	MOVLW	0x88			;
	MOVWF	X_P,A			;
	MOVLW	0x0E			;
	XORWF	RAM_C2,W,BANKED		;
	BZ	adr_18FB		;

	MOVLW	0x80			;
	MOVWF	X_P,A			;
	MOVLW	0x00			;
	XORWF	RAM_C2,W,BANKED		;
	BZ	adr_18FB		;

	MOVLW	0x00			;35,02命令字从地址00读
	MOVWF	X_P,A			;
	MOVLW	0x03			;
	XORWF	RAM_C2,W,BANKED		;
	BNZ	adr_18FB		;

	MOVLW	0xFE			;
	MOVWF	X_P,A			;
adr_18FB:
	MOVF	X_P,W,A			;RAM_D1
;	MOVWF	ADDRESS,A		;
	CALL	sub_192B		;RAM_9B 以后8个RAM置零
adr_1900:
	CALL	sub_1B48_Load8EEto93to9A_byX_P		;读出EEPROM数据放到RAM_93以后的8个RAM
	CALL	sub_1BE7_93to9A_FanXuPaiLie		;

	MOVLW	0xFE			;
	XORWF	X_P,W,A			;
	BNZ	adr_1912		;

	MOVLW	0x00			;
	MOVWF	RAM_93,BANKED		;
	MOVWF	RAM_94,BANKED		;
adr_1912:
	CALL	sub_1933		;
	MOVLW	0x08			;
	ADDWF	X_P,F,A			;
	DECF	RAM_C4,F,BANKED		;对命令35，是0X10
	BNZ	adr_1900		;
;
	MOVLW	.8			;
	MOVWF	Temp_0,A		;
	LFSR	FSR0,0x9B		;RAM_9B
	LFSR	FSR1,0x8B		;RAM_8B
adr_1921:
	MOVF	POSTINC0,W,A		;
	XORWF	POSTINC1,W,A		;
	BNZ	adr_192A		;	

	DECF	Temp_0,F,A		;
	BNZ	adr_1921		;
adr_192A:
	RETURN				;
;---------------------------------------
sub_192B:
	MOVLW	.8			;
	MOVWF	Temp_0,A		;
	LFSR	FSR0,0x9B		;RAM_9B
adr_192D:
	MOVLW	0x00			;
	MOVWF	POSTINC0,A		;
	DECFSZ	Temp_0,F,A		;
	BRA	adr_192D		;

	RETURN				;
;---------------------------------------
sub_1933:
	MOVLW	.8			;
	MOVWF	Temp_0,A		;
	LFSR	FSR0,0x93		;RAM_93
	LFSR	FSR1,0x9B		;RAM_9B
adr_1935:
	MOVF	POSTINC0,W,A		;
	XORWF	POSTINC1,F,A		;
	DECFSZ	Temp_0,F,A		;
	BRA	adr_1935		;

	MOVLW	0x9B			;
	CALL	sub_0C11_HashCalc_byWisAddr		;
	RETURN				;
;---------------------------------------
sub_1944:


	RETURN				;
;---------------------------------------
sub_1A8E:

	RETURN				;
;---------------------------------------
sub_1ACB:
	MOVLW	0x73			;
	MOVWF	RAM_83,BANKED		;
	MOVLW	0x0C			;
	MOVWF	RAM_84,BANKED		;
	CALL	sub_1B3C_RAM83_88_WritetoEE98_9D		;
	RETURN				;
;---------------------------------------
sub_1AD7:
	CALL	sub_1B26_I2C_90to93_wr0x01		;写同步值
	MOVLW	0x94			;
	MOVWF	ADDRESS,A		;
	MOVLW	0xFF			;
	MOVWF	TEMP_EE,A		;
;	CALL	EE_WRITE		;
	CALL	IIC_WRITE		;

	MOVLW	0x00			;
	MOVWF	ADDRESS,A		;
;	CALL	EE_READ			;
	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVWF	A_P,A			;
	RLCF	A_P,F,A			;
	BNC	adr_1AEA		;

	MOVLW	0x94			;
	MOVWF	ADDRESS,A		;
	CALL	EE_WT_CHK_AddrBYW_FFor0		;
adr_1AEA:
	MOVLW	0x95			;
	MOVWF	ADDRESS,A		;
	MOVLW	0xFF			;
	MOVWF	TEMP_EE,A		;
;	CALL	EE_WRITE		;
	CALL	IIC_WRITE		;

	CALL	sub_1C13		;
;
	MOVLW	0x04			;
	MOVWF	X_P,A			;
	RETURN				;
;---------------------------------------
sub_1B3C_RAM83_88_WritetoEE98_9D:
	MOVLW	0x98			;
	MOVWF	ADDRESS,A		;
	LFSR	FSR0,0x83		;RAM_83
	MOVLW	.6			;
	MOVWF	Temp_0,A		;
	CALL	EE_WRITE_M		;
	RETURN				;
;---------------------------------------
sub_1B26_I2C_90to93_wr0x01:;写入同步值，I2C中90、91、92、93均写入01
	MOVLW	0x01			;
	MOVWF	RAM_8B,BANKED		;
	MOVWF	RAM_8C,BANKED		;
	MOVWF	RAM_8D,BANKED		;
	MOVWF	RAM_8E,BANKED		;

	MOVLW	0x90			;
	MOVWF	ADDRESS,A		;
	LFSR	FSR0,0x8B		;RAM_8B
	MOVLW	.4			;
	MOVWF	Temp_0,A		;
	CALL	EE_WRITE_M		;
	RETURN				;
;---------------------------------------
sub_1C13:
	MOVLW	0x01			;修正00行
	MOVWF	X_P,A			;
	CALL	sub_1B48_Load8EEto93to9A_byX_P		;

	LFSR	FSR0,0x9B		;RAM_9B
	CALL	sub_1B51_Read09to10_InFSR0 ;读取09到10共8个数据到9B中

	MOVLW	.2			;
	MOVWF	Temp_0,A		;
	LFSR	FSR0,0x9B		;RAM_9B
	LFSR	FSR1,0x9E		;RAM_9E
	CALL	sub_1BEF		;

	MOVLW	0x01			;
	MOVWF	ADDRESS,A		;
	LFSR	FSR0,0x93		;RAM_93
	MOVLW	.12			;
	MOVWF	Temp_0,A		;
	CALL	EE_WRITE_M		;

	RETURN				;
;---------------------------------------

;---------------------------------------
;sub_1A25:
;	MOVF	RAM_D1,W,BANKED		;
;	MOVWF	X_P,A			;
;	CALL	sub_1B48_Load8EEto93to9A_byX_P		;
;;
;	BTFSS	RAM_CB,6,BANKED		;
;	BRA	adr_1A30		;
;	CALL	sub_1BE7_93to9A_FanXuPaiLie		;loaded data 反序
;adr_1A30:
;	CALL	sub_1A34		;
;	RETURN				;
;---------------------------------------
;sub_1A34:
;	MOVLW	0x03			;
;	MOVWF	RAM_CE,BANKED		;
;adr_1A38:
;	DECF	RAM_CE,F,BANKED		;
;	BN	adr_1A54		;
;
;	CALL	sub_19F7_Data83and8Bis93		;
;	MOVLW	.10			;
;	CALL	DELAY_N_10MS		;
;
;	MOVLW	.10			;
;	MOVWF	X_P,A			;
;	CALL	TX_IR_B_10_data81_LenBYX_P_add1			;
;	CALL	sub_1A55		;
;	BNZ	adr_1A38		;
;
;	CALL	sub_1933		;
;	INCF	RAM_C3,F,BANKED		;
;	BSF	STATUS,Z,A		;
;adr_1A54:
;	RETURN				;
;---------------------------------------
sub_1A55:
	MOVLW	0x8B			;
	CALL	sub_0C11_HashCalc_byWisAddr		;
	MOVLW	.100			;
	CALL	sub_1C30_RxFrame_pBuf80		;

	BCF	STATUS,Z,A		;
	MOVLW	0x08			;
	MOVWF	X_P,A			;
	BTFSC	f21_over1,A		;
	BRA	adr_1A6D		;

	LFSR	FSR0,0x92		;RAM_92
	LFSR	FSR1,0x8A		;RAM_8A

adr_1A64:
	MOVF	POSTDEC0,W,A		;
	XORWF	POSTDEC1,W,A		;
	BNZ	adr_1A6D		;

	DECF	X_P,F,A			;
	BNZ	adr_1A64		;
adr_1A6D:

	RETURN				;
;---------------------------------------
sub_1A7B:
	MOVF	RAM_C2,W,BANKED		;
	MOVWF	RAM_81,BANKED		;
	MOVLW	0x09			;
	MOVWF	X_P,A			;
	LFSR	FSR0,0x8A		;RAM_8A
adr_1A83:
	MOVLW	0xFF			;
	MOVWF	POSTDEC0,A		;
	DECF	X_P,F,A			;
	BNZ	adr_1A83		;

	MOVLW	.10			;
	MOVWF	X_P,A			;
	CALL	TX_IR_B_10_data81_LenBYX_P_add1			;

	RETURN				;
;---------------------------------------

;---------------------------------------
sub_19D9_LoadData:
	MOVLW	0x4E			;
	BTFSS	RAM_CA_1,BANKED		;
	MOVLW	0x05			;
	MOVWF	X_P,A			;
	CALL	sub_1B48_Load8EEto93to9A_byX_P		;

	MOVLW	0x56			;
	BTFSS	RAM_CA_1,BANKED		;
	MOVLW	0x9B			;
	MOVWF	ADDRESS,A		;
;	CALL	EE_READ			;
	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVWF	RAM_97,BANKED		;

	CLRF	RAM_98,BANKED		;
	CLRF	RAM_99,BANKED		;
	MOVF	RAM_C2,W,BANKED		;
	MOVWF	RAM_9A,BANKED		;

	RETURN				;
;---------------------------------------
sub_19F7_Data83and8Bis93:
	MOVF	RAM_C2,W,BANKED		;
	MOVWF	RAM_81,BANKED		;
	MOVF	RAM_C3,W,BANKED		;
	MOVWF	RAM_82,BANKED		;

	BTFSS	RAM_CA_1,BANKED		;
	BRA	adr_1A09		;

	MOVLW	0x0A			;
	XORWF	RAM_82,W,BANKED		;
	BNZ	adr_1A09		;

	CALL	sub_19D9_LoadData		;
adr_1A09:
	MOVLW	0x08			;
	MOVWF	X_P,A			;
	LFSR	FSR0,0x93		;RAM_93
	LFSR	FSR1,0x8B		;RAM_8B
	LFSR	FSR2,0x83		;RAM_83
adr_1A0B:
	MOVF	POSTINC0,W,A		;
	MOVWF	POSTINC1,A		;
	MOVWF	POSTINC2,A		;
	DECFSZ	X_P,F,A			;
	BRA	adr_1A0B		;

	RETURN				;
;---------------------------------------
sub_0E34_mayWriteDecode_83to8B:
	MOVLW	0x8B			;计算数据入口:RAM_8B + N
	MOVWF	RAM_BB,BANKED		;
	MOVLW	0xA3			;输出计算结果的指针
	MOVWF	RAM_BC,BANKED		;
	MOVLW	0x3F			;读表指针
	MOVWF	A_P,A			;
	MOVLW	0x3F			;循环次数64
	MOVWF	X_P,A			;
	BCF	RAM_CB,6,BANKED		;
	CALL	sub_0D40		;

	MOVLW	0x93			;
	MOVWF	RAM_BB,BANKED		;
	MOVLW	0xAB			;
	MOVWF	RAM_BC,BANKED		;
	MOVLW	0x3F			;
	MOVWF	A_P,A			;
	MOVLW	0x3F			;
	MOVWF	X_P,A			;
	BSF	RAM_CB,6,BANKED		;
	CALL	sub_0D40		;

	MOVLW	0x0F			;
	MOVWF	RAM_C1,BANKED		;
adr_0E5A:
	CALL	sub_0DE5		;
	DECF	RAM_C1,F,BANKED		;
	BNN	adr_0E5A		;

	MOVLW	0xA3			;
	MOVWF	RAM_BB,BANKED		;
	MOVLW	0x8B			;
	MOVWF	RAM_BC,BANKED		;
	MOVLW	0x7F			;
	MOVWF	A_P,A			;
	MOVLW	0x3F			;
	MOVWF	X_P,A			;
	BCF	RAM_CB,6,BANKED		;
	CALL	sub_0D40		;

	RETURN				;
;---------------------------------------
sub_0D40:
	MOVF	A_P,W,A			;
	MOVWF	RAM_BD,BANKED		;读表指针
	MOVF	X_P,A			;
	MOVWF	RAM_BE,BANKED		;循环次数
adr_0D44:
	MOVF	RAM_BE,W,BANKED		;
	MOVWF	TEMP0,A			;
	RRCF	TEMP0,F,A		;
	RRCF	TEMP0,F,A		;
	RRCF	TEMP0,F,A		;计算字节偏移量，
	MOVLW	0x1F			;00011111
	ANDWF	TEMP0,F,A		;
	MOVF	RAM_BC,W,BANKED		;
	ADDWF	TEMP0,W,A		;
	MOVWF	FSR0L,A			;FSR0 :输出计算结果的指针
	CLRF	FSR0H,A			;

	CALL	LOAD_0E73		;
	BTFSC	RAM_CB,6,BANKED		;
	CALL	LOAD_0F53		;
	MOVF	RAM_BD,W,BANKED		;
	CALL	READ_TAB		;
	MOVF	TEMP1,W,A		;F
	BN	adr_0D76		;

	MOVLW	0x07			;小于80时，读表出的数据暂存
	ANDWF	TEMP1,W,A		;
	MOVWF	RAM_C0,BANKED		;

	CALL	LOAD_0E73		;
	BTFSC	RAM_CB,6,BANKED		;
	CALL	LOAD_0F53		;
	MOVF	RAM_BD,W,BANKED		;
	CALL	READ_TAB		;再读数据，测试后看是否可以删除重读

	MOVWF	TEMP0,A			;
	RRCF	TEMP0,F,A		;
	RRCF	TEMP0,F,A		;
	RRCF	TEMP0,F,A		;读出数据除以8确定源数据的字节偏移量
	MOVLW	0x1F			;00011111
	ANDWF	TEMP0,F,A		;
	MOVF	RAM_BB,W,BANKED		;
	ADDWF	TEMP0,W,A		;
	MOVWF	FSR1L,A			;
	CLRF	FSR1H,A			;

	BCF	STATUS,C,A		;此指令位置需要调整
	CALL	LOAD_10E3		;
	MOVF	RAM_C0,W,BANKED		;
	CALL	READ_TAB		;
	ANDWF	INDF1,W,A		;
	BZ	adr_0D77		;
adr_0D76:
	BSF	STATUS,C,A		;
adr_0D77:
	RRCF	INDF0,F,A		;输出计算结果的指针
	DECF	RAM_BD,F,BANKED		;

	DECF	RAM_BE,F,BANKED		;循环次数
	BNN	adr_0D44		;

	RETURN				;
;---------------------------------------
sub_0DE5:
	MOVLW	0xA7			;
	MOVWF	RAM_BB,BANKED		;
	MOVLW	0x9B			;
	MOVWF	RAM_BC,BANKED		;指针
	MOVLW	0xBF			;
	MOVWF	A_P,A			;
	MOVLW	0x3F			;
	MOVWF	X_P,A			;
	BCF	RAM_CB,6,BANKED		;
	CALL	sub_0D40		;
	CALL	sub_0DAF		;

	MOVLW	.8			;
	MOVWF	X_P,A			;
	LFSR	FSR0,0x9B		;RAM_9B
	LFSR	FSR1,0xB3		;RAM_B3
adr_0DFB:
	MOVF	POSTINC1,W,A		;
	XORWF	POSTINC0,F,A		;
	DECFSZ	X_P,F,A			;
	BRA	adr_0DFB		;

	MOVLW	0x9B			;
	MOVWF	RAM_BB,BANKED		;
	MOVLW	0x9B			;
	MOVWF	RAM_BC,BANKED		;
	CALL	sub_0D81		;

	MOVLW	0x9B			;
	MOVWF	RAM_BB,BANKED		;
	MOVLW	0xB3			;
	MOVWF	RAM_BC,BANKED		;
	MOVLW	0xDF			;
	MOVWF	A_P,A			;
	MOVLW	0x1F			;
	MOVWF	X_P,A			;
	BCF	RAM_CB,6,BANKED		;
	CALL	sub_0D40		;

	MOVLW	.4			;
	MOVWF	X_P,A			;
	LFSR	FSR0,0xA6		;RAM_A6
	LFSR	FSR1,0xAA		;RAM_AA
	LFSR	FSR2,0xB6		;RAM_B6
adr_0E22:
	MOVF	INDF0,W			;
	XORWF	INDF2,F,A		;

	MOVF	INDF1,W,A		;
	MOVWF	INDF0,A			;

	MOVF	INDF2,W,A		;
	MOVWF	INDF1,A			;

	MOVF	POSTDEC0,W,A		;
	MOVF	POSTDEC1,W,A		;
	MOVF	POSTDEC2,W,A		;

	DECFSZ	X_P,F,A			;
	BRA	adr_0E22		;

	RETURN				;
;---------------------------------------
sub_0DAF:
	CALL	LOAD_10D3		;
	MOVF	RAM_C1,W,BANKED		;
	CALL	READ_TAB		;
	MOVF	TEMP1,W,A		;
	BZ	adr_0DD3		;

adr_0DB6:
	RRCF	RAM_AB,F,BANKED		;
	RRCF	RAM_AC,F,BANKED		;
	RRCF	RAM_AD,F,BANKED		;
	RRCF	RAM_AE,F,BANKED		;

	BCF	RAM_AB,7,BANKED		;
	BTFSC	RAM_AE,3,BANKED		;
	BSF	RAM_AB,7,BANKED		;
;
	RRCF	RAM_AF,F,BANKED		;
	RRCF	RAM_B0,F,BANKED		;
	RRCF	RAM_B1,F,BANKED		;
	RRCF	RAM_B2,F,BANKED		;

	BCF	RAM_AF,7,BANKED		;
	BTFSC	RAM_B2,3,BANKED		;
	BSF	RAM_AF,7,BANKED		;

	DECF	TEMP1,F,A		;
	BNZ	adr_0DB6		;
adr_0DD3:
	MOVLW	0xAB			;
	MOVWF	RAM_BB,BANKED		;
	MOVLW	0xB3			;
	MOVWF	RAM_BC,BANKED		;
	MOVLW	0x7F			;
	MOVWF	A_P,A			;
	MOVLW	0x3F			;
	MOVWF	X_P,A			;
	BSF	RAM_CB,6,BANKED		;
	CALL	sub_0D40		;

	RETURN				;
;---------------------------------------
sub_0D81:
	MOVLW	0xE0			;
	MOVWF	RAM_BD,BANKED		;
adr_0D85:
	CALL	LOAD_0FD3		;
	MOVF	RAM_BB,W,BANKED		;
	MOVWF	FSR0L,A			;
	CLRF	FSR0H,A			;
	BCF	STATUS,C,A		;
	RRCF	INDF0,W,A		;
;	MOVWF	X_P,A			;
	IORWF	RAM_BD,W,BANKED		;
	CALL	READ_TAB		;
	MOVWF	A_P,A			;
	BCF	STATUS,C,A		;
	RRCF	INDF0,W,A		;
	BC	adr_0D95		;'1'

	BCF	STATUS,C,A		;
	RRCF	A_P,F,A			;
	BCF	STATUS,C,A		;
	RRCF	A_P,F,A			;
	BCF	STATUS,C,A		;
	RRCF	A_P,F,A			;
	BCF	STATUS,C,A		;
	RRCF	A_P,F,A			;
adr_0D95:
	MOVLW	0x0F			;
	ANDWF	A_P,F,A			;
	MOVF	RAM_BC,W,BANKED		;
	MOVWF	FSR0L,A			;
	CLRF	FSR0H,A			;

	BTFSC	RAM_BD,5,BANKED		;
	BRA	adr_0DA3		;

	INCF	RAM_BC,F,BANKED		;
	BCF	STATUS,C,A		;
	RLCF	INDF0,F,A		;
	BCF	STATUS,C,A		;
	RLCF	INDF0,F,A		;
	BCF	STATUS,C,A		;
	RLCF	INDF0,F,A		;
	BCF	STATUS,C,A		;
	RLCF	INDF0,F,A		;
	MOVF	INDF0,W,A		;
	IORWF	A_P,F,A			;
adr_0DA3:
	MOVF	A_P,W,A			;
	MOVWF	INDF0,A			;
	INCF	RAM_BB,F,BANKED		;
	MOVLW	0x20			;
	SUBWF	RAM_BD,F,BANKED		;
	BC	adr_0D85		;

	RETURN				;
;---------------------------------------

;---------------------------------------
LOAD_0E73:
	MOVLW	UPPER(TAB_0E73)		;
	MOVWF	TBLPTRU,A		;
	MOVLW	HIGH(TAB_0E73)		;
	MOVWF	TBLPTRH,A		;
	MOVLW	LOW(TAB_0E73)		;
	MOVWF	TBLPTRL,A		;

	RETURN				;
;---------------------------------------
LOAD_0F53:
	MOVLW	UPPER(TAB_0F53)		;
	MOVWF	TBLPTRU,A		;
	MOVLW	HIGH(TAB_0F53)		;
	MOVWF	TBLPTRH,A		;
	MOVLW	LOW(TAB_0F53)		;
	MOVWF	TBLPTRL,A		;

	RETURN				;
;---------------------------------------
LOAD_0FD3:
	MOVLW	UPPER(TAB_0FD3)		;
	MOVWF	TBLPTRU,A		;
	MOVLW	HIGH(TAB_0FD3)		;
	MOVWF	TBLPTRH,A		;
	MOVLW	LOW(TAB_0FD3)		;
	MOVWF	TBLPTRL,A		;

	RETURN				;
;---------------------------------------
LOAD_10D3:
	MOVLW	UPPER(TAB_10D3)		;
	MOVWF	TBLPTRU,A		;
	MOVLW	HIGH(TAB_10D3)		;
	MOVWF	TBLPTRH,A		;
	MOVLW	LOW(TAB_10D3)		;
	MOVWF	TBLPTRL,A		;

	RETURN				;
;---------------------------------------
LOAD_10E3:
	MOVLW	UPPER(TAB_10E3)		;
	MOVWF	TBLPTRU,A		;
	MOVLW	HIGH(TAB_10E3)		;
	MOVWF	TBLPTRH,A		;
	MOVLW	LOW(TAB_10E3)		;
	MOVWF	TBLPTRL,A		;

	RETURN				;
;---------------------------------------



;---------------------------------------
READ_TAB:
	ADDWF	TBLPTRL,F,A		;
	BTFSC	STATUS,C,A		;
	INCF	TBLPTRH,F,A		;
	BTFSC	STATUS,C,A		;
	INCF	TBLPTRU,F,A		;

	TBLRD*				;
	MOVF	TABLAT,W,A		;
	MOVWF	TEMP1,A
	RETURN				;
;---------------------------------------
;------------------------------------
TAB_0E73:
;00--7F
	DB  0x39,0x31,0x29,0x21,0x19,0x11,0x09,0x01
	DB  0x3B,0x33,0x2B,0x23,0x1B,0x13,0x0B,0x03
	DB  0x3D,0x35,0x2D,0x25,0x1D,0x15,0x0D,0x05
	DB  0x3F,0x37,0x2F,0x27,0x1F,0x17,0x0F,0x07
	DB  0x38,0x30,0x28,0x20,0x18,0x10,0x08,0x00
	DB  0x3A,0x32,0x2A,0x22,0x1A,0x12,0x0A,0x02
	DB  0x3C,0x34,0x2C,0x24,0x1C,0x14,0x0C,0x04
	DB  0x3E,0x36,0x2E,0x26,0x1E,0x16,0x0E,0x06
	DB  0x07,0x27,0x0F,0x2F,0x17,0x37,0x1F,0x3F
	DB  0x06,0x26,0x0E,0x2E,0x16,0x36,0x1E,0x3E
	DB  0x05,0x25,0x0D,0x2D,0x15,0x35,0x1D,0x3D
	DB  0x04,0x24,0x0C,0x2C,0x14,0x34,0x1C,0x3C
	DB  0x03,0x23,0x0B,0x2B,0x13,0x33,0x1B,0x3B
	DB  0x02,0x22,0x0A,0x2A,0x12,0x32,0x1A,0x3A
	DB  0x01,0x21,0x09,0x29,0x11,0x31,0x19,0x39
	DB  0x00,0x20,0x08,0x28,0x10,0x30,0x18,0x38
;80--DF
	DB  0x80,0x80,0x1F,0x04,0x00,0x01,0x02,0x03
	DB  0x80,0x80,0x03,0x08,0x04,0x05,0x06,0x07
	DB  0x80,0x80,0x07,0x0C,0x08,0x09,0x0A,0x0B
	DB  0x80,0x80,0x0B,0x10,0x0C,0x0D,0x0E,0x0F
	DB  0x80,0x80,0x0F,0x14,0x10,0x11,0x12,0x13
	DB  0x80,0x80,0x13,0x18,0x14,0x15,0x16,0x17
	DB  0x80,0x80,0x17,0x1C,0x18,0x19,0x1A,0x1B
	DB  0x80,0x80,0x1B,0x00,0x1C,0x1D,0x1E,0x1F
	DB  0x0F,0x06,0x13,0x14,0x1C,0x0B,0x1B,0x10
	DB  0x00,0x0E,0x16,0x19,0x04,0x11,0x1E,0x09
	DB  0x01,0x07,0x17,0x0D,0x1F,0x1A,0x02,0x08
	DB  0x12,0x0C,0x1D,0x05,0x15,0x0A,0x03,0x18
;------------------------------------
TAB_0F53:
;00--7F
	DB  0x38,0x30,0x28,0x20,0x18,0x10,0x08,0x00
	DB  0x39,0x31,0x29,0x21,0x19,0x11,0x09,0x01
	DB  0x3A,0x32,0x2A,0x22,0x1A,0x12,0x0A,0x02
	DB  0x3B,0x33,0x2B,0x23,0x80,0x80,0x80,0x80
	DB  0x3E,0x36,0x2E,0x26,0x1E,0x16,0x0E,0x06
	DB  0x3D,0x35,0x2D,0x25,0x1D,0x15,0x0D,0x05
	DB  0x3C,0x34,0x2C,0x24,0x1C,0x14,0x0C,0x04
	DB  0x1B,0x13,0x0B,0x03,0x80,0x80,0x80,0x80
	DB  0x80,0x80,0x0D,0x04,0x10,0x0A,0x17,0x00
	DB  0x80,0x80,0x02,0x09,0x1B,0x0E,0x05,0x14
	DB  0x80,0x80,0x16,0x07,0x12,0x0B,0x03,0x19
	DB  0x80,0x80,0x0F,0x01,0x06,0x1A,0x13,0x0C
	DB  0x80,0x80,0x2C,0x3A,0x37,0x22,0x28,0x32
	DB  0x80,0x80,0x21,0x33,0x2B,0x36,0x30,0x24
	DB  0x80,0x80,0x2F,0x38,0x34,0x2A,0x3B,0x25
	DB  0x80,0x80,0x31,0x23,0x2D,0x35,0x27,0x20
;------------------------------------
TAB_0FD3:
;00--FF
	DB  0xD2,0x84,0x6F,0xB1,0xA9,0x3E,0x50,0xC7
	DB  0x1F,0xD8,0xA3,0x74,0xC5,0x6B,0x0E,0x92
	DB  0x7B,0x41,0x9C,0xE2,0x06,0xAD,0xF3,0x58
	DB  0x21,0xE7,0x4A,0x8D,0xFC,0x90,0x35,0x6B
	DB  0x4B,0x2E,0xF0,0x8D,0x3C,0x97,0x5A,0x61
	DB  0xD0,0xB7,0x49,0x1A,0xE3,0x5C,0x2F,0x86
	DB  0x14,0xBD,0xC3,0x7E,0xAF,0x68,0x05,0x92
	DB  0x6B,0xD8,0x14,0xA7,0x95,0x0F,0xE2,0x3C
	DB  0xC1,0xAF,0x92,0x68,0x0D,0x34,0xE7,0x5B
	DB  0xAF,0x42,0x7C,0x95,0x61,0xDE,0x0B,0x38
	DB  0x9E,0xF5,0x28,0xC3,0x70,0x4A,0x1D,0xB6
	DB  0x43,0x2C,0x95,0xFA,0xBE,0x17,0x60,0x8D
	DB  0x2C,0x41,0x7A,0xB6,0x85,0x3F,0xD0,0xE9
	DB  0xEB,0x2C,0x47,0xD1,0x50,0xFA,0x39,0x86
	DB  0x42,0x1B,0xAD,0x78,0xF9,0xC5,0x63,0x0E
	DB  0xB8,0xC7,0x1E,0x2D,0x6F,0x09,0xA4,0x53
	DB  0x7D,0xE3,0x06,0x9A,0x12,0x85,0xBC,0x4F
	DB  0xD8,0xB5,0x6F,0x03,0x47,0x2C,0x1A,0xE9
	DB  0xA6,0x90,0xCB,0x7D,0xF1,0x3E,0x52,0x84
	DB  0x3F,0x06,0xA1,0xD8,0x94,0x5B,0xC7,0x2E
	DB  0xA0,0x9E,0x63,0xF5,0x1D,0xC7,0xB4,0x28
	DB  0xD7,0x09,0x34,0x6A,0x28,0x5E,0xCB,0xF1
	DB  0xD6,0x49,0x8F,0x30,0xB1,0x2C,0x5A,0xE7
	DB  0x1A,0xD0,0x69,0x87,0x4F,0xE3,0xB5,0x2C
	DB  0xF1,0x8E,0x6B,0x34,0x97,0x2D,0xC0,0x5A
	DB  0x3D,0x47,0xF2,0x8E,0xC0,0x1A,0x69,0xB5
	DB  0x0E,0x7B,0xA4,0xD1,0x58,0xC6,0x93,0x2F
	DB  0xD8,0xA1,0x3F,0x42,0xB6,0x7C,0x05,0xE9
	DB  0xE4,0xD1,0x2F,0xB8,0x3A,0x6C,0x59,0x07
	DB  0x0F,0x74,0xE2,0xD1,0xA6,0xCB,0x95,0x38
	DB  0x41,0xE8,0xD6,0x2B,0xFC,0x97,0x3A,0x50
	DB  0xFC,0x82,0x49,0x17,0x5B,0x3E,0xA0,0x6D
;------------------------------------

;------------------------------------
TAB_10D3:
	DB  0x01,0x02,0x02,0x02,0x02,0x02,0x02,0x01
	DB  0x02,0x02,0x02,0x02,0x02,0x02,0x01,0x00
;------------------------------------
TAB_10E3:
	DB  0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01
;------------------------------------





;---------------------------------------
INITIAL:
	MOVLW	n_TRISC			;
	MOVWF	TRISC,A			;
	BCF	BAT_CL,A		;
	BSF	IR_A,A			;
	BSF	IR_B,A			;

	MOVLW	B'00000000'		;
	MOVWF	INTCON,A		;

	MOVLW	B'00000000'		;
	MOVWF	INTCON2,A		;

	MOVF	PORTB,W,A		;

	MOVLW	B'10000100'		;
	MOVWF	INTCON2,A		;

	MOVLW	B'00000000'		;
	MOVWF	INTCON3,A		;

	MOVLW	B'00000000'		;
	MOVWF	PIR1,A			;

	MOVLW	B'00000000'		;
	MOVWF	PIR2,A			;

	MOVLW	B'00000000'		;
	MOVWF	PIE1,A			;

	MOVLW	B'00000000'		;
	MOVWF	PIE2,A			;

	MOVLW	B'00000000'		;
	MOVWF	IPR1,A			;

	MOVLW	B'00000000'		;
	MOVWF	IPR2,A			;

	MOVLW	B'00000000'		;
	MOVWF	RCON,A			;

	MOVLW	B'01000000'		;
	MOVWF	T0CON,A			;
	CLRF	TMR0L,A			;
	CLRF	TMR0H,A			;

	MOVLW	B'00010000'		;
;	MOVLW	B'00000000'		;
	MOVWF	T1CON,A			;
	CLRF	TMR1L,A			;
	CLRF	TMR1H,A			;

	MOVLW	B'00000000'		;
	MOVWF	T2CON,A			;
	CLRF	TMR2,A			;
	CLRF	PR2,A			;

;	MOVLW	B'00111000'		;
	MOVLW	B'00010000'		;
	MOVWF	T3CON,A			;
	CLRF	TMR3L,A			;
	CLRF	TMR3H,A			;

	MOVLW	B'00000000'		;
	MOVWF	PSTR1CON,A		;
	MOVWF	PSTR2CON,A		;
	BANK15
	MOVLW	B'00000000'		;
	MOVWF	PSTR3CON,BANKED		;
	BANK0

	MOVLW	B'00000000'		;
	MOVWF	SSPCON1,A		;
	MOVWF	SSPCON2,A		;
	MOVWF	SSPCON3,A		;

	BANK15
	MOVLW	B'11111010'		;
	MOVWF	PMD0,BANKED		;
	MOVLW	B'11111110'		;
	MOVWF	PMD1,BANKED		;
	MOVLW	B'00001110'		;
	MOVWF	PMD2,BANKED		;

	MOVLW	B'00000000'		;
	MOVWF	ANSELA,BANKED		;
	MOVWF	ANSELB,BANKED		;
	MOVWF	ANSELC,BANKED		;

	MOVLW	B'10100000'		;2.048v
	MOVWF	VREFCON0,BANKED		;

	MOVLW	B'00000000'		;
	MOVWF	VREFCON1,BANKED		;

	MOVLW	B'00000000'		;
	MOVWF	VREFCON2,BANKED		;

	MOVLW	B'00000000'		;
	MOVWF	CTMUICONH,BANKED	;

	MOVLW	B'00000001'		;
	MOVWF	CCPTMRS0,BANKED		;
	MOVLW	B'00000000'		;
	MOVWF	CCPTMRS1,BANKED		;

	MOVLW	B'00000000'		;
	MOVWF	T4CON,BANKED		;
	MOVWF	T5CON,BANKED		;
	MOVWF	T6CON,BANKED		;

	MOVLW	B'00000000'		;
	MOVWF	CCP3CON,BANKED		;
	MOVWF	CCP4CON,BANKED		;
	MOVWF	CCP5CON,BANKED		;


	MOVLW	B'00000000'		;
	MOVWF	ECCP3AS,BANKED		;

	MOVLW	B'00000000'		;
	MOVWF	SRCON0,BANKED		;
	MOVLW	B'00000000'		;
	MOVWF	SRCON1,BANKED		;

	BANK0


	MOVLW	B'00000000'		;
	MOVWF	ECCP1AS,A		;
	MOVWF	ECCP2AS,A		;
;
	MOVLW	B'00000000'		;
	MOVWF	T2CON,A			;

	MOVLW	B'00000000'		;
	MOVWF	CM1CON0,A		;
	MOVLW	B'00000000'		;
	MOVWF	CM2CON0,A		;

	MOVLW	B'01100110'		;2.80V
	MOVWF	HLVDCON,A		;

	MOVLW	B'00000000'		;
	MOVWF	SLRCON,A		;

	MOVLW	n_AN_FVR		;
	MOVWF	ADCON0,A		;
	MOVLW	B'00000000'		;
	MOVWF	ADCON1,A		;

	MOVLW	B'00001011'		;左对齐
	MOVWF	ADCON2,A		;相当于结果除以4

	MOVLW	B'00000000'		;
	MOVWF	T1GCON,A		;
;	MOVLW	B'00000000'		;
	MOVWF	T3GCON,A		;
	BANK15
	MOVLW	B'00000000'		;
	MOVWF	T5GCON,BANKED		;
	BANK0
;
	MOVLW	n_TRISA1		;
	MOVWF	TRISA,A			;

	CLRF	PORTA,A			;
	CLRF	LATA,A			;

;
	CLRF	PORTB,A			;
	CLRF	LATB,A			;
	MOVLW	n_TRISB0		;
	MOVWF	TRISB,A			;
	MOVLW	B'11110010'		;
	MOVWF	WPUB,A			;
	MOVF	PORTB,W,A		;
	CLRF	IOCB,A			;
;
	MOVLW	B'00000000'		;
	MOVWF	SSP1CON1,A		;

	MOVLW	B'00000000'		;
	MOVWF	TXSTA1,A		;
	MOVWF	RCSTA1,A		;
;
	MOVLW	B'01101100'		;
	MOVWF	OSCCON,A		;
	MOVLW	B'00000000'		;
	MOVWF	OSCCON2,A		;
	MOVLW	B'00000000'		;
	MOVWF	OSCTUNE,A		;
;

	MOVLW	B'00000000'		;
	MOVWF	CCP2CON,A		;

	CLRF	CCP1CON,A		;

;------------------
	CALL	CLEAR_RAM		;

	RETURN				;
;---------------------------------------
;------------------
RECOV:
;------------------
;	BCF	f_10msTask,A		;
;	MOVLW	n_10ms			;
;	MOVWF	C_10ms,A		;
;------------------
;	BCF	PIR2,TMR3IF		;
;	BCF	PIR1,CCP1IF		;
;	BSF	PIE1,CCP1IE		;
;	BCF	T3CON,TMR3ON		;
;	CLRF	TMR3L,A			;
;	CLRF	TMR3H,A			;
;	BSF	T3CON,TMR3ON		;

;	MOVLW	n_125us			;
;	MOVWF	TMR0L,A			;
;	BCF	INTCON,T0IF,A		;
;	BSF	INTCON,T0IE,A		;
	BSF	INTCON,PEIE,A		;
	BSF	INTCON,GIE,A		;
;	BSF	T0CON,TMR0ON,A		;

;	BSF	T1CON,TMR1ON,A		;
	RETURN				;
;---------------------------------------
CLEAR_RAM:
;------------------
	CLRF	FSR0H,A			;
	CLRF	FSR0L,A			;
CLR_RAM:
	CLRF	INDF0			;
	INFSNZ	FSR0L,F,A		;
	INCF	FSR0H,F,A		;
	BTFSS	FSR0H,1,A		;0200H
	GOTO	CLR_RAM			;
;------------------
	RETURN				;
;---------------------------------------
;---------------------------------------
EE_READ:
	CLRWDT				;
	MOVF	ADDRESS,W,A		;
	MOVWF	EEADR,A			;
;
	BCF	EECON1,EEPGD,A		;
	BCF	EECON1,CFGS,A		;
	BSF	EECON1,RD,A		;
;
	MOVF	EEDATA,W,A		;
	MOVWF	TEMP_EE,A		;
	MOVWF	TEMP_bb,A		;
	RETURN				;
;---------------------------------------
EE_WRITE:
EE_WRITE_RE:
	CLRWDT				;
	MOVF	ADDRESS,W,A		;
	MOVWF	EEADR,A			;

	MOVF	TEMP_EE,W,A		;
	MOVWF	EEDATA,A		;

	BCF	EECON1,EEPGD,A		;
	BCF	EECON1,CFGS,A		;
	BSF	EECON1,WREN,A		;

	MOVLW	0x55			;
	MOVWF	EECON2,A		;
	MOVLW	0xAA			;
	MOVWF	EECON2,A		;
	BSF	EECON1,WR		;
WAIT_wr:
	CLRWDT				;
	BTFSC	EECON1,WR,A		;
	GOTO	WAIT_wr			;

	BCF	EECON1,WREN,A		;
	MOVF	EEDATA,W,A		;
	BCF	EECON1,EEPGD,A		;
	BCF	EECON1,CFGS,A		;
	BSF	EECON1,RD,A		;

	XORWF	EEDATA,W,A		;
	BTFSS	STATUS,Z,A		;
	BRA	EE_WRITE_RE		;	
WRITE_END:
	RETURN				;
;---------------------------------------
tab_0CB7:
	DB  0x45,0x03,0x11,0x4F,0x85,0x4C,0x7A,0x31
	DB  0xF6,0x7C,0xCB,0xB8,0x15,0xA4,0xD0,0x42
;---------------------------------------
tab_0CC7:
	DB  0x51,0x9B,0x49,0x14,0xB1,0x2E,0x8F,0xB5
	DB  0x95,0xDE,0x62,0x7A,0xD8,0x0D,0x44,0x1C
;---------------------------------------

;---------------------------------------
;TAB_0D20:
;	DB  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x04,0xFF
;	DB  0xFF,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,0xFF
;	DB  0xFF,0xFF,0xFF,0xFF,0xFF,0x04,0xFF,0xFF
;	DB  0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
;---------------------------------------
TAB_0ADE:
	DB  0x99,0x8F,0x00,0xC4,0xA7,0xA0,0x99,0xDB
	DB  0x99,0xB4,0xDB,0xA0,0x99,0xA7,0xC4,0xB4
	DB  0xA7,0xC4,0xD1,0x99,0xD1,0xA0,0xC4,0xDB
	DB  0x8F,0x00,0xC4,0x8F,0xB4,0x82,0x99,0x82
;---------------------------------------
;TAB_0CD7:
	DB  0xEF,0xCD,0xAB,0x89,0x98,0xBA,0xDC,0xFE
;---------------------------------------



;---------------------------------------
DELAY_MS:
	MOVLW	.200			;
DELAY_PUB:
	MOVWF	TEMP0,A			;
DELAY_M0:
	MOVLW 	.200			;
	MOVWF	TEMP1,A			;
DELAY_M1:
	CLRWDT				;
	NOP				;
	NOP				;
	DECFSZ	TEMP1,F,A		;
	GOTO	DELAY_M1		;
	DECFSZ	TEMP0,F,A		;
	GOTO	DELAY_M0		;
	RETURN				;
;---------------------------------------
;---------------------------------------
DELAY_30US:
	MOVLW 	.13			;
	MOVWF	TEMP0,A			;
DELAY_30US_0:
	CLRWDT				;
	DECFSZ	TEMP0,F,A		;
	GOTO	DELAY_30US_0		;
	RETURN				;
;---------------------------------------
;---------------------------------------
DELAY_50US:
	MOVLW 	.25			;
	MOVWF	TEMP0,A			;
DELAY_50US_0:
	CLRWDT				;
	DECFSZ	TEMP0,F,A		;
	GOTO	DELAY_50US_0		;
	RETURN				;
;---------------------------------------

;---------------------------------------
IIC_WRITE:
	CALL	CAL_ADS			;
	MOVF	TEMP_EE,W,A		;
	MOVWF	TEMP_TT,A		;
IIC_WRITE_RE:
	CALL	IIC_STRAT		;

	MOVF	ADDRESS_WR_H,W		;
	MOVWF	TEMP_tx			;
	CALL	IIC_SEND		;
	CALL	IIC_RECEIVE_ACK		;
	BTFSC	STATUS,C		;
	GOTO	IIC_WRITE_RE		;

	MOVF	ADDRESS_L,W		;
	MOVWF	TEMP_tx			;
	CALL	IIC_SEND		;
	CALL	IIC_RECEIVE_ACK		;
	BTFSC	STATUS,C		;
	GOTO	IIC_WRITE_RE		;

	MOVF	TEMP_EE,W,A		;
	MOVWF	TEMP_tx,A		;
	CALL	IIC_SEND		;
	CALL	IIC_RECEIVE_ACK		;
	BTFSC	STATUS,C		;
	GOTO	IIC_WRITE_RE		;

	CALL	IIC_STOP		;

	CALL	IIC_READ_byTempEE_Tempbb		;
	MOVF	TEMP_EE,W,A		;
	XORWF	TEMP_TT,W,A		;
	BTFSS	STATUS,Z,A		;
	BRA	IIC_WRITE_RE		;
	RETURN				;

;IIC_WRITE_ER
;	BSF	BAT_CL			;

;	MOVLW	0XA0			;
;	MOVWF	ADDRESS,A		;
;	CALL	EE_WRITE		;

;	MOVLW	0XA1			;
;	MOVWF	ADDRESS,A		;
;	MOVF	TEMP_TT,W,A		;
;	MOVWF	TEMP_EE,A		;
;	CALL	EE_WRITE		;

;	MOVLW	0XA2			;
;	MOVWF	ADDRESS,A		;
;	MOVF	ADDRESS_L,W,A		;
;	MOVWF	TEMP_EE,A		;
;	CALL	EE_WRITE		;

;	GOTO	TEST2
;----------------------------------------
IIC_READ_byTempEE_Tempbb:
	CALL	CAL_ADS			;
IIC_READ_byTempEE_Tempbb_A:
	CALL	IIC_STRAT		;

	MOVF	ADDRESS_WR_H,W		;
	MOVWF	TEMP_tx,A		;
	CALL	IIC_SEND		;
	CALL	IIC_RECEIVE_ACK		;
	BTFSC	STATUS,C,A		;
	GOTO	IIC_READ_byTempEE_Tempbb_A		;

	MOVF	ADDRESS_L,W,A		;
	MOVWF	TEMP_tx,A		;
	CALL	IIC_SEND		;
	CALL	IIC_RECEIVE_ACK		;
	BTFSC	STATUS,C,A		;
	GOTO	IIC_READ_byTempEE_Tempbb_A		;
;
IIC_READ_byTempEE_Tempbb_B:
	CALL	IIC_STRAT		;
	MOVF	ADDRESS_RD_H,W		;
	MOVWF	TEMP_tx			;
	CALL	IIC_SEND		;
	CALL	IIC_RECEIVE_ACK		;
	BTFSC	STATUS,C,A		;
	GOTO	IIC_READ_byTempEE_Tempbb_B		;

	CALL	IIC_RECIVE		;
;	CALL	IIC_SEND_ACK		;
	CALL	IIC_RECEIVE_ACK		;
	MOVF	TEMP_EE,W,A		;
	MOVWF	TEMP_bb,A		;
	CALL	IIC_STOP		;
;
	RETURN				;
;----------------------------------------
IIC_STRAT:
	BSF	IIC_SCL			;
	NOP				;
	NOP				;
	BSF	IIC_SDA			;
	NOP				;
	NOP				;
	NOP				;
	NOP				;
	BCF	IIC_SDA			;
	NOP				;
	NOP				;
	NOP				;
	NOP				;
	BCF	IIC_SCL			;
	RETURN				;
;----------------------------------------
IIC_STOP:
	BCF	IIC_SDA			;
	NOP				;
	NOP				;
	NOP				;
	NOP				;

	BSF	IIC_SCL			;
	NOP				;
	NOP				;
	NOP				;
	NOP				;

	BSF	IIC_SDA			;
	NOP				;
	NOP				;
	NOP				;
	NOP				;
	NOP				;
	NOP				;
	NOP				;
	NOP				;

	RETURN				;
;----------------------------------------
CAL_ADS:
;	RLCF	ADDRESS_H,W		;
;	MOVWF	ADDRESS_WR_H		;
;	MOVLW	B'00001110'		;24C16A
;	MOVLW	B'00000110'		;24C08
;	MOVLW	B'00000000'		;24C02
;	ANDWF	ADDRESS_WR_H,F		;

;	MOVF	ADDRESS_WR_H,W		;
;	MOVWF	ADDRESS_RD_H		;

;	MOVLW	B'10100000'		;
;	IORWF	ADDRESS_WR_H,F		;

;	MOVLW	B'10100001'		;
;	IORWF	ADDRESS_RD_H,F		;

	MOVLW	B'10100000'		;
	MOVWF	ADDRESS_WR_H		;

	MOVLW	B'10100001'		;
	MOVWF	ADDRESS_RD_H		;

	MOVF	ADDRESS,W,A		;add to change EE
	MOVWF	ADDRESS_L,A		;

	RETURN				;
;----------------------------------------
IIC_SEND:
	CLRWDT				;

	MOVLW	.8			;
	MOVWF	CNT_e0,A		;
IIC_SEND_LP:
	BCF	IIC_SCL			;
	CLRWDT				;
	RLCF	TEMP_tx,F,A		;

	BCF	IIC_SDA			;
	BTFSC	STATUS,C		;
	BSF	IIC_SDA			;

	BSF	IIC_SCL			;
	NOP				;
	NOP				;
	NOP				;
	NOP				;
	NOP				;
	NOP				;
	NOP				;
	NOP				;

	DECFSZ	CNT_e0,F,A		;
	GOTO	IIC_SEND_LP		;

	BCF	IIC_SCL			;

	RETURN				;
;----------------------------------------
IIC_RECIVE:
	MOVLW	.8			;
	MOVWF	CNT_e0,A		;

	MOVLW	n_TRISB1		;
	MOVWF	TRISB,A			;

;	BSF	IIC_SDA			;
IIC_RECIVE_LP:
	BSF	IIC_SCL			;
	NOP				;
	NOP				;
	NOP				;
	NOP				;

	BCF	STATUS,C,A		;
	BTFSC	IIC_SDA			;
	BSF	STATUS,C,A		;
	RLCF	TEMP_EE,F,A		;

	BCF	IIC_SCL			;
	NOP				;
	NOP				;
	NOP				;
	NOP				;

	DECFSZ	CNT_e0,F,A		;
	GOTO	IIC_RECIVE_LP		;

	MOVLW	n_TRISB0		;
	MOVWF	TRISB,A			;
	RETURN				;
;----------------------------------------
IIC_RECEIVE_ACK:
	BCF	IIC_SCL			;
	NOP				;
	NOP				;
	BSF	IIC_SDA			;

	NOP				;
	NOP				;
	NOP				;
	NOP				;

	MOVLW	n_TRISB1		;
	MOVWF	TRISB,A			;
	BSF	IIC_SCL			;
	NOP				;
	NOP				;
	NOP				;
	NOP				;
	NOP				;
	NOP				;
	NOP				;
	NOP				;

	BCF	STATUS,C,A		;
	BTFSC	IIC_SDA			;
	BSF	STATUS,C,A		;

	BCF	IIC_SCL			;

	MOVLW	n_TRISB0		;
	MOVWF	TRISB,A			;

	RETURN				;
;----------------------------------------
;IIC_SEND_ACK:
;	MOVLW	.1			;
;	XORWF	CNT_e1,W,A		;

;;	BSF	IIC_SDA			;
;;	BTFSS	STATUS,Z,A		;
;;	BCF	IIC_SDA			;

;	BTFSS	STATUS,Z,A		;
;	BRA	ACK_Y			;
;ACK_N:
;	BSF	IIC_SDA			;
;	BRA	ACK_E			;
;ACK_Y:
;	BCF	IIC_SDA			;
;ACK_E:
;	BSF	IIC_SCL			;
;	NOP				;
;	NOP				;
;	NOP				;
;	NOP				;
;	NOP				;
;	NOP				;
;	NOP				;
;	NOP				;

;	BCF	IIC_SCL			;
;	RETURN				;
;----------------------------------------
EEtoIIC:
	BSF	BAT_CL			;

;	MOVLW	0x00			;
;	MOVWF	ADDRESS_H,A		;
	MOVLW	0x00			;0x00
	MOVWF	ADDRESS_L,A		;
	MOVLW	0x00			;0x00
	MOVWF	ADDRESS,A		;
	MOVLW	.160			;160
	MOVWF	CNT0,A			;
LP_EtoI:
	CALL	EE_READ			;
	CALL	IIC_WRITE		;

	INCF	ADDRESS_L,F,A		;
	INCF	ADDRESS,F,A		;
	DECFSZ	CNT0,F,A		;
	BRA	LP_EtoI			;

INDI_E
	BSF	LED			;
	CALL	DELAY_MS		;
	BCF	LED			;
	CALL	DELAY_MS		;
	BSF	LED			;
	CALL	DELAY_MS		;
	BCF	LED			;
	CALL	DELAY_MS		;
	BCF	BAT_CL			;

LP_EI0
	CLRWDT				;
	BRA	LP_EI0			;
;----------------------------------------
IICtoEE:
	BSF	BAT_CL			;

;	MOVLW	0x00			;
;	MOVWF	ADDRESS_H,A		;
	MOVLW	0x00			;
	MOVWF	ADDRESS_L,A		;
	MOVLW	0x00			;
	MOVWF	ADDRESS,A		;
	MOVLW	.160			;
	MOVWF	CNT0,A			;

;	MOVLW	.1			;
;	MOVWF	CNT_e1,A		;
LP_ItoE:
	CALL	IIC_READ_byTempEE_Tempbb		;
	CALL	EE_WRITE		;

	INCF	ADDRESS_L,F,A		;
	INCF	ADDRESS,F,A		;
	DECFSZ	CNT0,F,A		;
	BRA	LP_ItoE			;

	BRA	INDI_E			;
;----------------------------------------
;-------------------------------------------------
	ORG	0xF00000
;-------------------------------------------------
;---------------------------------------
;00-0F
	DE 0x00,	0x47,	0x80,	0x22,	0x71,	0x85,	0x87,	0x2F
	DE 0x9B,	0xCB,	0x61,	0x90,	0x8F,	0xE3,	0x74,	0x90
;10-1F
	DE 0xA4,	0xA1,	0x0D,	0x94,	0x2F,	0x47,	0xFD,	0xF6
	DE 0x60,	0xDD,	0xC6,	0xC2,	0xD4,	0xD6,	0xFB,	0xB3
;20-2F
	DE 0xC8,	0xB3,	0x58,	0x91,	0x80,	0xCD,	0xE0,	0x27
	DE 0x5D,	0x6A,	0xA0,	0xD9,	0xDF,	0x73,	0xFE,	0xF1
;30-3F
	DE 0x50,	0x5D,	0xFA,	0x2C,	0x2C,	0x2E,	0xA5,	0xE5
	DE 0xA8,	0x69,	0xED,	0x15,	0x21,	0xF4,	0x0C,	0x2F
;40-4F
	DE 0x2A,	0xEF,	0xCE,	0x93,	0x03,	0xE9,	0xC0,	0xA7
	DE 0xE2,	0x1E,	0xC3,	0x1A,	0x77,	0x86,	0x65,	0xB8
;50-5F
	DE 0xAB,	0x7D,	0xD2,	0x1F,	0x2D,	0xFE,	0xC7,	0x84
	DE 0x9B,	0x1C,	0xDF,	0xF7,	0xDE,	0x7C,	0xB0,	0xC4
;60-6F
	DE 0xF1,	0x15,	0xFB,	0x45,	0x14,	0xA8,	0x79,	0x78
	DE 0x23,	0x7F,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF
;70-7F
	DE 0xFF,	0xFF,	0xFF,	0x4F,	0x8F,	0xEC,	0x14,	0x8E
	DE 0x69,	0x86,	0xE5,	0x7F,	0xFF,	0x00,	0x00,	0x02
;80-8F
;	DE 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87
;	DE 0x55,	0x24,	0x29,	0x2C,	0xCB,	0x61,	0x90,	0x8F
;90-9F
;	DE 0x01,	0x01,	0x01,	0x01,	0xFF,	0xFF,	0xFF,	0xFF
;	DE 0x73,	0x0C,	0xFF,	0xFF,	0xFF,	0xFF,	0x21,	0xdf
;80-8F
	DE 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	DE 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
;90-9F
	DE 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	DE 0xFF, 0x02, 0xFE, 0x00, 0x51, 0x68, 0x21, 0xDF
;---------------------------------------
;-------------------------------------------------
	ORG	0xF000A0
;-------------------------------------------------
;---------------------------------------
;A0-AF
	DE 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	DE 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
;B0-BF
	DE 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	DE 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
;C0-CF
	DE 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	DE 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
;D0-DF
	DE 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	DE 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
;E0-EF
	DE 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	DE 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
;F0-FF
	DE 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	DE 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
;---------------------------------------
;--------------------------------------------------------
	END
;--------------------------------------------------------
