
#ifndef _proto_h_
#define _proto_h_

#include "remo_typedef.h"

extern bool Running;
extern bool Write4Byte;

void setup0( void );
void setup( void );
void loop( void );
//int16 Proto_GetData( uint8 id );
void Deal( uint8 d );
void Error( uint32 ET, uint32 EC );
void Debug( uint32 data );

#endif



