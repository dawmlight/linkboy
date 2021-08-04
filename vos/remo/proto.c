
#include "remo_typedef.h"

#include "ERROR_TYPE.h"
#include "remo_device.h"
#include "remo_tick.h"
#include "remo_SoftDelay.h"
#include "VM.h"

void Deal( uint8 d );
void SendData( int32 d );
void Proto_Init( void );

void RunIns( uint8 d );
//void SendData( int32 d );

void SetAddr( uint8 d );
void Read( uint8 d );
void Store( uint8 d );
void Check( uint8 d );
void Version( uint8 d );
void Error( uint32 ET, uint32 EC );
void Debug( uint32 data );

bool Running;

uint32 TickShow;

//==========================================================================
//公共函数
uint8 String_length( uint8* p )
{
	uint8 len = 0;
	while( *p != 0) {
		p++;
		len++;
	}
	return len;
}
uint8 StringConst_length( const uint8* p )
{
	uint8 len = 0;
	while( *p != 0) {
		p++;
		len++;
	}
	return len;
}
//==========================================================================

//加载并执行目标文件程序
void LoadAndRun()
{
	//装载Flash
	Flash_Load();
	
	//IO初始化
	IO_Init();
	
	//公共定时器进程驱动器初始化
	remo_tick_init();
	
	//串口通信需要放到IO初始化之后
	remo_USART_Init( 115200 );
	
	VM_Reset();
	
	/*
	//ULength = 0;
	Running = true;
	
	//这里需要好好研究一下, 为何每次都需要手工启动定时器
	Timer_SetTimeMs( 10 );
	
	//开启高速驱动定时器 (ESP32防止读写flash cache冲突, 放到最后)
	remo_DrvTimer_Start();
	*/
	
	Running = false;
	uint32 Head = Flash_ReadUint32(0);
	Head &= 0x000000FF;
	if( Head == 0x00000003 ) { //0x00008003
	    Running = true;

		//这里需要好好研究一下, 为何每次都需要手工启动定时器
		Timer_SetTimeMs( 10 );
	
		//开启高速驱动定时器 (ESP32防止读写flash cache冲突, 放到最后)
		remo_DrvTimer_Start();
	}
}
//系统启动时执行一次 - 如果是K210等需要反复读取缓冲区的, 调用此函数, 而不是 setup
void setup0()
{
	//对于STM32系列好像默认就是开启的
	remo_SYS_interrupt_enable();
	
	//协议解析器初始化 (必须要在Flash初始化之前)
	Proto_Init();
	
	//系统初始化
	remo_SYS_Init();
	
	//软件延时器初始化
	SoftDelay_Init();
	
	//Flash初始化
	Flash_Init();
	
	//定时器初始化
	Timer_Init();
	
	//驱动定时器初始化 - ESP32:不能在读取flash之前就开启定时器中断! 将导致cache冲突
	remo_DrvTimer_Init();
	
	//加载并执行目标文件程序
	LoadAndRun();
	
	//这里设置是否禁用上电后自动执行用户代码
	//Running = false;
}
//系统启动时执行一次 - 本函数适用于带有串口中断进行Deal处理的芯片
void setup()
{
	setup0();
	
	//按下复位键之后的1秒钟为预留时间, 用于特殊情况下的程序下载
	SoftDelay_ms(1000);
}
//系统反复调用此函数
void loop()
{
	if( Running ) {
		
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		VM_Run();
		
		//判断是否有其他中断需要处理
		//VM_Interrupt();
		
		//判断定时器是否时间到
		if( myTimerEnable && Timer_isTimeOut() ) {

			//读取VEX_Tick
			uint8 t = Mem_Get_uint8( 0 );
			if( t == 0 ) {
				t = 10;
			}
			Timer_SetTimeMs( t );

			//并未判断中断类型
			if( !EA ) {
				return;
			}
			//内核驱动状态指示
			TickShow++;
			if( TickShow >= 100 ) {
				TickShow = 0;
			}
			if( TickShow < 5 ) {
				//IO_OutWrite( 39, 0 ); //f103re
				//IO_OutWrite( 13, 1 ); //f411
				//IO_OutWrite( 22, 0 ); //f103c8t6 方形绿板
				//IO_OutWrite( 45, 0 ); //f103c8t6 黑色串口板
			}
			else {
				//IO_OutWrite( 39, 1 ); //f103re
				//IO_OutWrite( 13, 0 ); //f411
				//IO_OutWrite( 22, 1 ); //f103c8t6 方形绿板
				//IO_OutWrite( 45, 1 ); //f103c8t6 黑色串口板
			}
			//触发中断
			VM_ClearSysValue();
			VM_AddInterruptEvent( 0x11 );
			
			VM_Interrupt();
		}
	}
	
	/*
	if( Serial_OK ) {
		Serial_OK = false;
		Deal( Serial_Data );
		GPIO_ToggleBits(GPIOA,GPIO_Pin_5);
	}
	*/
}


//----------------------------------------------------------------------------------
//协议解析器

uint32 Pro_Cnum;
uint8 Pro_VM_status;
uint16 Pro_Length;

#define VM_HEAD 0xAA
#define VM_SETADDR 0xF0	//设置指令地址
#define VM_STORE 0xF1	//存储指令
#define VM_READ 0xF2	//读取指令
#define VM_CHECK 0xF3	//读取校验和

#define VM_DEBUG 0xFD	//调试信息
#define VM_ERROR 0xFE	//协议错误
#define VM_VERSION 0xFF	//读取版本信息
	//#define VM_VERSION_STOP 0
	//#define VM_VERSION_CONTINUE 1

const uint8 VM_VERSION_A = 2;
const uint8 VM_VERSION_B = 32;

//A.B: 0.0 创世版 - 2019.5.8
//A.B: 1.0 2020.2.27 完善了大部分的硬件接口相关模块
//A.B: 1.9内测版 完成了点阵屏幕和彩屏的一些加速器

//A.B: 2.0 改名为 vos，增加了基础串口
//A.B: 2.1 增加了电路仿真引擎
//A.B: 2.30 (2021.6.7) 增加了switch语句字节码

//A.B: 2.031 (2021.6.22) 完善了步进驱动方波模式; 调整了版本号机制, 增加为3位数字
//A.B: 2.032 (2021.7.28) 增加了PS2摇杆模拟量，改善了稳定性


/*
//已过时
#define ULEN 200
uint8 UBuffer[ULEN];
uint8 ULength;

//获取一个串口数据
int16 Proto_GetData( uint8 id )
{
	int16 d = -1;
	if( ULength > 0 ) {
		remo_SYS_interrupt_disable();
		ULength--;
		d = UBuffer[ULength];
		remo_SYS_interrupt_enable();
	}
	return d;
}
	if( ULength < ULEN ) {
		UBuffer[ULength] = d;
		ULength++;
	}
*/


//处理数据
void Deal( uint8 d )
{
	Pro_Cnum++;
	//判断是否为合法数据头部
	if( Pro_Cnum == 1 ) {
		if( d != VM_HEAD ) {
			//... 报错
			Pro_Cnum = 0;
		}
		return;
	}
	//判断是否为数据长度低位
	if( Pro_Cnum == 2 ) {
		Pro_Length = d;
		return;
	}
	//判断是否为数据长度高位
	if( Pro_Cnum == 3 ) {
		Pro_Length += d * 256;
		return;
	}
	//如果是运行指令状态, 接收一个指令并解码运行
	if( Pro_Cnum == 4 ) {
		Pro_VM_status = d;
	}
	//关闭虚拟机, 防止共同占用数据通道造成异常
	//Running = false;
	
	
	//协议锁机制 - 只有先发送版本命令才会解锁
	if( Running && (Pro_VM_status != VM_VERSION || Pro_Length != 1) ) {
		Pro_Cnum = 0;
		return;
	}
	
	//如果是设置指令地址
	switch( Pro_VM_status ) {
		case VM_SETADDR:	SetAddr( d ); break;
		case VM_STORE:		Store( d ); break;
		case VM_READ:		Read( d ); break;
		case VM_CHECK:		Check( d ); break;
		
		case VM_VERSION:	Version( d ); break;
		default:			Error( E_ProError, Pro_VM_status ); Pro_Cnum = 0; break;
	}
}
//==================================================================================
//指令接收器

uint8 CheckSum;

//设置每次写入4个字节还是2个字节 (默认为2个字节)
//如需设置需要在: void Flash_Init() -> Write4Byte = true;
bool Write4Byte;

void Proto_Init()
{
	Pro_Cnum = 0;
	Pro_VM_status = 0;
	CheckSum = 0;
	Write4Byte = false;
	//ULength = 0;
}
//--------------------------------------------

uint16 InsDisc_CurrentIndex;

void CODE_Start()
{
	InsDisc_CurrentIndex = 0;
	Flash_Unlock();
	Flash_Clear();
	Flash_Lock();
}
void CODE_AddInsPage( uint8 *ins, uint32 Cnum )
{
	Flash_Unlock();
	
	if( Write4Byte ) {
		for( uint32 i = 0; i < Cnum; i += 4 ) {
			uint32 d = *ins;
			++ins;
			uint32 d1 = *ins;
			++ins;
			uint32 d2 = *ins;
			++ins;
			uint32 d3 = *ins;
			++ins;
			d += ((d1 << 8) + (d2 << 16) + (d3 << 24));
			
			Flash_Write4Byte( InsDisc_CurrentIndex, d );
			InsDisc_CurrentIndex += 4;
		}
	}
	else {
		for( uint32 i = 0; i < Cnum; i += 2 ) {
			uint16 d = *ins;
			++ins;
			uint16 d1 = *ins;
			++ins;
			d += d1 << 8;
			Flash_Write2Byte( InsDisc_CurrentIndex, d );
			InsDisc_CurrentIndex += 2;
		}
	}
	
	Flash_Lock();
}
//--------------------------------------------
//接收地址并设置
uint16 SetAddr_Addr;
void SetAddr( uint8 d )
{
	if( Pro_Cnum == 4 ) {
		return;
	}
	if( Pro_Cnum == 5 ) {
		SetAddr_Addr = d;
		return;
	}
	if( Pro_Cnum == 6 ) {
		SetAddr_Addr += d * 256;
		Pro_Cnum = 0;
		
		CheckSum = 0;
		CODE_Start();
		
		remo_USART_write( VM_HEAD );
		remo_USART_write( 1 );
		remo_USART_write( 0 );
		remo_USART_write( VM_SETADDR );
	}
}
//--------------------------------------------
//读取指令
uint16 AddrH;
void Read( uint8 d )
{
	if( Pro_Cnum == 4 ) {
		return;
	}
	if( Pro_Cnum == 5 ) {
		AddrH = d;
		return;
	}
	if( Pro_Cnum == 6 ) {
		uint16 Addr = AddrH * 256 + d;
		Pro_Cnum = 0;
		
		remo_USART_write( VM_HEAD );
		remo_USART_write( 1 );
		remo_USART_write( 1 );
		remo_USART_write( VM_READ );
		
		for( uint32 i = 0; i < 256; ++i ) {
			remo_USART_write( Flash_ReadByte( Addr + i ) );
		}
	}
}
//--------------------------------------------
//接收一个指令并存储
uint32 Number;

//指令存储缓冲区
uint8 ByteCodeBuffer[256];
uint32 Cnum;

void Store( uint8 d )
{
	if( Pro_Cnum == 4 ) {
		Number = Pro_Length - 1;
		Cnum = 0;
		return;
	}
	CheckSum ^= d;
	ByteCodeBuffer[ Cnum ] = d;
	++Cnum;
	if( Cnum == Number ) {
		Pro_Cnum = 0;
		
		CODE_AddInsPage( ByteCodeBuffer, Cnum );
		
		remo_USART_write( VM_HEAD );
		remo_USART_write( 1 );
		remo_USART_write( 0 );
		remo_USART_write( VM_STORE );
	}
}
//--------------------------------------------
//发回校验和
void Check( uint8 d )
{
	Pro_Cnum = 0;
	
	remo_USART_write( VM_HEAD );
	remo_USART_write( 2 );
	remo_USART_write( 0 );
	remo_USART_write( VM_CHECK );
	remo_USART_write( CheckSum );
	
	Flash_Save();
	
	//由于 LoadAndRun 会初始化串口 需要延时一小段时间等待数据发送完成
	SoftDelay_ms(100);
	
	//加载并执行目标文件程序
	LoadAndRun();
}
//--------------------------------------------
//版本信息协议
void Version( uint8 d )
{
	Pro_Cnum = 0;
	
	uint8 slen = StringConst_length( remo_SYS_DEV_MESSAGE() );
	
	remo_USART_write( VM_HEAD );
	remo_USART_write( 6 + slen );
	remo_USART_write( 0 );
	remo_USART_write( VM_VERSION );
	remo_USART_write( VM_VERSION_A );
	remo_USART_write( VM_VERSION_B );
	
	//2020.2.27 增加了RAM和ROM信息
	remo_USART_write( remo_SYS_BASE_LENGTH / 1024 );
	remo_USART_write( Flash_GetSize() );
	
	remo_USART_write( remo_SYS_DEV_VERSION() );
	remo_USART_SendStringConst( remo_SYS_DEV_MESSAGE() );
	
	//这里放置需要停止的所有外设代码
	remo_DrvTimer_Stop();
	Running = false;
}
//--------------------------------------------
//协议出错处理程序
void Error( uint32 ET, uint32 EC )
{
	remo_USART_write( VM_HEAD );
	remo_USART_write( 3 );
	remo_USART_write( 0 );
	remo_USART_write( VM_ERROR );
	remo_USART_write( ET );
	remo_USART_write( EC );
}
//========================================================
//调试信息
void Debug( uint32 data )
{
	remo_USART_write( VM_HEAD );
	remo_USART_write( 5 );
	remo_USART_write( 0 );
	remo_USART_write( VM_DEBUG );
	remo_USART_write( data & 0xFF );
	remo_USART_write( data >> 8 & 0xFF );
	remo_USART_write( data >> 16 & 0xFF );
	remo_USART_write( data >> 24 & 0xFF );
}






