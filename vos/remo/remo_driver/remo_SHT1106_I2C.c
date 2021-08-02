/*
 * Copyright (c) 2021 linkboy
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "remo_typedef.h"

#include "remo_I2C.h"
#include "remo_SoftDelay.h"

#include "remo_SHT1106_I2C.h"

//---------------------------------------------------------
void remo_SHT1106_I2C_delay(void);
//---------------------------------------------------------

void remo_SHT1106_I2C_SetPin( uint8 SCL, uint8 SDA )
{
	remo_I2C_SetPin( SCL, SDA );
}

void remo_SHT1106_I2C_Init( void )
{
	//端口初始化
	remo_I2C_Init();
	
	remo_SHT1106_I2C_delay();
	
	//软件初始化
	remo_SHT1106_I2C_write_command( 0xae );//--turn off oled panel
	remo_SHT1106_I2C_write_command( 0x00 );//---set low column address
	remo_SHT1106_I2C_write_command( 0x10 );//---set high column address
	remo_SHT1106_I2C_write_command( 0x40 );//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	remo_SHT1106_I2C_write_command( 0x81 );//--set contrast control register
	remo_SHT1106_I2C_write_command( 0xcf );// Set SEG Output Current Brightness
	remo_SHT1106_I2C_write_command( 0xa1 );//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	remo_SHT1106_I2C_write_command( 0xc8 );//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	remo_SHT1106_I2C_write_command( 0xa6 );//--set normal display
	remo_SHT1106_I2C_write_command( 0xa8 );//--set multiplex ratio(1 to 64)
	remo_SHT1106_I2C_write_command( 0x3f );//--1/64 duty
	remo_SHT1106_I2C_write_command( 0xd3 );//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	remo_SHT1106_I2C_write_command( 0x00 );//-not offset
	remo_SHT1106_I2C_write_command( 0xd5 );//--set display clock divide ratio/oscillator frequency
	remo_SHT1106_I2C_write_command( 0x80 );//--set divide ratio, Set Clock as 100 Frames/Sec
	remo_SHT1106_I2C_write_command( 0xd9 );//--set pre-charge period
	remo_SHT1106_I2C_write_command( 0xf1 );//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	remo_SHT1106_I2C_write_command( 0xda );//--set com pins hardware configuration
	remo_SHT1106_I2C_write_command( 0x12 );
	remo_SHT1106_I2C_write_command( 0xdb );//--set vcomh
	remo_SHT1106_I2C_write_command( 0x40 );//Set VCOM Deselect Level
	remo_SHT1106_I2C_write_command( 0x20 );//-Set Page Addressing Mode (0x00/0x01/0x02)
	remo_SHT1106_I2C_write_command( 0x02 );//
	remo_SHT1106_I2C_write_command( 0x8d );//--set Charge Pump enable/disable
	remo_SHT1106_I2C_write_command( 0x14 );//--set(0x10) disable
	remo_SHT1106_I2C_write_command( 0xa4 );// Disable Entire Display On (0xa4/0xa5)
	remo_SHT1106_I2C_write_command( 0xa6 );// Disable Inverse Display On (0xa6/a7)
	remo_SHT1106_I2C_write_command( 0xaf );//--turn on oled panel
}

void remo_SHT1106_I2C_write_command( uint8 C )
{
	remo_I2C_start_iic();
	remo_I2C_write_byte( 0x78 );
	remo_I2C_write_byte( 0x00 );
	remo_I2C_write_byte( C );
	remo_I2C_end_iic();
}
void remo_SHT1106_I2C_write_data( uint8 D )
{
	remo_I2C_start_iic();
	remo_I2C_write_byte( 0x78 );
	remo_I2C_write_byte( 0x40 );
	remo_I2C_write_byte( D );
	remo_I2C_end_iic();
}
void remo_SHT1106_I2C_delay()
{
	//SoftDelay_10us( 10 );
}




