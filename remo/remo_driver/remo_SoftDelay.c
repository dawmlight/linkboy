
#include "remo_typedef.h"
#include "remo_device.h"

//uint32 SoftDelay_t;
//static uint32 FREQ;

//��ʼ��
void SoftDelay_Init()
{
	//FREQ = t;
	//SoftDelay_t = FREQ / (7200000 / 7); //  /7����GD32V����ͨ��
	//����STM32F1C8T6��˵���ܳ���7. �ٿ��ǵ�tickռ��, ��������t=f/8
}

//��ʱ10΢��
//volatile int32 i_delay;
//volatile int32 j_delay;

void SoftDelay_10us( int32 nCount )
{
	//for( i_delay = 0; i_delay < x; ++i_delay ) {
	//	for( j_delay = 0; j_delay < SoftDelay_t; ++j_delay ) {}
	//}
	
	for(; nCount > 0; nCount--) {
		uint32 tus = 0;
		Timer_start_count();
		do {
			tus = Timer_get_count() / Timer_get_1us_count();
		}
		while( tus < 10 );
	}
}
//��ʱ����
void SoftDelay_ms(int32 nCount)
{
	//for(; nCount > 0; nCount--) {
	//	SoftDelay_10us( 100 );
	//}
	
	for(; nCount > 0; nCount--) {
		uint32 tus = 0;
		Timer_start_count();
		do {
			tus = Timer_get_count() / Timer_get_1us_count();
		}
		while( tus < 1000 );
	}
}
//��ʱ��
void SoftDelay_s(int32 nCount)
{
	for(; nCount > 0; nCount--) {
		SoftDelay_ms( 1000 );
	}
}

