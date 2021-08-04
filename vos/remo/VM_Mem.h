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

