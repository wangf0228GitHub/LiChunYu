#ifndef   __FlagDefine_h__
#define   __FlagDefine_h__
/************************************************************************/
/* 第一个为最低位                                                       */
/************************************************************************/
typedef union   
{
	struct
	{
		unsigned bRx:1;
		unsigned bSame:1;//比较用，判断是否相同
		unsigned bSleep:1;
		unsigned bAlarm:1;//高地位同时追步
	};
	unsigned char AllFlag;
} GFlags;        // general flags
#endif
