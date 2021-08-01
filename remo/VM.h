
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
