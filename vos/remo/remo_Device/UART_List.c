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

bool Done;

uint8 Serial_Data;

static uint16 U0_buffer_addr;
static int16 U0_max_length;
static uint16 U0_length_addr;
static bool U0_isOpen;

static uint16 U1_buffer_addr;
static int16 U1_max_length;
static uint16 U1_length_addr;
static bool U1_isOpen;

static uint16 U2_buffer_addr;
static int16 U2_max_length;
static uint16 U2_length_addr;
static bool U2_isOpen;

static uint16 U3_buffer_addr;
static int16 U3_max_length;
static uint16 U3_length_addr;
static bool U3_isOpen;

void UART_List_Init()
{
	U0_max_length = 0;
	U0_length_addr = 0;
	U0_isOpen = false;
	
	U1_max_length = 0;
	U1_length_addr = 0;
	U1_isOpen = false;

	U2_max_length = 0;
	U2_length_addr = 0;
	U2_isOpen = false;

	U3_max_length = 0;
	U3_length_addr = 0;
	U3_isOpen = false;
}
bool remo_USART_isOpen( int8 ID )
{
	switch( ID ) {
		case 0:  return U0_isOpen;
		case 1:  return U1_isOpen;
		case 2:  return U2_isOpen;
		case 3:  return U3_isOpen;
		default: return false;
	}
}
void UART_List_Receive0()
{
	if( U0_max_length > 0 ) {
		int16 cl = Mem_Get_int16( U0_length_addr );
		BASE[U0_buffer_addr+cl] = Serial_Data;
		cl++;
		if( cl >= U0_max_length ) {
			cl = 0;
		}
		Mem_Set_int16( U0_length_addr, cl );
	}
}
void UART_List_Receive1()
{
	if( U1_max_length > 0 ) {
		int16 cl = Mem_Get_int16( U1_length_addr );
		BASE[U1_buffer_addr+cl] = Serial_Data;
		cl++;
		if( cl >= U1_max_length ) {
			cl = 0;
		}
		Mem_Set_int16( U1_length_addr, cl );
	}
}
void UART_List_Receive2()
{
	if( U2_max_length > 0 ) {
		int16 cl = Mem_Get_int16( U2_length_addr );
		BASE[U2_buffer_addr+cl] = Serial_Data;
		cl++;
		if( cl >= U2_max_length ) {
			cl = 0;
		}
		Mem_Set_int16( U2_length_addr, cl );
	}
}
void UART_List_Receive3()
{
	if( U3_max_length > 0 ) {
		int16 cl = Mem_Get_int16( U3_length_addr );
		BASE[U3_buffer_addr+cl] = Serial_Data;
		cl++;
		if( cl >= U3_max_length ) {
			cl = 0;
		}
		Mem_Set_int16( U3_length_addr, cl );
	}
}
void UART_List_SetAddr0( uint32 addr, int16 max, uint32 len_addr )
{
	U0_buffer_addr = addr;
	U0_max_length = max;
	U0_length_addr = len_addr;
	U0_isOpen = true;
}
void UART_List_SetAddr1( uint32 addr, int16 max, uint32 len_addr )
{
	U1_buffer_addr = addr;
	U1_max_length = max;
	U1_length_addr = len_addr;
	U1_isOpen = true;
}
void UART_List_SetAddr2( uint32 addr, int16 max, uint32 len_addr )
{
	U2_buffer_addr = addr;
	U2_max_length = max;
	U2_length_addr = len_addr;
	U2_isOpen = true;
}
void UART_List_SetAddr3( uint32 addr, int16 max, uint32 len_addr )
{
	U3_buffer_addr = addr;
	U3_max_length = max;
	U3_length_addr = len_addr;
	U3_isOpen = true;
}
//----------------------------------------------------
//proto接口函数
void remo_USART_write_byte( int8 ID, uint8 tempc );

//发送字节
void remo_USART_write( uint8 tempc )
{
	remo_USART_write_byte( UART_DEFAULT_ID, tempc );
}
//发送字符串
void remo_USART_SendString(uint8 *tempp)
{
	while( *tempp != 0 )
	{
		remo_USART_write( *tempp++ );
	}
}
//发送常量字符串
void remo_USART_SendStringConst(const uint8 *tempp)
{
	while( *tempp != 0 )
	{
		remo_USART_write( *tempp++ );
	}
}



