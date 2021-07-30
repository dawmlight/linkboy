
#ifndef _remo_I2C_h_
#define _remo_I2C_h_

#include "remo_typedef.h"


void remo_I2C_SetPin( uint8 SCL, uint8 SDA );
void remo_I2C_Init(void);
void remo_I2C_write_byte( uint8 data );
void remo_I2C_start_iic(void);
void remo_I2C_end_iic(void);
void remo_I2C_test( void );
void remo_I2C_test1( int32 d );



#endif
