#ifndef   __FlagDefine_h__
#define   __FlagDefine_h__
/************************************************************************/
/* 第一个为最低位                                                       */
/************************************************************************/
typedef union   
{
	struct
	{
		unsigned temp0:1;
		unsigned temp1:1;
		unsigned power:1;
		unsigned ONbuzh:1;//捕捉开始
		unsigned ONcal:1;//开始计算
		unsigned ONrx:1;
		unsigned rxok:1;
		unsigned RDee:1;//'0'read 24C02,'1'read EEPROM
	};
	unsigned char AllFlag;
} _f20;        // general flags
typedef union   
{
	struct
	{
		unsigned over0:1;
		unsigned over1:1;//'1' is not received,'0' is received.
		unsigned eeFF:1;
		unsigned head:1;
		unsigned flgS3:1;
		unsigned RST:1;
		unsigned swChg:1;
	};
	unsigned char AllFlag;
} _f21;        // general flags

typedef union   
{
	struct
	{
		unsigned b0:1;
		unsigned b1:1;
		unsigned b2:1;
		unsigned b3:1;
		unsigned b4:1;
		unsigned b5:1;
		unsigned b6:1;
		unsigned b7:1;
	};
	unsigned char Byte;
} _fByte; 

typedef union 
{
	unsigned char RAMBUF[222];

	struct 
	{
		unsigned char RAM_22	;
		unsigned char RAM_23	;
		unsigned char RAM_24	;
		unsigned char RAM_25	;
		unsigned char RAM_26	;
		unsigned char RAM_27	;
		unsigned char RAM_28	;
		unsigned char RAM_29	;
		unsigned char RAM_2A	;
		unsigned char RAM_2B	;
		unsigned char RAM_2C	;
		unsigned char RAM_2D	;
		unsigned char RAM_2E	;
		unsigned char RAM_2F	;
		unsigned char RAM_30	;
		unsigned char RAM_31	;
		unsigned char RAM_32	;
		unsigned char RAM_33	;
		unsigned char RAM_34	;
		unsigned char RAM_35	;
		unsigned char RAM_36	;
		unsigned char RAM_37	;
		unsigned char RAM_38	;
		unsigned char RAM_39	;
		unsigned char RAM_3A	;
		unsigned char RAM_3B	;
		_fByte RAM_3C;
		unsigned char RAM_3D	;
		unsigned char RAM_3E	;
		unsigned char RAM_3F	;
		unsigned char RAM_40	;
		unsigned char RAM_41	;
		unsigned char RAM_42	;
		unsigned char RAM_43	;
		unsigned char RAM_44	;
		unsigned char RAM_45	;
		unsigned char RAM_46	;
		unsigned char RAM_47	;
		unsigned char RAM_48	;
		unsigned char RAM_49	;
		unsigned char RAM_4A	;
		unsigned char RAM_4B	;
		unsigned char RAM_4C	;
		unsigned char RAM_4D	;
		unsigned char RAM_4E	;
		unsigned char RAM_4F	;
		unsigned char RAM_50	;
		unsigned char RAM_51	;
		unsigned char RAM_52	;
		unsigned char RAM_53	;
		unsigned char RAM_54	;
		unsigned char RAM_55	;
		unsigned char RAM_56	;
		unsigned char RAM_57	;
		unsigned char RAM_58	;
		unsigned char RAM_59	;
		unsigned char RAM_5A	;
		unsigned char RAM_5B	;
		unsigned char RAM_5C	;
		unsigned char RAM_5D	;
		unsigned char RAM_5E	;
		unsigned char RAM_5F	;
		unsigned char RAM_60	;
		unsigned char RAM_61	;
		unsigned char RAM_62	;
		unsigned char RAM_63	;
		unsigned char RAM_64	;
		unsigned char RAM_65	;
		unsigned char RAM_66	;
		unsigned char RAM_67	;
		unsigned char RAM_68	;
		unsigned char RAM_69	;
		unsigned char RAM_6A	;
		unsigned char RAM_6B	;
		unsigned char RAM_6C	;
		unsigned char RAM_6D	;
		unsigned char RAM_6E	;
		unsigned char RAM_6F	;
		unsigned char RAM_70	;
		unsigned char RAM_71	;
		unsigned char RAM_72	;
		unsigned char RAM_73	;
		unsigned char RAM_74	;
		unsigned char RAM_75	;
		unsigned char RAM_76	;
		unsigned char RAM_77	;
		unsigned char RAM_78	;
		unsigned char RAM_79	;
		unsigned char RAM_7A	;
		unsigned char RAM_7B	;
		unsigned char RAM_7C	;
		unsigned char RAM_7D	;
		unsigned char RAM_7E	;
		unsigned char RAM_7F	;
		unsigned char RAM_80	;
		unsigned char RAM_81	;
		unsigned char RAM_82	;
		unsigned char RAM_83	;
		unsigned char RAM_84	;
		unsigned char RAM_85	;
		unsigned char RAM_86	;
		unsigned char RAM_87	;
		unsigned char RAM_88	;
		unsigned char RAM_89	;
		unsigned char RAM_8A	;
		unsigned char RAM_8B	;
		unsigned char RAM_8C	;
		unsigned char RAM_8D	;
		unsigned char RAM_8E	;
		unsigned char RAM_8F	;
		unsigned char RAM_90	;
		unsigned char RAM_91	;
		unsigned char RAM_92	;
		unsigned char RAM_93	;
		unsigned char RAM_94	;
		unsigned char RAM_95	;
		unsigned char RAM_96	;
		unsigned char RAM_97	;
		unsigned char RAM_98	;
		unsigned char RAM_99	;
		unsigned char RAM_9A	;
		unsigned char RAM_9B	;
		unsigned char RAM_9C	;
		unsigned char RAM_9D	;
		unsigned char RAM_9E	;
		unsigned char RAM_9F	;
		unsigned char RAM_A0	;
		unsigned char RAM_A1	;
		unsigned char RAM_A2	;
		unsigned char RAM_A3	;//最终接收字节
		unsigned char RAM_A4	;//脉冲宽度
		unsigned char RAM_A5	;
		unsigned char RAM_A6	;
		unsigned char RAM_A7	;
		unsigned char RAM_A8	;
		unsigned char RAM_A9	;
		unsigned char RAM_AA	;
		unsigned char RAM_AB	;
		unsigned char RAM_AC	;
		unsigned char RAM_AD	;
		unsigned char RAM_AE	;
		unsigned char RAM_AF	;
		unsigned char RAM_B0	;
		unsigned char RAM_B1	;
		unsigned char RAM_B2	;
		unsigned char RAM_B3	;
		unsigned char RAM_B4	;
		unsigned char RAM_B5	;
		unsigned char RAM_B6	;
		unsigned char RAM_B7	;
		unsigned char RAM_B8	;
		unsigned char RAM_B9	;
		unsigned char RAM_BA	;
		unsigned char RAM_BB	;
		unsigned char RAM_BC	;
		unsigned char RAM_BD	;
		unsigned char RAM_BE	;
		unsigned char RAM_BF	;
		unsigned char RAM_C0	;
		unsigned char RAM_C1	;
		unsigned char RAM_C2	;//上一帧命令字
		unsigned char RAM_C3	;
		unsigned char RAM_C4	;
		unsigned char RAM_C5	;
		unsigned char RAM_C6	;
		unsigned char RAM_C7	;
		unsigned char RAM_C8	;
		_fByte fC9;
		_fByte fCA;//存储器中0x9e地址的数据
		
		_fByte fCB;
		unsigned char RAM_CC	;
		unsigned char RAM_CD	;
		unsigned char RAM_CE	;
		unsigned char RAM_CF	;
		unsigned char RAM_D0	;
		unsigned char RAM_D1	;
		unsigned char RAM_D2	;
		_fByte fD3	;
		unsigned char RAM_D4	;
		unsigned char RAM_D5	;
		unsigned char RAM_D6	;
		unsigned char RAM_D7	;
		unsigned char RAM_D8	;
		unsigned char RAM_D9	;
		unsigned char RAM_DA	;
		unsigned char RAM_DB	;
		unsigned char RAM_DC	;
		unsigned char RAM_DD	;
		unsigned char RAM_DE	;
		unsigned char RAM_DF	;
		unsigned char RAM_E0	;
		unsigned char RAM_E1	;
		unsigned char RAM_E2	;
		unsigned char RAM_E3	;
		unsigned char RAM_E4	;
		unsigned char RAM_E5	;
		unsigned char RAM_E6	;
		unsigned char RAM_E7	;
		unsigned char RAM_E8	;
		unsigned char RAM_E9	;
		unsigned char RAM_EA	;
		unsigned char RAM_EB	;
		unsigned char RAM_EC	;
		unsigned char RAM_ED	;
		unsigned char RAM_EE	;
		unsigned char RAM_EF	;
		unsigned char RAM_F0	;
		unsigned char RAM_F1	;
		unsigned char RAM_F2	;
		unsigned char RAM_F3	;
		unsigned char RAM_F4	;
		unsigned char RAM_F5	;
		unsigned char RAM_F6	;
		unsigned char RAM_F7	;
		unsigned char RAM_F8	;
		unsigned char RAM_F9	;
		unsigned char RAM_FA	;
		unsigned char RAM_FB	;
		unsigned char RAM_FC	;
		unsigned char RAM_FD	;
		unsigned char RAM_FE	;
		unsigned char RAM_FF	;


	};
}_RAM;
#endif
