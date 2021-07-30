
#include <stdio.h>

#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"

#include "iot_gpio.h"

#include "hi_io.h"
#include "hi_adc.h"
#include "hi_gpio.h"
#include "hi_uart.h"
#include "hi_time.h"
#include "hi_flash.h"
#include "hi_hrtimer.h"
#include "hi_partition_table.h"

#include "remo_typedef.h"
#include "proto.h"
#include "remo_SoftDelay.h"
#include "remo_tick.h"
#include "VM_Mem.h"

//========================================================================================

#define DEV_VERSION 'A'
//CCָ��·��������, 0��ʾ������, ������ֵ��ʾ����
const uint8 DEV_MESSAGE[] = "CPU:HI3861,FREQ:160,CC:0";

//-------------------------------------
//ע�� 32K ���ǳ���15���ܾͲ����� ��֪��Ϊʲô (����ʧ��) 2020.8.16 ������GD32Eϵ�а��ļ����ô���
//30 * 1024
//2020.7.29 ������CC����Ҫ��һ����С
#define C_remo_SYS_BASE_LENGTH 4096 //30K

//-------------------------------------

#define Flash_PageSize 1024
#define Flash_StartPage 28	//2020.3.5 ����: 28K, Ԥ��28K
#define Flash_PageNumber 100 //�õ�100K

//#define remo_CLOCK_M 160

#include "remo_Device\Common.c"
#include "remo_Device\hi3861.c"

//========================================================================================

//����ʱ, ����Ӧ�þ������� nop ָ��
volatile int32 Timer_i;
void remo_WS2812_Delay_0_4us(void)
{
	/*
	Timer_i += 1;
	Timer_i += 1;
	Timer_i += 1;
	Timer_i += 1;
	Timer_i += 1;
	*/
	
	//������� E103CBT6 120MHz
	Timer_i += 1;
	Timer_i += 1;
	//Timer_i += 1;
}

void remo_WS2812_WriteData( uint8 id, uint32 d )
{
	//GD32V-108MHz
	//F411RE-100MHz
	
	for( int8 i = 0; i < 24; ++i ) {
	
		IO_OutWrite_1( id );
		if( (d & 0x800000) == 0 ) {
			Timer_i += 1;
			//Timer_i += 1;
			//Timer_i += 1;
			//Timer_i += 1;
			IO_OutWrite_0( id );
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
		}
		else {
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			IO_OutWrite_0( id );
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			Timer_i += 1;
			//Timer_i += 1;
			//Timer_i += 1;
		}
		d <<= 1;
	}
}


