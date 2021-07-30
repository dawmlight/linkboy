

#include "remo_typedef.h"

//---------------------------------------------------------
void remo_tick_init(void);

//---------------------------------------------------------
void remo_tick_speaker_SetPin( uint8 vDIN );
void remo_tick_run10us(void);
void remo_tick_run100us(void);
void remo_tick_run1000us(void);
void remo_tick_speaker_start(int32 t);
void remo_tick_speaker_stop(void);

//---------------------------------------------------------
void remo_tick_motor_Init(void);
void remo_tick_motor_SetPin( int32 t, uint8 a1, uint8 a2, uint8 b1, uint8 b2 );
void remo_tick_motor_run(void);
void remo_tick_motor_set_value( int8 va, int8 vb, uint8 dta1, uint8 dta2, uint8 dtb1, uint8 dtb2 );

//---------------------------------------------------------
void remo_tick_servo_init(void);
void remo_tick_servo_SetPin( uint8 id, uint8 vDIN );
void remo_tick_servo_run(void);
void remo_tick_servo_SetValue( uint8 id, int32 d );

//---------------------------------------------------------
void remo_tick_SUART_Init(void);
void remo_tick_SUART_SetValue( uint8 id, uint8 d );
void remo_tick_SUART_run(void);

//---------------------------------------------------------
void remo_tick_WS2812_Init(void);
void remo_tick_WS2812_SetValue( uint8 id, uint8 r, uint8 g, uint8 b );

//---------------------------------------------------------
void remo_tick_WS2812Array_Init(void);
void remo_tick_WS2812Array_SetValue( uint8 id, uint16 n, uint32 buf );

//---------------------------------------------------------
void remo_tick_DS18B20_start_sd( uint8 id );
void remo_tick_DS18B20_write_byte( uint8 id, uint8 data );
uint8 remo_tick_DS18B20_read_byte( uint8 id );

//---------------------------------------------------------
uint32 remo_tick_DHT11_read( uint8 id );
uint8 remo_tick_DHT11_read_byte( uint8 id );

//---------------------------------------------------------
void remo_tick_IR_init(void);
void remo_tick_IR_SetPin( uint8 id );
void remo_tick_IR_run(void);
uint8 remo_tick_IR_read(void);

//---------------------------------------------------------
int32 remo_tick_ultrasonic_get_value( uint8 TRIG, uint8 ECHO );

//---------------------------------------------------------
void remo_tick_CrossRol_init(void);
void remo_tick_CrossRol_run(void);
int8 remo_tick_CrossRol_create( uint8 clk, uint8 dt );
int32 remo_tick_CrossRol_get_value( int8 id );

//---------------------------------------------------------
extern void (*D_pixel) (int32 x, int32 y);
extern void (*D_clear_pixel) (int32 x, int32 y);

//---------------------------------------------------------
void remo_tick_BufferOper_SetWH( int16 bn, uint32 addr, int32 w, int32 h, int16 cn );
void remo_tick_BufferOper_SetSwap( bool xys, bool xs, bool ys );
void remo_tick_BufferOper_SetPenWidth( int32 w );
void remo_tick_BufferOper_SetForColor( int32 w );
void remo_tick_BufferOper_SetBackColor( int32 w );
void remo_tick_BufferOper_pixel( int32 x, int32 y );
void remo_tick_BufferOper_clear_pixel( int32 x, int32 y );
void remo_tick_BufferOper_pixel0( int32 x, int32 y );
void remo_tick_BufferOper_pixel1( int32 x, int32 y );
int32 remo_tick_BufferOper_get_pixel( int32 x, int32 y );
void remo_tick_BufferOper_Clear( uint8 w );

//---------------------------------------------------------
bool remo_tick_GUI_Draw( uint8 dtype, uint32 icon, int32 XX, int32 YY );
void remo_tick_GUIchar_Draw( int8 PerWidth, uint32 icon, uint16 start, int16 XX, int16 YY );

//---------------------------------------------------------
void remo_tick_StepMotor_init(void);
void remo_tick_StepMotor_create( uint8 dir, uint8 clk );
void remo_tick_StepMotor_Set( uint32 time, uint32 angle, uint32 dir, uint32 runforever, uint32 accel, uint32 upspeed, uint32 downspeed, uint32 realspeed, uint32 myspeed );
void remo_tick_StepMotor_SetParam( uint8 ttype );




