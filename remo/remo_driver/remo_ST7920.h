
#ifndef _remo_ST7920_h_
#define _remo_ST7920_h_

#include "remo_typedef.h"


void remo_ST7920_SetPin( uint8 rs, uint8 rw, uint8 e );
void remo_ST7920_SetValue( uint8 v );
void remo_ST7920_Init( void );
void remo_ST7920_write_command( uint8 C );
void remo_ST7920_write_data( uint8 D );


#endif
