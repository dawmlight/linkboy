

#include "remo_typedef.h"

#include "remo_device.h"
#include "remo_tick.h"
#include "remo_SoftDelay.h"
#include "remo_ST7789.h"
#include "remo_CC.h"
#include "VM_Mem.h"

//---------------------------------------------------------
void remo_tick_speaker_run(void);
void remo_tick_StepMotor_run(void);
//---------------------------------------------------------

void remo_tick_init(void)
{
	remo_tick_motor_Init();
	remo_tick_servo_init();
	remo_tick_SUART_Init();
	remo_tick_WS2812_Init();
	remo_tick_IR_init();
	remo_tick_CrossRol_init();
	remo_CC_Init();
	remo_tick_StepMotor_init();
}
//触发频率为100KHz
void remo_tick_run10us(void)
{
	//这里需要加上舵机驱动进程
	remo_tick_servo_run();

	//扬声器播放进程
	remo_tick_speaker_run();

	//软串口进程
	remo_tick_SUART_run();
}
//触发频率为10KHz
void remo_tick_run100us(void)
{
	//这里频率为10KHz

	//马达调速
	remo_tick_motor_run();

	//NEC红外遥控解码
	remo_tick_IR_run();
	
	//正交编码器
	remo_tick_CrossRol_run();
	
	//步进电机驱动器
	remo_tick_StepMotor_run();
}
//触发频率为1KHz
void remo_tick_run1000us(void)
{
	//...
}
//---------------------------------------------------------

static int32 tone_tick;
static int32 tone_time;

static uint8 status;
static bool speaker_open;
static uint8 Speak_PIN;

void remo_tick_speaker_SetPin( uint8 vDIN )
{
	speaker_open = false;
	Speak_PIN = vDIN;
}

void remo_tick_speaker_run(void)
{
	if( !speaker_open ) {
		return;
	}
	tone_tick += 100;
	if( tone_tick >= tone_time ) {
		tone_tick -= tone_time;

		//播放声音
		IO_OutWrite( Speak_PIN, status );
		if( status == 0 ) {
			status = 1;
		}
		else {
			status = 0;
		}
	}
}

void remo_tick_speaker_start(int32 t)
{
	tone_time = t;
	speaker_open = true;
}

void remo_tick_speaker_stop(void)
{
	speaker_open = false;
	//IO_OutWrite( 1, 0 );
}
//---------------------------------------------------------

static int8 motor_tick;
static bool motor_open;
static uint8 tt;
static uint8 T;

static int8 valueA;
static int8 valueB;
static uint8 DTA1;
static uint8 DTA2;
static uint8 DTB1;
static uint8 DTB2;

static uint8 M_A1;
static uint8 M_A2;
static uint8 M_B1;
static uint8 M_B2;

void remo_tick_motor_Init(void)
{
	motor_open = false;
}

void remo_tick_motor_SetPin( int32 t, uint8 a1, uint8 a2, uint8 b1, uint8 b2 )
{
	T = t;
	motor_open = true;

	valueA = 20;
	valueB = 60;

	DTA1 = 0;
	DTA2 = 1;
	DTB1 = 0;
	DTB2 = 1;

	M_A1 = a1;
	M_A2 = a2;
	M_B1 = b1;
	M_B2 = b2;
}

void remo_tick_motor_run(void)
{
	if( !motor_open ) {
		return;
	}
	tt += 1;
	if( tt < T ) {
		return;
	}
	tt = 0;

	motor_tick %= 100;
	bool ResetA = motor_tick == 0;
	bool ResetB = ResetA;

	if( motor_tick >= valueA ) {
		IO_OutWrite( M_A1, 0 );
		IO_OutWrite( M_A2, 0 );
		ResetA = false;
	}
	if( ResetA ) {
		IO_OutWrite( M_A1, DTA1 );
		IO_OutWrite( M_A2, DTA2 );
	}
	if( motor_tick >= valueB ) {
		IO_OutWrite( M_B1, 0 );
		IO_OutWrite( M_B2, 0 );
		ResetB = false;
	}
	if( ResetB ) {
		IO_OutWrite( M_B1, DTB1 );
		IO_OutWrite( M_B2, DTB2 );
	}
	motor_tick += 1;
}

void remo_tick_motor_set_value( int8 va, int8 vb, uint8 dta1, uint8 dta2, uint8 dtb1, uint8 dtb2 )
{
	valueA = va;
	valueB = vb;
	DTA1 = dta1;
	DTA2 = dta2;
	DTB1 = dtb1;
	DTB2 = dtb2;
}
//---------------------------------------------------------

static int32 servo_tick;
static bool servo_open;
static uint8 SERVO_PIN[8];
static int32 S_PWM[8];
static int32 Servo_n;

void remo_tick_servo_init()
{
	SERVO_PIN[0] = 0xFF;
	SERVO_PIN[1] = 0xFF;
	SERVO_PIN[2] = 0xFF;
	SERVO_PIN[3] = 0xFF;
	SERVO_PIN[4] = 0xFF;
	SERVO_PIN[5] = 0xFF;
	SERVO_PIN[6] = 0xFF;
	SERVO_PIN[7] = 0xFF;

	Servo_n = 0;
	servo_open = false;
}

void remo_tick_servo_SetPin( uint8 id, uint8 vDIN )
{
	servo_open = true;
	SERVO_PIN[id] = vDIN;
}

void remo_tick_servo_run(void)
{
	if( !servo_open ) {
		return;
	}
	servo_tick += 1;
	if( servo_tick != 250 ) {

		if( SERVO_PIN[Servo_n] != 0xFF && servo_tick == S_PWM[Servo_n] ) {
			IO_OutWrite( SERVO_PIN[Servo_n], 0 );
		}
		return;
	}
	if( SERVO_PIN[Servo_n] != 0xFF ) {
		IO_OutWrite( SERVO_PIN[Servo_n], 0 );
	}
	servo_tick = 0;

	Servo_n++;
	if( Servo_n == 8 ) {
		Servo_n = 0;
	}
	if( SERVO_PIN[Servo_n] != 0xFF ) {
		IO_OutWrite( SERVO_PIN[Servo_n], 1 );
	}
}

void remo_tick_servo_SetValue( uint8 id, int32 d )
{
	S_PWM[id] = d;
}
//---------------------------------------------------------

static uint8 U_id;
static bool U_Open;
static uint16 U_data;
static uint8 U_index;
static uint32 U_tick;

void remo_tick_SUART_Init(void)
{
	U_Open = false;
}

void remo_tick_SUART_SetValue( uint8 id, uint8 d )
{
	U_id = id;
	U_data = d;

	U_data <<= 1;
	U_data |= 0x0600;
	U_index = 0;
	U_tick = 0;

	U_Open = true;
	
	//等待发送完成
	//while( U_Open ) {}
	
	//用于GD32V, 2ms和50ms都不行 (SYN6288语音播放)
	SoftDelay_ms( 5 );
}

void remo_tick_SUART_run(void)
{
	if( !U_Open ) {
		return;
	}
	U_tick += 1000;
	if( U_tick >= 10417 ) {
		U_tick -= 10417;
		IO_OutWrite( U_id, U_data & 0x01 );
		U_data >>= 1;
		U_index++;
		if( U_index == 11 ) {
			U_Open = false;
		}
	}
}
//---------------------------------------------------------

void remo_tick_WS2812_Init(void)
{
	//...
}

void remo_tick_WS2812_SetValue0( uint8 id, uint8 r, uint8 g, uint8 b )
{
	uint32 d = g; d <<= 8;
	d += r; d <<= 8;
	d += b;
	
	remo_WS2812_WriteData( id, d );
}

void remo_tick_WS2812_SetValue( uint8 id, uint8 r, uint8 g, uint8 b )
{
	remo_SYS_interrupt_disable();

	remo_tick_WS2812_SetValue0( id, r, g, b );

	remo_SYS_interrupt_enable();

	SoftDelay_10us( 10 );
}

//---------------------------------------------------------

void remo_tick_WS2812Array_Init(void)
{
	//...
}

void remo_tick_WS2812Array_SetValue( uint8 id, uint16 n, uint32 buf )
{
	remo_SYS_interrupt_disable();

	for( int16 i = 0; i < n; i += 3 ) {
		remo_tick_WS2812_SetValue0( id, BASE[buf+i], BASE[buf+i+1], BASE[buf+i+2] );
	}

	remo_SYS_interrupt_enable();

	SoftDelay_10us( 10 );
}
//---------------------------------------------------------

void remo_tick_DS18B20_start_sd( uint8 id )
{
	remo_SYS_interrupt_disable();

	//DS_OUT = 0;//精确延时，维持至少480us 最大96us 取中间值 720us
	IO_OutWrite( id, 0 );

	SoftDelay_10us( 24 );
	SoftDelay_10us( 24 );
	SoftDelay_10us( 24 );
	//DS_OUT = 1;//15-60us 取中间值 37.5us
	IO_OutWrite( id, 1 );

	SoftDelay_10us( 4 );
	//DS_DIR = 0;
	IO_DirWrite( id, 0 );

	//跳过DS18B20发回的应答信号 60 - 240 按照 250余量
	SoftDelay_10us( 25 );
	//DS_DIR = 1;
	IO_DirWrite( id, 1 );

	remo_SYS_interrupt_enable();
}
void remo_tick_DS18B20_write_byte( uint8 id, uint8 data )
{
	remo_SYS_interrupt_disable();

	for( int8 i = 0; i < 8; ++i ) {
		//DS_OUT = 0;
		IO_OutWrite( id, 0 );

		//至少维持了1us,表示写时序(包括写0时序或写1时序)开始
		remo_WS2812_Delay_0_4us();
		remo_WS2812_Delay_0_4us();
		//指令dat的最低位赋予给总线,必须在拉低总线后的15us内,
        //因为15us后DS18B20会对总线采样。
		IO_OutWrite( id, data & 0x01 );
		data >>= 1;
		//必须让写时序持续至少60us 60 - 120us 取中间值 90us
		SoftDelay_10us( 9 );

		IO_OutWrite( id, 1 );

		remo_WS2812_Delay_0_4us();
		remo_WS2812_Delay_0_4us();
		remo_WS2812_Delay_0_4us();
		remo_WS2812_Delay_0_4us();
		remo_WS2812_Delay_0_4us();
		remo_WS2812_Delay_0_4us();
		remo_WS2812_Delay_0_4us();
	}
	remo_SYS_interrupt_enable();
}
uint8 remo_tick_DS18B20_read_byte( uint8 id )
{
	uint8 data = 0;
	remo_SYS_interrupt_disable();

	for( int8 i = 0; i < 8; ++i ) {
		//DS_OUT = 0;//将总线拉低，要在1us之后释放总线
		IO_OutWrite( id, 0 );

		remo_WS2812_Delay_0_4us();//单片机要在此下降沿后的15us内读数据才会有效
		remo_WS2812_Delay_0_4us();

		//至少维持了1us,表示读时序开始

		IO_OutWrite( id, 1 );

		IO_DirWrite( id, 0 );
		data >>= 1;

		//延时7us,此处参照推荐的读时序图，尽量把控制器采样时间放到读时序后的15us内的最后部分
		SoftDelay_10us( 1 );

		if( IO_InRead( id ) == 1 ) {
			data |= 0x80;
		}
		//此延时不能少，确保读时序的长度不少于60us
		SoftDelay_10us( 5 );

		IO_DirWrite( id, 1 );
		SoftDelay_10us( 1 );
	}
	remo_SYS_interrupt_enable();

	return data;
}
//---------------------------------------------------------

uint32 remo_tick_DHT11_read( uint8 id )
{
	uint32 data = 0;
	remo_SYS_interrupt_disable();

	IO_OutWrite( id, 0 );
	SoftDelay_ms( 18 );
	IO_OutWrite( id, 1 );
	SoftDelay_10us( 3 );

	IO_DirWrite( id, 0 );

	while( IO_InRead( id ) == 1 ) {}
	while( IO_InRead( id ) == 0 ) {}
	while( IO_InRead( id ) == 1 ) {}

	data = remo_tick_DHT11_read_byte( id ); data <<= 8;
	data += remo_tick_DHT11_read_byte( id ); data <<= 8;
	data += remo_tick_DHT11_read_byte( id ); data <<= 8;
	data += remo_tick_DHT11_read_byte( id );
	uint8 sum = remo_tick_DHT11_read_byte( id );

	remo_SYS_interrupt_enable();

	IO_DirWrite( id, 1 );
	return data;
}
uint8 remo_tick_DHT11_read_byte( uint8 id )
{
	uint8 d = 0;
	for( uint8 i = 0; i < 8; ++i ) {
		d <<= 1;
		while( IO_InRead( id ) == 0 ) {}
		SoftDelay_10us( 5 );
		if( IO_InRead( id ) == 1 ) {
			d |= 0x01;
			while( IO_InRead( id ) == 1 ) {}
		}
	}
	return d;
}
//---------------------------------------------------------

static bool IR_open;
static uint8 IR_Data;

static bool IR_readtime;
static uint8 IR_tick;
static uint8 IR_LastData;
static uint8 IR_Index;
static uint32 IR_RecData;
static uint8 IR_RecDataByte;
static uint8 IR_no_data_tick;

void remo_tick_IR_init()
{
	IR_open = false;
	IR_readtime = false;
	IR_tick = 0;
	IR_LastData = 1;
	IR_RecDataByte = 0xFF;
	IR_no_data_tick = 0;
}

void remo_tick_IR_SetPin( uint8 id )
{
	IR_open = true;
	IR_Data = id;
}

void remo_tick_IR_run(void)
{
	if( !IR_open ) {
		return;
	}
	if( IR_readtime ) {
		IR_tick++;
		if( IR_tick > 200 ) {
			IR_Index = 0;
			IR_tick = 0;
			IR_no_data_tick += 1;
			if( IR_no_data_tick >= 5 ) {
				IR_Index = 0;
				IR_readtime = false;
				IR_no_data_tick = 0;
				IR_RecDataByte = 0xFF;
				return;
			}
		}
	}
	uint8 cdata = IO_InRead( IR_Data );
	uint8 ldata = IR_LastData;
	IR_LastData = cdata;
	if( ldata == 1 && cdata == 0 ) {
		IR_no_data_tick = 0;
		uint8 tttt = IR_tick;
		IR_tick = 0;
		IR_readtime = true;
		IR_Index++;
		if( IR_Index == 1 || IR_Index == 2 ) {
			return;
		}
		IR_RecData >>= 1;
		if( tttt >= 16 ) {
			IR_RecData |= 0x80000000;
		}
		if( IR_Index == 34 ) {
			IR_Index = 0;
			//IR_readtime = false;
			IR_no_data_tick = 0;
			IR_RecDataByte = (uint8)(IR_RecData >> 16);
		}
	}
}

uint8 remo_tick_IR_read()
{
	return IR_RecDataByte;
}
//---------------------------------------------------------
int32 remo_tick_ultrasonic_get_value( uint8 TRIG, uint8 ECHO )
{
	uint32 tus = 0;
	int32 ddd = 0;

	remo_SYS_interrupt_disable();

	IO_OutWrite( TRIG, 1 );
	SoftDelay_10us( 1 );
	IO_OutWrite( TRIG, 0 );

	while( ddd < 50000 && IO_InRead( ECHO ) == 0 ) {
		ddd += 1;
	}
	Timer_start_count();
	while( IO_InRead( ECHO ) == 1 ) {
		tus = Timer_get_count() / Timer_get_1us_count();

		//最大测量距离限制为3.4m
		if( tus > 20000 ) {
			break;
		}
	}
	remo_SYS_interrupt_enable();
	
	//1us转换为对应的距离0.34毫米, 并除以2
	return tus * 34 / 200;
}
//---------------------------------------------------------

static uint8 cr_clk[10];
static uint8 cr_dt[10];
static int32 cr_tick[10];

static bool second[10];
static uint8 d1[10];
static uint8 d2[10];

static int8 cr_number;

void remo_tick_CrossRol_init()
{
	cr_number = 0;
}
void remo_tick_CrossRol_run()
{
	for( uint8 i = 0; i < cr_number; ++i ) {
		uint8 clk = IO_InRead( cr_clk[i] );
		if( !second[i] && clk == 0 ) {
			d1[i] = IO_InRead( cr_dt[i] );
			second[i] = true;
			return;
		}
		if( second[i] && clk == 1 ) {
			d2[i] = IO_InRead( cr_dt[i] );
		
			if( d1[i] == 1 && d2[i] == 0 ) {
				cr_tick[i] += 1;
			}
			if( d1[i] == 0 && d2[i] == 1 ) {
				cr_tick[i] -= 1;
			}
			second[i] = false;
		}
	}
}
int8 remo_tick_CrossRol_create( uint8 clk, uint8 dt )
{
	cr_clk[cr_number] = clk;
	cr_dt[cr_number] = dt;
	
	cr_tick[cr_number] = 0;
	second[cr_number] = true;
	
	d1[cr_number] = 1;
	d2[cr_number] = 1;
	
	int8 c = cr_number;
	
	cr_number++;
	
	return c;
}
int32 remo_tick_CrossRol_get_value( int8 id )
{
	return cr_tick[id];
}

//---------------------------------------------------------

static uint32 Buf_start;
static uint32 ColumnNumber;
static int16 BufferNumber;

static int32 Buf_Width;
static int32 Buf_Height;

static bool Buf_X_swap;
static bool Buf_Y_swap;
static bool Buf_XY_swap;

static int32 Buf_PenWidth;
static int32 Buf_ForColor;
static int32 Buf_BackColor;

static bool dot_set;
//static bool NeedRefresh;

void remo_tick_BufferOper_SetWH( int16 bn, uint32 addr, int32 w, int32 h, int16 cn )
{
	BufferNumber = bn;
	Buf_start = addr;
	Buf_Width = w;
	Buf_Height = h;

	ColumnNumber = cn;
	
	D_pixel = remo_tick_BufferOper_pixel;
	D_clear_pixel = remo_tick_BufferOper_clear_pixel;
}
void remo_tick_BufferOper_SetSwap( bool xys, bool xs, bool ys )
{
	Buf_XY_swap = xys;
	Buf_X_swap = xs;
	Buf_Y_swap = ys;
}
void remo_tick_BufferOper_SetPenWidth( int32 w )
{
	Buf_PenWidth = w;
}

void remo_tick_BufferOper_SetForColor( int32 w )
{
	Buf_ForColor = w;
}

void remo_tick_BufferOper_SetBackColor( int32 w )
{
	Buf_BackColor = w;
}

void remo_tick_BufferOper_pixel( int32 x, int32 y )
{
	//NeedRefresh = false;
	if( Buf_ForColor < 0 ) {
		return;
	}
	//NeedRefresh = false;
	if( Buf_ForColor == 0 ) {
		dot_set = false;
	}
	else {
		dot_set = true;
	}
	remo_tick_BufferOper_pixel1( x, y );
	//return NeedRefresh;
}

void remo_tick_BufferOper_clear_pixel( int32 x, int32 y )
{
	//NeedRefresh = false;
	if( Buf_BackColor < 0 ) {
		return;
	}
	if( Buf_BackColor == 0 ) {
		dot_set = false;
	}
	else {
		dot_set = true;
	}
	remo_tick_BufferOper_pixel1( x, y );
	//return NeedRefresh;
}
//-----------------------------
void remo_tick_BufferOper_pixel1( int32 x, int32 y )
{
	if( x < 0 || x >= Buf_Width || y < 0 || y >= Buf_Height ) {
		return;
	}
	if( Buf_X_swap ) {
		x = Buf_Width - 1 - x;
	}
	if( Buf_Y_swap ) {
		y = Buf_Height - 1 - y;
	}
	if( Buf_XY_swap ) {
		int32 t = x;
		x = y;
		y = t;
	}

	if( Buf_PenWidth == 1 ) {
		remo_tick_BufferOper_pixel0( x, y );
	}
	if( Buf_PenWidth < 1 ) {
		return;
	}
	int32 x0 = x - Buf_PenWidth/2;
	y = y - Buf_PenWidth/2;

	for( uint32 i = 0; i < Buf_PenWidth; ++i ) {
		int32 xx0 = x0;
		for( uint32 j = 0; j < Buf_PenWidth; ++j ) {
			remo_tick_BufferOper_pixel0( xx0, y );
			xx0 += 1;
		}
		y += 1;
	}
}
void remo_tick_BufferOper_pixel0( int32 x, int32 y )
{
	int8 m = (int8)(int16)y % 8;
	uint16 yy = (uint16)y;
	uint16 index = yy / 8;
	index = index * ColumnNumber + (uint16)x;
	uint8 data = BASE[Buf_start+index];

	if( dot_set ) {
		data |= 1 << m;
	}
	else {
		data &= ~(1 << m);
	}
	BASE[Buf_start+index] = data;
	//NeedRefresh = true;
}
//---------------------------------------------------
int32 remo_tick_BufferOper_get_pixel( int32 x, int32 y )
{
	if( x < 0 || x >= Buf_Width || y < 0 || y >= Buf_Height ) {
		return 1;
	}
	int8 m = (int8)(int16)y % 8;
	uint16 yy = (uint16)y;
	uint16 index = yy / 8;
	index = index * ColumnNumber + (uint16)x;
	uint8 data = BASE[Buf_start+index];
	data >>= m;
		
	if( (data & 1 ) != 0 ) {
		return 1;
	}
	else {
		return 0;
	}
}
void remo_tick_BufferOper_Clear( uint8 w )
{
	for( int16 i = 0; i < BufferNumber; ++i ) {
		BASE[Buf_start+i] = w;
	}
}
//---------------------------------------------------------
//这里需要动态重新映射
void (*D_pixel) (int32 x, int32 y);
void (*D_clear_pixel) (int32 x, int32 y);

int32 D_get_pixel( int32 x, int32 y )
{
	return remo_tick_BufferOper_get_pixel( x, y );
}
//---------------------------------------------------------

const uint8 GUI_Type = 0;
const int8 GUI_CharWidth = 1;
const uint8 GUI_ColumnL = 2;
const uint8 GUI_ColumnH = 3;
const uint8 GUI_area = 4;
const uint8 GUI_d0 = 5;
const uint8 GUI_d1 = 6;
const uint8 GUI_d2 = 7;
const uint8 GUI_d3 = 8;
const uint8 GUI_buffer = 9;

bool remo_tick_GUI_Draw( uint8 dtype, uint32 icon, int32 XX, int32 YY )
{
	bool isCode = (icon & 0xFF0000) != 0;
	icon &= 0xFFFF;
	
	bool isHit = false;
	
	uint16 YBlockNumber;
	if( isCode ) {
		YBlockNumber = Flash_ReadByte(icon + GUI_area) / 8;
	}
	else {
		YBlockNumber = BASE[icon + GUI_area] / 8;
	}
	
	uint16 BufferIndex = 0;
	int16 X = (int16)XX;
	
	uint8 CL;
	if( isCode ) {
		CL = Flash_ReadByte(icon + GUI_ColumnL);
	}
	else {
		CL = BASE[icon + GUI_ColumnL];
	}
	for( uint8 i = 0; i < CL; i += 1 ) {
		BufferIndex = i;
		int16 Y = (int16)YY;
		for( uint16 YN = 0; YN < YBlockNumber; YN += 1 ) {
			
			uint8 data;
			if( isCode ) {
				data = Flash_ReadByte(icon + GUI_buffer + BufferIndex);
			}
			else {
				data = BASE[icon + GUI_buffer + BufferIndex];
			}
			if( dtype == 0 ) {
				for( uint8 lp = 0; lp < 8; ++lp ) {
					if( (data & 1) != 0 ) {
						(*D_pixel)( X, Y );
					}
					else {
						(*D_clear_pixel)( X, Y );
					}
					data >>= 1;
					Y += 1;
				}
			}
			//这里是已经过时的
			if( dtype == 1 ) {
				for( uint8 lp = 0; lp < 8; ++lp ) {
					if( (data & 1) != 0 ) {
						(*D_clear_pixel)( X, Y );
					}
					data >>= 1;
					Y += 1;
				}
			}
			if( dtype == 2 ) {
				for( uint8 lp = 0; lp < 8; ++lp ) {
					if( (data & 1) != 0 ) {
						if( D_get_pixel( X, Y ) != 0 ) {
							isHit = true;
							return isHit;
						}
					}
					data >>= 1;
					Y += 1;
				}
			}
			uint8 temp;
			if( isCode ) {
				temp = Flash_ReadByte(icon + GUI_ColumnL);
			}
			else {
				temp = BASE[icon + GUI_ColumnL];
			}
			BufferIndex += temp;
		}
		X += 1;
	}
	return isHit;
}

void remo_tick_GUIchar_Draw( int8 PerWidth, uint32 tfont, uint16 BufferIndex, int16 X, int16 Y )
{
	tfont &= 0xFFFF;
	
	uint16 ColumnE = Flash_ReadByte(tfont+GUI_ColumnH) * 256 + Flash_ReadByte(tfont+GUI_ColumnL);
	uint8 lineNE = Flash_ReadByte(tfont+GUI_area);
	uint16 line = lineNE;
	
	if( lineNE % 8 == 0 && lineNE != 0 ) {
		lineNE = lineNE / 8;
	}
	else {
		lineNE = lineNE / 8 + 1;
	}
	for( uint8 i = 0; i < PerWidth; i += 1 ) {
		uint16 bindex = BufferIndex + i;
		int16 yy = Y;
		for( uint8 j = 0; j < lineNE; j += 1 ) {
			uint8 data = Flash_ReadByte(tfont+GUI_buffer+bindex);
			for( uint8 lp = 0; lp < 8; ++lp ) {
				if( (data & 1) != 0 ) {
					(*D_pixel)( X, yy );
				}
				else {
					(*D_clear_pixel)( X, yy );
				}
				data >>= 1;
				yy += 1;
				if( Y + (int)line <= yy ) {
					break;
				}
			}
			bindex += ColumnE;
		}
		X += 1;
	}
}


//---------------------------------------------------------
//StepMotor
void remo_tick_StepMotor_run0( int8 i );

static bool SM_Open;

#define SM_MAX 5
static int8 SM_Number;

static uint8 TickType;

static uint8 SM_Dir[SM_MAX];
static uint8 SM_Clk[SM_MAX];

static uint32 A_time[SM_MAX];
static uint32 A_angle[SM_MAX];
static uint32 A_dir[SM_MAX];
static uint32 A_runforever[SM_MAX];
static uint32 A_accel[SM_MAX];
static uint32 A_upspeed[SM_MAX];
static uint32 A_downspeed[SM_MAX];
static uint32 A_realspeed[SM_MAX];
static uint32 A_myspeed[SM_MAX];

static int16 offset[SM_MAX];
//static bool rrDIR;

void remo_tick_StepMotor_init()
{
	SM_Number = 0;
	SM_Open = false;
	//rrDIR = 0;
	TickType = 0;
}
void remo_tick_StepMotor_create( uint8 dir, uint8 clk )
{
	SM_Dir[SM_Number] = dir;
	SM_Clk[SM_Number] = clk;
}

void remo_tick_StepMotor_Set( uint32 time, uint32 angle, uint32 dir, uint32 runforever, uint32 accel, uint32 upspeed, uint32 downspeed, uint32 realspeed, uint32 myspeed )
{
	A_time[SM_Number] = time;
	A_angle[SM_Number] = angle;
	A_dir[SM_Number] = dir;
	A_runforever[SM_Number] = runforever;
	A_accel[SM_Number] = accel;
	
	A_upspeed[SM_Number] = upspeed;
	A_downspeed[SM_Number] = downspeed;
	A_realspeed[SM_Number] = realspeed;
	A_myspeed[SM_Number] = myspeed;
	
	offset[SM_Number] = 0;
	
	SM_Number++;
	SM_Open = true;
}

void remo_tick_StepMotor_run()
{
	if( !SM_Open ) {
		return;
	}
	for( int8 i = 0; i < SM_Number; ++i ) {
		remo_tick_StepMotor_run0( i );
	}
}
void remo_tick_StepMotor_run0( int8 i )
{
	uint8 D_dir = BASE[ A_dir[i] ];
	if( D_dir == 0 ) {
		return;
	}
	bool D_runforever = BASE[ A_runforever[i] ];
	int32 D_angle = Mem_Get_int32( A_angle[i] );
	int32 D_time = Mem_Get_int32( A_time[i] );
	bool D_accel = BASE[ A_accel[i] ];
	
	bool D_upspeed = BASE[ A_upspeed[i] ];
	bool D_downspeed = BASE[ A_downspeed[i] ];
	int16 D_realspeed = Mem_Get_int16( A_realspeed[i] );
	int16 D_myspeed = Mem_Get_int16( A_myspeed[i] );
	
	//判断是否使用加减速
	if( D_accel ) {
		
		//判断是否是初始加速阶段
		if( D_upspeed ) {
			if( D_realspeed < D_myspeed ) {
				
				//如果转动方向不一样, 需要先减速到0后, 才能切换方向
				//if( rrDIR != D_dir ) {
				//	D_realspeed -= 1;
				//	if( D_realspeed < 10 && D_realspeed > -10 ) {
				//		rrDIR = D_dir;
				//	}
				//}
				//else {
					D_realspeed += 1;
				//}
			}
			else {
				D_upspeed = false;
			}
		}
		//判断是否有限步数运行并且接近结束
		if( !D_runforever ) {
			if( D_downspeed ) {
				if( D_realspeed > 1 ) {
					D_realspeed -= 1;
				}
			}
		}
		offset[i] += D_realspeed;
	}
	else {
		offset[i] += D_myspeed;
	}
	Mem_Set_int16( A_realspeed[i], D_realspeed );
	BASE[A_upspeed[i]] = D_upspeed;
	
	if( offset[i] < 10000 ) {
		return;
	}
	offset[i] -= 10000;
	if( !D_runforever ) {
		if( D_time > 0 ) {
			D_time -= 1;
			Mem_Set_int32( A_time[i], D_time );
			
			if( !D_downspeed ) {
				int32 dline = D_realspeed;
				dline = dline * D_realspeed / 20000;
				if( D_time < dline ) {
					D_downspeed = true;
					D_upspeed = false;
					BASE[A_upspeed[i]] = D_upspeed;
					BASE[A_downspeed[i]] = D_downspeed;
				}
			}
		}
		else {
			if( D_dir != 0 ) {
				D_dir = 0;
				BASE[ A_dir[i] ] = D_dir;
			}
			return;
		}
	}
	if( D_dir == 1 ) {
		IO_OutWrite( SM_Dir[i], 0 );
		D_angle += 1;
	}
	else {
		IO_OutWrite( SM_Dir[i], 1 );
		D_angle -= 1;
	}
	Mem_Set_int32( A_angle[i], D_angle );
	
	if( TickType == 0 ) {
		IO_OutWrite( SM_Clk[i], 1 );
		IO_OutWrite( SM_Clk[i], 0 );
	}
	else {
		uint8 d = IO_OutRead( SM_Clk[i] );
		IO_OutWrite( SM_Clk[i], ~d );
	}
}
void remo_tick_StepMotor_SetParam( uint8 ttype )
{
	TickType = ttype;
}






