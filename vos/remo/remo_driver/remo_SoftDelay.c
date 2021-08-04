/*
 * Copyright (c) 2021 linkboy
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

