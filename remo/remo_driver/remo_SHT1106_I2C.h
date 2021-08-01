
#ifndef _remo_SHT1106_I2C_h_
#define _remo_SHT1106_I2C_h_

#include "remo_typedef.h"


void remo_SHT1106_I2C_SetPin( uint8 SCL, uint8 SDA );
void remo_SHT1106_I2C_Init( void );
void remo_SHT1106_I2C_write_command( uint8 C );
void remo_SHT1106_I2C_write_data( uint8 D );


#endif
