
#ifndef _remo_SHT1106_DCSPI_h_
#define _remo_SHT1106_DCSPI_h_

#include "remo_typedef.h"


void remo_SHT1106_DCSPI_SetPin( uint8 Type, uint8 SCL, uint8 SI, uint8 CS, uint8 DC );
void remo_SHT1106_DCSPI_SetValue( uint8 v );
void remo_SHT1106_DCSPI_Init( void );
void remo_SHT1106_DCSPI_write_command( uint8 C );
void remo_SHT1106_DCSPI_write_data( uint8 D );


#endif
