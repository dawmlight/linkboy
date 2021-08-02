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
//CC指电路仿真引擎, 0表示不包含, 其他数值表示包含
const uint8 DEV_MESSAGE[] = "CPU:HI3861,FREQ:160,CC:0";

//-------------------------------------
//配置RAM尺寸
#define C_remo_SYS_BASE_LENGTH 4096 //30K

//-------------------------------------

#define Flash_PageSize 1024
#define Flash_StartPage 28
#define Flash_PageNumber 100

//#define remo_CLOCK_M 160

#include "remo_Device\Common.c"
#include "remo_Device\hi3861.c"

//========================================================================================

//短延时, 后续应该尽量换成 nop 指令
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
	
	//以下针对 E103CBT6 120MHz
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


