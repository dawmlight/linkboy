
#include "remo_typedef.h"

#include "remo_SoftDelay.h"
#include "remo_device.h"

#include "remo_ST7920.h"

//#define delay(); SoftDelay_10us( 1 );
void delay()
{
	for( int16 i = 0; i < 500; i++ ) {

	}
}

//---------------------------------------------------------
void remo_ST7920_delay(void);
void PORT_init(void);
void SendByte( uint8 b );
//---------------------------------------------------------

uint8 remo_ST7920_CS;
uint8 remo_ST7920_SID;
uint8 remo_ST7920_SCK;

void remo_ST7920_SetPin( uint8 vRS, uint8 vRW, uint8 vE )
{
	remo_ST7920_CS = vRS;
	remo_ST7920_SID = vRW;
	remo_ST7920_SCK = vE;
}

void remo_ST7920_SetValue( uint8 v )
{
	//...
}

void remo_ST7920_Init( void )
{
	//端口初始化
	PORT_init();
	
	SoftDelay_ms( 10 );

	remo_ST7920_write_command( 0x30 );
	SoftDelay_ms( 10 );
	remo_ST7920_write_command( 0x30 );
	SoftDelay_ms( 10 );
	remo_ST7920_write_command( 0x0c );
	SoftDelay_ms( 10 );
	remo_ST7920_write_command( 0x01 );
	SoftDelay_ms( 10 );
	remo_ST7920_write_command( 0x06 );
	
	SoftDelay_ms( 100 );
	
	//转换到图形模式
	remo_ST7920_write_command( 0x34 );

	SoftDelay_ms( 10 );

	remo_ST7920_write_command( 0x36 );

	SoftDelay_ms( 10 );
}
void PORT_init( void )
{
	//端口初始化
	IO_DirWrite( remo_ST7920_CS, 1 );
	IO_DirWrite( remo_ST7920_SID, 1 );
	IO_DirWrite( remo_ST7920_SCK, 1 );

	IO_OutWrite( remo_ST7920_CS, 1 );
	IO_OutWrite( remo_ST7920_SID, 1 );
	IO_OutWrite( remo_ST7920_SCK, 0 );
}
void remo_ST7920_write_command( uint8 C )
{
	//---------------------------------------------------
	IO_OutWrite( remo_ST7920_SID, 1 );
	delay();
	for( uint8 i = 0; i < 5; ++i ) {
		IO_OutWrite( remo_ST7920_SCK, 1 );
		delay();
		IO_OutWrite( remo_ST7920_SCK, 0 );
		delay();
	}
	//---------------------------------------------------
	IO_OutWrite( remo_ST7920_SID, 0 );
	delay();
	IO_OutWrite( remo_ST7920_SCK, 1 );
	delay();
	IO_OutWrite( remo_ST7920_SCK, 0 );
	delay();
	//---------------------------------------------------
	IO_OutWrite( remo_ST7920_SID, 0 );
	delay();
	IO_OutWrite( remo_ST7920_SCK, 1 );
	delay();
	IO_OutWrite( remo_ST7920_SCK, 0 );
	delay();
	//---------------------------------------------------
	SendByte( C );
}
void remo_ST7920_write_data( uint8 D )
{
	IO_OutWrite( remo_ST7920_SID, 1 );
	for( uint8 i = 0; i < 5; ++i ) {
		IO_OutWrite( remo_ST7920_SCK, 1 );
		delay();
		IO_OutWrite( remo_ST7920_SCK, 0 );
		delay();
	}
	
	IO_OutWrite( remo_ST7920_SID, 0 );
	delay();
	IO_OutWrite( remo_ST7920_SCK, 1 );
	delay();
	IO_OutWrite( remo_ST7920_SCK, 0 );
	delay();

	IO_OutWrite( remo_ST7920_SID, 1 );
	delay();
	IO_OutWrite( remo_ST7920_SCK, 1 );
	delay();
	IO_OutWrite( remo_ST7920_SCK, 0 );
	delay();

	SendByte( D );
}
void SendByte( uint8 b )
{
	IO_OutWrite( remo_ST7920_SID, 0 );
	delay();
	IO_OutWrite( remo_ST7920_SCK, 1 );
	delay();
	IO_OutWrite( remo_ST7920_SCK, 0 );
	delay();

	for( uint8 i = 0; i < 4; ++i ) {
		IO_OutWrite( remo_ST7920_SID, b & 0x80 );
		b <<= 1;
		IO_OutWrite( remo_ST7920_SCK, 1 );
		delay();
		IO_OutWrite( remo_ST7920_SCK, 0 );
		delay();
	}

	IO_OutWrite( remo_ST7920_SID, 0 );
	delay();
	for( uint8 i = 0; i < 4; ++i ) {
		IO_OutWrite( remo_ST7920_SCK, 1 );
		delay();
		IO_OutWrite( remo_ST7920_SCK, 0 );
		delay();
	}

	for( uint8 i = 0; i < 4; ++i ) {
		IO_OutWrite( remo_ST7920_SID, b & 0x80 );
		b <<= 1;
		IO_OutWrite( remo_ST7920_SCK, 1 );
		delay();
		IO_OutWrite( remo_ST7920_SCK, 0 );
		delay();
	}

	IO_OutWrite( remo_ST7920_SID, 0 );
	delay();
	for( uint8 i = 0; i < 4; ++i ) {
		IO_OutWrite( remo_ST7920_SCK, 1 );
		delay();
		IO_OutWrite( remo_ST7920_SCK, 0 );
		delay();
	}

	IO_OutWrite( remo_ST7920_SID, 1 );
}
void remo_ST7920_delay()
{
	//SoftDelay_10us( 10 );
}




