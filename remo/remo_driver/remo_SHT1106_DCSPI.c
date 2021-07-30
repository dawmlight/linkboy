
#include "remo_typedef.h"

#include "remo_SoftDelay.h"
#include "remo_device.h"

#include "remo_SHT1106_DCSPI.h"

//---------------------------------------------------------
void remo_SHT1106_DCSPI_delay(void);
//---------------------------------------------------------

uint8 Type;
uint8 SCL;
uint8 SI;
uint8 CS;
uint8 DC;

void remo_SHT1106_DCSPI_SetPin( uint8 vType, uint8 vSCL, uint8 vSI, uint8 vCS, uint8 vDC )
{
	Type = vType;
	SCL = vSCL;
	SI = vSI;
	CS = vCS;
	DC = vDC;
}

void remo_SHT1106_DCSPI_SetValue( uint8 v )
{
	//...
}

void remo_SHT1106_DCSPI_Init( void )
{
	//端口初始化
	IO_DirWrite( SCL, 1 );
	IO_DirWrite( SI, 1 );
	IO_DirWrite( CS, 1 );
	IO_DirWrite( DC, 1 );
	
	IO_OutWrite( SCL, 1 );
	IO_OutWrite( SI, 0 );
	//IO_OutWrite( CS, 0 ); /这里不需要再设置了
	IO_OutWrite( DC, 0 );
	
	SoftDelay_ms( 10 );
	
	//OLED屏幕初始化
	if( Type == 0 ) {
		//软件初始化
		remo_SHT1106_DCSPI_write_command( 0xae );//--turn off oled panel
		remo_SHT1106_DCSPI_write_command( 0x00 );//---set low column address
		remo_SHT1106_DCSPI_write_command( 0x10 );//---set high column address
		remo_SHT1106_DCSPI_write_command( 0x40 );//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
		remo_SHT1106_DCSPI_write_command( 0x81 );//--set contrast control register
		remo_SHT1106_DCSPI_write_command( 0xcf );// Set SEG Output Current Brightness
		remo_SHT1106_DCSPI_write_command( 0xa1 );//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
		remo_SHT1106_DCSPI_write_command( 0xc8 );//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
		remo_SHT1106_DCSPI_write_command( 0xa6 );//--set normal display
		remo_SHT1106_DCSPI_write_command( 0xa8 );//--set multiplex ratio(1 to 64)
		remo_SHT1106_DCSPI_write_command( 0x3f );//--1/64 duty
		remo_SHT1106_DCSPI_write_command( 0xd3 );//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
		remo_SHT1106_DCSPI_write_command( 0x00 );//-not offset
		remo_SHT1106_DCSPI_write_command( 0xd5 );//--set display clock divide ratio/oscillator frequency
		remo_SHT1106_DCSPI_write_command( 0x80 );//--set divide ratio, Set Clock as 100 Frames/Sec
		remo_SHT1106_DCSPI_write_command( 0xd9 );//--set pre-charge period
		remo_SHT1106_DCSPI_write_command( 0xf1 );//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
		remo_SHT1106_DCSPI_write_command( 0xda );//--set com pins hardware configuration
		remo_SHT1106_DCSPI_write_command( 0x12 );
		remo_SHT1106_DCSPI_write_command( 0xdb );//--set vcomh
		remo_SHT1106_DCSPI_write_command( 0x40 );//Set VCOM Deselect Level
		remo_SHT1106_DCSPI_write_command( 0x20 );//-Set Page Addressing Mode (0x00/0x01/0x02)
		remo_SHT1106_DCSPI_write_command( 0x02 );//
		remo_SHT1106_DCSPI_write_command( 0x8d );//--set Charge Pump enable/disable
		remo_SHT1106_DCSPI_write_command( 0x14 );//--set(0x10) disable
		remo_SHT1106_DCSPI_write_command( 0xa4 );// Disable Entire Display On (0xa4/0xa5)
		remo_SHT1106_DCSPI_write_command( 0xa6 );// Disable Inverse Display On (0xa6/a7)
		remo_SHT1106_DCSPI_write_command( 0xaf );//--turn on oled panel
	}
	//ST7565屏幕初始化
	if( Type == 1 ) {
		//软件初始化
		remo_SHT1106_DCSPI_write_command( 0xaf ); //ON DISPLAY
		remo_SHT1106_DCSPI_write_command( 0x40 ); //STAR DISPLAY
		remo_SHT1106_DCSPI_write_command( 0xa0 ); //ADC NORMAL
		remo_SHT1106_DCSPI_write_command( 0xa6 ); //
		remo_SHT1106_DCSPI_write_command( 0xa4 ); //CLEAR
		remo_SHT1106_DCSPI_write_command( 0xa2 ); //1/9BIAS
		remo_SHT1106_DCSPI_write_command( 0xc8 ); //COMMON OUTPUT DIRECTION
		remo_SHT1106_DCSPI_write_command( 0x2f ); //POWER CONTROL
		remo_SHT1106_DCSPI_write_command( 0x25 ); //RESISTER RATIO
		remo_SHT1106_DCSPI_write_command( 0x81 ); //VOLUM MODE SET
		remo_SHT1106_DCSPI_write_command( 0x25 ); //RESISTER RATIO
	}
	
	//其他驱动
	/*
	remo_SHT1106_DCSPI_write_command(0xAE);   //display off
	remo_SHT1106_DCSPI_write_command(0x20);	//Set Memory Addressing Mode	
	remo_SHT1106_DCSPI_write_command(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	remo_SHT1106_DCSPI_write_command(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	remo_SHT1106_DCSPI_write_command(0xc8);	//Set COM Output Scan Direction
	remo_SHT1106_DCSPI_write_command(0x00);//---set low column address
	remo_SHT1106_DCSPI_write_command(0x10);//---set high column address
	remo_SHT1106_DCSPI_write_command(0x40);//--set start line address
	remo_SHT1106_DCSPI_write_command(0x81);//--set contrast control register
	remo_SHT1106_DCSPI_write_command(0xFF);
	remo_SHT1106_DCSPI_write_command(0xa1);//--set segment re-map 0 to 127
	remo_SHT1106_DCSPI_write_command(0xa6);//--set normal display
	remo_SHT1106_DCSPI_write_command(0xa8);//--set multiplex ratio(1 to 64)
	remo_SHT1106_DCSPI_write_command(0x3F);//
	remo_SHT1106_DCSPI_write_command(0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	remo_SHT1106_DCSPI_write_command(0xd3);//-set display offset
	remo_SHT1106_DCSPI_write_command(0x00);//-not offset
	remo_SHT1106_DCSPI_write_command(0xd5);//--set display clock divide ratio/oscillator frequency
	remo_SHT1106_DCSPI_write_command(0xf0);//--set divide ratio
	remo_SHT1106_DCSPI_write_command(0xd9);//--set pre-charge period
	remo_SHT1106_DCSPI_write_command(0x22); //
	remo_SHT1106_DCSPI_write_command(0xda);//--set com pins hardware configuration
	remo_SHT1106_DCSPI_write_command(0x12);
	remo_SHT1106_DCSPI_write_command(0xdb);//--set vcomh
	remo_SHT1106_DCSPI_write_command(0x20);//0x20,0.77xVcc
	remo_SHT1106_DCSPI_write_command(0x8d);//--set DC-DC enable
	remo_SHT1106_DCSPI_write_command(0x14);//
	remo_SHT1106_DCSPI_write_command(0xaf);//--turn on oled panel 
	*/
}

void remo_SHT1106_DCSPI_write_command( uint8 C )
{
	IO_OutWrite( DC, 0 );
	for( uint8 i = 0; i < 8; ++i ) {
		if( (C & 0x80) == 0 ) {
			IO_OutWrite( SI, 0 );
		}
		else {
			IO_OutWrite( SI, 1 );
		}
		IO_OutWrite( SCL, 0 );
		C <<= 1;
		IO_OutWrite( SCL, 1 );
	}
}
void remo_SHT1106_DCSPI_write_data( uint8 D )
{
	IO_OutWrite( DC, 1 );
	for( uint8 i = 0; i < 8; ++i ) {
		if( (D & 0x80) == 0 ) {
			IO_OutWrite( SI, 0 );
		}
		else {
			IO_OutWrite( SI, 1 );
		}
		IO_OutWrite( SCL, 0 );
		D <<= 1;
		IO_OutWrite( SCL, 1 );
	}
}
void remo_SHT1106_DCSPI_delay()
{
	//SoftDelay_10us( 10 );
}




