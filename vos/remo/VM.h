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

#ifndef _VM_h_
#define _VM_h_

#include "remo_typedef.h"

extern uint32 PC;

void VM_Reset( void );
void VM_Run( void );
uint8 VM_GetInterrupt( void );
void VM_Interrupt( void );
void VM_AddInterruptEvent( uint8 d );
void VM_ClearSysValue( void );

uint8 Mem_Get_uint8( uint16 Addr );

extern uint32 ReadData;
extern bool WriteNotRead;
extern bool Running;
extern bool EA;
extern uint8 myTimerEnable;


#endif
