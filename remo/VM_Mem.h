
#ifndef _Mem_h_
#define _Mem_h_

#include "remo_typedef.h"

void Mem_Clear( void );

//uint8 Mem_GetCode_uint8( uint16 Addr );
uint16 Mem_GetCode_uint16( uint16 Addr );
uint32 Mem_GetCode_uint32( uint16 Addr );
//void Mem_Set_uint8( uint16 Addr, uint8 d );
void Mem_Set_uint16( uint16 Addr, uint16 d );
void Mem_Set_uint32( uint16 Addr, uint32 d );
//uint8 Mem_Get_uint8( uint16 Addr );
uint16 Mem_Get_uint16( uint16 Addr );
uint32 Mem_Get_uint24( uint16 Addr );
uint32 Mem_Get_uint32( uint16 Addr );
//void Mem_Set_int8( uint16 Addr, int8 d );
void Mem_Set_int16( uint16 Addr, int16 dd );
void Mem_Set_int32( uint16 Addr, int32 dd );
int32 Mem_Get_int8( uint16 Addr );
int32 Mem_Get_int16( uint16 Addr );
int32 Mem_Get_int32( uint16 Addr );
uint8* Mem_GetAddr( uint32 addr );

#endif

