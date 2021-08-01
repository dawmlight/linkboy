
#include "remo_typedef.h"
#include "remo_device.h"
#include "remo_I2C.h"
#include "remo_SoftDelay.h"

//---------------------------------------------------------
void remo_I2C_delay(void);
void remo_I2C_wait(void);
//---------------------------------------------------------

uint8 I2C_SCL;
uint8 I2C_SDA;

uint32 Wait_tick;

void remo_I2C_SetPin( uint8 SCL, uint8 SDA )
{
	I2C_SCL = SCL;
	I2C_SDA = SDA;
}

void remo_I2C_Init()
{
	IO_DirWrite( I2C_SCL, 1 );
	IO_DirWrite( I2C_SDA, 1 );
	
	IO_OutWrite( I2C_SCL, 1 );
	IO_OutWrite( I2C_SDA, 1 );
	
	remo_I2C_start_iic();
	remo_I2C_end_iic();
}

//开始IIC通信
void remo_I2C_start_iic()
{
	IO_OutWrite( I2C_SDA, 1 );
	IO_OutWrite( I2C_SCL, 1 );
	//remo_I2C_delay();
	IO_OutWrite( I2C_SDA, 0 );
	//remo_I2C_delay();
	IO_OutWrite( I2C_SCL, 0 );
	//remo_I2C_delay();
}
//结束IIC通信
void remo_I2C_end_iic()
{
	IO_OutWrite( I2C_SDA, 0 );
	IO_OutWrite( I2C_SCL, 1 );
	//remo_I2C_delay();
	IO_OutWrite( I2C_SDA, 1 );
	//remo_I2C_delay();
}
//等待应答信号
void remo_I2C_wait()
{
	IO_DirWrite( I2C_SDA, 0 );
	IO_OutWrite( I2C_SCL, 1 );
	Wait_tick = 0;
	while( IO_InRead( I2C_SDA ) == 1 && Wait_tick < 100000 ) { Wait_tick++; }
	IO_OutWrite( I2C_SCL, 0 );
	IO_DirWrite( I2C_SDA, 1 );
}
//写一个字节
void remo_I2C_write_byte( uint8 data )
{
	for( uint8 i = 0; i < 8; ++i ) {
		IO_OutWrite( I2C_SDA, data & 0x80 );
		data <<= 1;
		IO_OutWrite( I2C_SCL, 1 );
		IO_OutWrite( I2C_SCL, 0 );
	}
	remo_I2C_wait();
}









