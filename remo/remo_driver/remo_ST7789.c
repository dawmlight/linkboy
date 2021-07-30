
#include "remo_typedef.h"

#include "remo_SoftDelay.h"
#include "remo_device.h"

#include "remo_ST7789.h"
#include "remo_tick.h"

//#define delay(); SoftDelay_10us( 1 );

//---------------------------------------------------------
void remo_ST7789_LCD_Writ_Bus(uint8 dat);
void remo_ST7789_PORT_init( void );
void remo_ST7789_SetPin( uint8 vDIN, uint8 vCLK, uint8 vRES, uint8 vDC );
void remo_ST7789_Init( int32 x, int32 y );
void remo_ST7789_LCD_WR_DATA8(uint8 dat);
void remo_ST7789_LCD_WR_DATA(uint16 dat);
void remo_ST7789_LCD_WR_REG(uint8 dat);
void remo_ST7789_LCD_Address_Set(uint16 x1,uint16 y1,uint16 x2,uint16 y2);
//---------------------------------------------------------

int32 LCD_W;
int32 LCD_H;

bool s_ForTrans;
uint16 s_ForColor;
bool s_BackTrans;
uint16 s_BackColor;

uint8 remo_ST7789_DIN;
uint8 remo_ST7789_CLK;
uint8 remo_ST7789_RES;
uint8 remo_ST7789_DC;

void remo_ST7789_SetPin( uint8 vDIN, uint8 vCLK, uint8 vRES, uint8 vDC )
{
	remo_ST7789_DIN = vDIN;
	remo_ST7789_CLK = vCLK;
	remo_ST7789_RES = vRES;
	remo_ST7789_DC = vDC;
}

void remo_ST7789_Init( int32 w, int32 h )
{
	D_pixel = remo_ST7789_LCD_draw_pixel;
	D_clear_pixel = remo_ST7789_LCD_clear_pixel;
	
	LCD_W = w;
	LCD_H = h;
	
	//端口初始化
	remo_ST7789_PORT_init();
	
	//************* Start Initial Sequence **********
	remo_ST7789_LCD_WR_REG(0x36);
	remo_ST7789_LCD_WR_DATA8(0x00);

	remo_ST7789_LCD_WR_REG(0x3A);
	remo_ST7789_LCD_WR_DATA8(0x05);

	remo_ST7789_LCD_WR_REG(0xB2);
	remo_ST7789_LCD_WR_DATA8(0x0C);
	remo_ST7789_LCD_WR_DATA8(0x0C);
	remo_ST7789_LCD_WR_DATA8(0x00);
	remo_ST7789_LCD_WR_DATA8(0x33);
	remo_ST7789_LCD_WR_DATA8(0x33);

	remo_ST7789_LCD_WR_REG(0xB7);
	remo_ST7789_LCD_WR_DATA8(0x35);

	remo_ST7789_LCD_WR_REG(0xBB);
	remo_ST7789_LCD_WR_DATA8(0x19);

	remo_ST7789_LCD_WR_REG(0xC0);
	remo_ST7789_LCD_WR_DATA8(0x2C);

	remo_ST7789_LCD_WR_REG(0xC2);
	remo_ST7789_LCD_WR_DATA8(0x01);

	remo_ST7789_LCD_WR_REG(0xC3);
	remo_ST7789_LCD_WR_DATA8(0x12);

	remo_ST7789_LCD_WR_REG(0xC4);
	remo_ST7789_LCD_WR_DATA8(0x20);

	remo_ST7789_LCD_WR_REG(0xC6);
	remo_ST7789_LCD_WR_DATA8(0x0F);

	remo_ST7789_LCD_WR_REG(0xD0);
	remo_ST7789_LCD_WR_DATA8(0xA4);
	remo_ST7789_LCD_WR_DATA8(0xA1);

	remo_ST7789_LCD_WR_REG(0xE0);
	remo_ST7789_LCD_WR_DATA8(0xD0);
	remo_ST7789_LCD_WR_DATA8(0x04);
	remo_ST7789_LCD_WR_DATA8(0x0D);
	remo_ST7789_LCD_WR_DATA8(0x11);
	remo_ST7789_LCD_WR_DATA8(0x13);
	remo_ST7789_LCD_WR_DATA8(0x2B);
	remo_ST7789_LCD_WR_DATA8(0x3F);
	remo_ST7789_LCD_WR_DATA8(0x54);
	remo_ST7789_LCD_WR_DATA8(0x4C);
	remo_ST7789_LCD_WR_DATA8(0x18);
	remo_ST7789_LCD_WR_DATA8(0x0D);
	remo_ST7789_LCD_WR_DATA8(0x0B);
	remo_ST7789_LCD_WR_DATA8(0x1F);
	remo_ST7789_LCD_WR_DATA8(0x23);

	remo_ST7789_LCD_WR_REG(0xE1);
	remo_ST7789_LCD_WR_DATA8(0xD0);
	remo_ST7789_LCD_WR_DATA8(0x04);
	remo_ST7789_LCD_WR_DATA8(0x0C);
	remo_ST7789_LCD_WR_DATA8(0x11);
	remo_ST7789_LCD_WR_DATA8(0x13);
	remo_ST7789_LCD_WR_DATA8(0x2C);
	remo_ST7789_LCD_WR_DATA8(0x3F);
	remo_ST7789_LCD_WR_DATA8(0x44);
	remo_ST7789_LCD_WR_DATA8(0x51);
	remo_ST7789_LCD_WR_DATA8(0x2F);
	remo_ST7789_LCD_WR_DATA8(0x1F);
	remo_ST7789_LCD_WR_DATA8(0x1F);
	remo_ST7789_LCD_WR_DATA8(0x20);
	remo_ST7789_LCD_WR_DATA8(0x23);

	remo_ST7789_LCD_WR_REG(0x21);
	
	remo_ST7789_LCD_WR_REG(0x11);
	//Delay (120);


	SoftDelay_ms( 10 );


	remo_ST7789_LCD_WR_REG(0x29);
}
void remo_ST7789_PORT_init( void )
{
	/*
	//端口初始化
	IO_DirWrite( remo_ST7789_DIN, 1 );
	IO_DirWrite( remo_ST7789_CLK, 1 );
	IO_DirWrite( remo_ST7789_DC, 1 );
	IO_DirWrite( remo_ST7789_RES, 1 );
	
	IO_OutWrite( remo_ST7789_CLK, 1 );
	IO_OutWrite( remo_ST7789_DC, 0 );
	IO_OutWrite( remo_ST7789_RES, 1 );
	*/
}

//函数说明：LCD清屏函数
//入口数据：无
//返回值：  无
void remo_ST7789_LCD_Clear( uint16 Color )
{
	remo_ST7789_LCD_Address_Set( 0, 0, LCD_W-1, LCD_H-1 );

	for( uint16 i=0; i<LCD_W; i+=1 ) {
		for ( uint16 j=0; j<LCD_H; j+=1 ) {
			remo_ST7789_LCD_WR_DATA(Color);
		}
	}
}
void remo_ST7789_LCD_set_fore(uint16 Color, bool t)
{
	s_ForTrans = t;
	s_ForColor = Color;
}
void remo_ST7789_LCD_set_back(uint16 Color, bool t)
{
	s_BackTrans = t;
	s_BackColor = Color;
}
void remo_ST7789_LCD_draw_pixel( int32 x, int32 y )
{
	if( s_ForTrans ) {
		return;
	}
	if( x < 0 || x >= LCD_W ) return;
	if( y < 0 || y >= LCD_H ) return;
	
	remo_ST7789_LCD_Address_Set(x,y,x,y);//设置光标位置
	remo_ST7789_LCD_WR_DATA( s_ForColor );
}
void remo_ST7789_LCD_clear_pixel( int32 x, int32 y )
{
	if( s_BackTrans ) {
		return;
	}
	if( x < 0 || x >= LCD_W ) return;
	if( y < 0 || y >= LCD_H ) return;
	
	remo_ST7789_LCD_Address_Set(x,y,x,y);//设置光标位置
	remo_ST7789_LCD_WR_DATA( s_BackColor );
}
//=================================================================================
//函数说明：LCD串行数据写入函数
//入口数据：dat  要写入的串行数据
//返回值：  无
void remo_ST7789_LCD_Writ_Bus(uint8 dat)
{
	for( uint8 i = 0; i < 8; ++i ) {
		IO_OutWrite( remo_ST7789_CLK, 0 );
		IO_OutWrite( remo_ST7789_DIN, dat & 0x80 );
		IO_OutWrite( remo_ST7789_CLK, 1 );
		dat <<= 1;
	}
}
//函数说明：LCD写入数据
//入口数据：dat 写入的数据
//返回值：  无
void remo_ST7789_LCD_WR_DATA8(uint8 dat)
{
	IO_OutWrite( remo_ST7789_DC, 1 );
	remo_ST7789_LCD_Writ_Bus(dat);
}
//函数说明：LCD写入数据
//入口数据：dat 写入的数据
//返回值：  无
void remo_ST7789_LCD_WR_DATA(uint16 dat)
{
	IO_OutWrite( remo_ST7789_DC, 1 );
	remo_ST7789_LCD_Writ_Bus( (dat >> 8) & 0xFF );
	remo_ST7789_LCD_Writ_Bus( dat & 0xFF );
}
//函数说明：LCD写入命令
//入口数据：dat 写入的命令
//返回值：  无
void remo_ST7789_LCD_WR_REG(uint8 dat)
{
	IO_OutWrite( remo_ST7789_DC, 0 );
	remo_ST7789_LCD_Writ_Bus(dat);
}
//函数说明：设置起始和结束地址
//入口数据：x1,x2 设置列的起始和结束地址
//y1,y2 设置行的起始和结束地址
//返回值：  无
void remo_ST7789_LCD_Address_Set(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
{
	remo_ST7789_LCD_WR_REG(0x2a);//列地址设置
	remo_ST7789_LCD_WR_DATA(x1);
	remo_ST7789_LCD_WR_DATA(x2);
	remo_ST7789_LCD_WR_REG(0x2b);//行地址设置
	remo_ST7789_LCD_WR_DATA(y1);
	remo_ST7789_LCD_WR_DATA(y2);
	remo_ST7789_LCD_WR_REG(0x2c);//储存器写
}




