
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
//��������
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

//���ز�ִ��Ŀ���ļ�����
void LoadAndRun()
{
	//װ��Flash
	Flash_Load();
	
	//IO��ʼ��
	IO_Init();
	
	//������ʱ��������������ʼ��
	remo_tick_init();
	
	//����ͨ����Ҫ�ŵ�IO��ʼ��֮��
	remo_USART_Init( 115200 );
	
	VM_Reset();
	
	/*
	//ULength = 0;
	Running = true;
	
	//������Ҫ�ú��о�һ��, Ϊ��ÿ�ζ���Ҫ�ֹ�������ʱ��
	Timer_SetTimeMs( 10 );
	
	//��������������ʱ�� (ESP32��ֹ��дflash cache��ͻ, �ŵ����)
	remo_DrvTimer_Start();
	*/
	
	Running = false;
	uint32 Head = Flash_ReadUint32(0);
	Head &= 0x000000FF;
	if( Head == 0x00000003 ) { //0x00008003
	    Running = true;

		//������Ҫ�ú��о�һ��, Ϊ��ÿ�ζ���Ҫ�ֹ�������ʱ��
		Timer_SetTimeMs( 10 );
	
		//��������������ʱ�� (ESP32��ֹ��дflash cache��ͻ, �ŵ����)
		remo_DrvTimer_Start();
	}
}
//ϵͳ����ʱִ��һ�� - �����K210����Ҫ������ȡ��������, ���ô˺���, ������ setup
void setup0()
{
	//����STM32ϵ�к���Ĭ�Ͼ��ǿ�����
	remo_SYS_interrupt_enable();
	
	//Э���������ʼ�� (����Ҫ��Flash��ʼ��֮ǰ)
	Proto_Init();
	
	//ϵͳ��ʼ��
	remo_SYS_Init();
	
	//�����ʱ����ʼ��
	SoftDelay_Init();
	
	//Flash��ʼ��
	Flash_Init();
	
	//��ʱ����ʼ��
	Timer_Init();
	
	//������ʱ����ʼ�� - ESP32:�����ڶ�ȡflash֮ǰ�Ϳ�����ʱ���ж�! ������cache��ͻ
	remo_DrvTimer_Init();
	
	//���ز�ִ��Ŀ���ļ�����
	LoadAndRun();
	
	//���������Ƿ�����ϵ���Զ�ִ���û�����
	//Running = false;
}
//ϵͳ����ʱִ��һ�� - �����������ڴ��д����жϽ���Deal�����оƬ
void setup()
{
	setup0();
	
	//���¸�λ��֮���1����ΪԤ��ʱ��, ������������µĳ�������
	SoftDelay_ms(1000);
}
//ϵͳ�������ô˺���
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
		
		//�ж��Ƿ��������ж���Ҫ����
		//VM_Interrupt();
		
		//�ж϶�ʱ���Ƿ�ʱ�䵽
		if( myTimerEnable && Timer_isTimeOut() ) {

			//��ȡVEX_Tick
			uint8 t = Mem_Get_uint8( 0 );
			if( t == 0 ) {
				t = 10;
			}
			Timer_SetTimeMs( t );

			//��δ�ж��ж�����
			if( !EA ) {
				return;
			}
			//�ں�����״ָ̬ʾ
			TickShow++;
			if( TickShow >= 100 ) {
				TickShow = 0;
			}
			if( TickShow < 5 ) {
				//IO_OutWrite( 39, 0 ); //f103re
				//IO_OutWrite( 13, 1 ); //f411
				//IO_OutWrite( 22, 0 ); //f103c8t6 �����̰�
				//IO_OutWrite( 45, 0 ); //f103c8t6 ��ɫ���ڰ�
			}
			else {
				//IO_OutWrite( 39, 1 ); //f103re
				//IO_OutWrite( 13, 0 ); //f411
				//IO_OutWrite( 22, 1 ); //f103c8t6 �����̰�
				//IO_OutWrite( 45, 1 ); //f103c8t6 ��ɫ���ڰ�
			}
			//�����ж�
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
//Э�������

uint32 Pro_Cnum;
uint8 Pro_VM_status;
uint16 Pro_Length;

#define VM_HEAD 0xAA
#define VM_SETADDR 0xF0	//����ָ���ַ
#define VM_STORE 0xF1	//�洢ָ��
#define VM_READ 0xF2	//��ȡָ��
#define VM_CHECK 0xF3	//��ȡУ���

#define VM_DEBUG 0xFD	//������Ϣ
#define VM_ERROR 0xFE	//Э�����
#define VM_VERSION 0xFF	//��ȡ�汾��Ϣ
	//#define VM_VERSION_STOP 0
	//#define VM_VERSION_CONTINUE 1

const uint8 VM_VERSION_A = 2;
const uint8 VM_VERSION_B = 32;

//A.B: 0.0 ������ - 2019.5.8
//A.B: 1.0 2020.2.27 �����˴󲿷ֵ�Ӳ���ӿ����ģ��
//A.B: 1.9�ڲ�� ����˵�����Ļ�Ͳ�����һЩ������

//A.B: 2.0 ����Ϊ vos�������˻�������
//A.B: 2.1 �����˵�·��������
//A.B: 2.30 (2021.6.7) ������switch����ֽ���

//A.B: 2.031 (2021.6.22) �����˲�����������ģʽ; �����˰汾�Ż���, ����Ϊ3λ����
//A.B: 2.032 (2021.7.28) ������PS2ҡ��ģ�������������ȶ���


/*
//�ѹ�ʱ
#define ULEN 200
uint8 UBuffer[ULEN];
uint8 ULength;

//��ȡһ����������
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


//��������
void Deal( uint8 d )
{
	Pro_Cnum++;
	//�ж��Ƿ�Ϊ�Ϸ�����ͷ��
	if( Pro_Cnum == 1 ) {
		if( d != VM_HEAD ) {
			//... ����
			Pro_Cnum = 0;
		}
		return;
	}
	//�ж��Ƿ�Ϊ���ݳ��ȵ�λ
	if( Pro_Cnum == 2 ) {
		Pro_Length = d;
		return;
	}
	//�ж��Ƿ�Ϊ���ݳ��ȸ�λ
	if( Pro_Cnum == 3 ) {
		Pro_Length += d * 256;
		return;
	}
	//���������ָ��״̬, ����һ��ָ���������
	if( Pro_Cnum == 4 ) {
		Pro_VM_status = d;
	}
	//�ر������, ��ֹ��ͬռ������ͨ������쳣
	//Running = false;
	
	
	//Э�������� - ֻ���ȷ��Ͱ汾����Ż����
	if( Running && (Pro_VM_status != VM_VERSION || Pro_Length != 1) ) {
		Pro_Cnum = 0;
		return;
	}
	
	//���������ָ���ַ
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
//ָ�������

uint8 CheckSum;

//����ÿ��д��4���ֽڻ���2���ֽ� (Ĭ��Ϊ2���ֽ�)
//����������Ҫ��: void Flash_Init() -> Write4Byte = true;
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
//���յ�ַ������
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
//��ȡָ��
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
//����һ��ָ��洢
uint32 Number;

//ָ��洢������
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
//����У���
void Check( uint8 d )
{
	Pro_Cnum = 0;
	
	remo_USART_write( VM_HEAD );
	remo_USART_write( 2 );
	remo_USART_write( 0 );
	remo_USART_write( VM_CHECK );
	remo_USART_write( CheckSum );
	
	Flash_Save();
	
	//���� LoadAndRun ���ʼ������ ��Ҫ��ʱһС��ʱ��ȴ����ݷ������
	SoftDelay_ms(100);
	
	//���ز�ִ��Ŀ���ļ�����
	LoadAndRun();
}
//--------------------------------------------
//�汾��ϢЭ��
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
	
	//2020.2.27 ������RAM��ROM��Ϣ
	remo_USART_write( remo_SYS_BASE_LENGTH / 1024 );
	remo_USART_write( Flash_GetSize() );
	
	remo_USART_write( remo_SYS_DEV_VERSION() );
	remo_USART_SendStringConst( remo_SYS_DEV_MESSAGE() );
	
	//���������Ҫֹͣ�������������
	remo_DrvTimer_Stop();
	Running = false;
}
//--------------------------------------------
//Э����������
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
//������Ϣ
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






