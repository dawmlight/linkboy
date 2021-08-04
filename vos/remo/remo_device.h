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

#ifndef __REMO_DEVICE_H
#define __REMO_DEVICE_H

#include "remo_typedef.h"

//================================================================

//MES
uint8 remo_SYS_DEV_VERSION( void );
const uint8* remo_SYS_DEV_MESSAGE( void );

//SYS
extern uint8 BASE[];
extern const int32 remo_SYS_BASE_LENGTH;

void remo_SYS_Init( void );
void remo_SYS_interrupt_enable( void );
void remo_SYS_interrupt_disable( void );

//USART

extern bool Serial_Data;
void remo_USART_Init(uint32 BaudRate);

bool remo_USART_isOpen( int8 ID );
void remo_USART_set( int8 ID, uint32 addr, int16 max, int32 len_addr );
void remo_USART_Baudrate( int8 ID, uint32 b );
void remo_USART_write_byte( int8 ID, uint8 tempc );

void remo_USART_write(uint8 tempc);
void remo_USART_SendString(uint8 *tempp);
void remo_USART_SendStringConst(const uint8 *tempp);

//TIMER
void Timer_Init( void );
void Timer_SetTimeMs( uint16 t );
bool Timer_isTimeOut( void );

void Timer_start_count( void );
uint32 Timer_get_count( void );
uint32 Timer_get_1us_count( void );

void Timer_Delay_0_4us(void);

//remo_IO
void IO_Init( void );
void IO_Close( void );
void IO_DirWrite( uint8 i, uint8 d );
void IO_PullWrite( uint8 i, uint8 d );
void IO_OutWrite( uint8 i, uint8 d );
void IO_OutWrite_0( uint8 i );
void IO_OutWrite_1( uint8 i );
uint8 IO_OutRead( uint8 i );
uint8 IO_InRead( uint8 i );
void IO_AnalogOpen( uint8 i, uint8 d );
int32 IO_AnalogRead( uint8 i );

//remo_Flash
void Flash_Init( void );
int32 Flash_GetSize(void);
void Flash_Unlock( void );
void Flash_Lock( void );
void Flash_Clear( void );
void Flash_Save( void );
void Flash_Load( void );
void Flash_Write2Byte( uint32 addr, uint16 b );
void Flash_Write4Byte( uint32 addr, uint32 b );
uint8 Flash_ReadByte( uint32 addr );
uint32 Flash_ReadUint32( uint32 addr );
uint8* Flash_GetAddr( uint32 addr );

//remo_DrvTimer
void remo_DrvTimer_Init( void );
void remo_DrvTimer_Start( void );
void remo_DrvTimer_Stop( void );

void remo_WS2812_Delay_0_4us(void);
void remo_WS2812_WriteData( uint8 id, uint32 d );

//================================================================
#endif

