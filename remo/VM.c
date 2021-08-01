
/*
remo虚拟机
接口:

VM_Reset()
VM_AddInterruptEvent()
VM_ClearSysValue()
VM_Run()

*/

#include "VM_Mem.h"
#include "proto.h"
#include "ERROR_TYPE.h"
#include "remo_typedef.h"
#include "Code.h"
#include "remo_device.h"
#include "remo_SoftDelay.h"
#include "remo_tick.h"
#include "remo_CC.h"


void DealInterface( uint32 v );


//===============================================

//常量 - 占据4个字节.
//const uint8 ConstByteNumber = 4;
//函数地址 - 占据3个字节
//const uint8 GotoByteNumber = 3;
//接口参数 - 占据4个字节
//const uint8 InterfaceByteNumber = 4;
//变量地址 - 占据2个字节
//const uint8 VarAddrByteNumber = 2;
//读写分量 - 占据1个字节
//const uint8 SubValueByteNumber = 1;

//===============================================
//虚拟寄存器, 这是一个基于寄存器的虚拟机(而不是基于栈)

uint16 I_DP, I_TDP, I_TTDP;
uint32 I_A, I_B;

uint32 PC;
uint16 DP;
uint16 TDP; 
uint16 TTDP;
uint16 VSP;

int8 A_s8;
uint8 A_u8;
int16 A_s16;
uint16 A_u16;
int32 A_s32;
uint32 A_u32;
int8 B_s8;
uint8 B_u8;
int16 B_s16;
uint16 B_u16;
int32 B_s32;
uint32 B_u32;

int32 A;
int32 B;
uint32 AU;
uint32 BU;
int64 l_a;
int64 l_b;

uint32 PC_Step;

uint8 myTimerEnable;

//===============================================
//运行中断控制
bool EA;
#define InterruptLength 20
uint8 InterruptList[InterruptLength];

volatile uint8 AVMParamNumber;
volatile uint8 InterruptAddIndex;
volatile uint8 InterruptReadIndex;

//----------------------------------------------------------------------------------
//虚拟机初始化
void VM_Reset()
{
	PC = 0;
	DP = 0;
	TDP = 0;
	TTDP = 0;
	
    I_DP = 0;
    I_TDP = 0;
    I_TTDP = 0;
    I_A = 0;
    I_B = 0;
    
    VSP = remo_SYS_BASE_LENGTH - 1;
	A_s8 = 0;
	A_u8 = 0;
	A_s16 = 0;
	A_u16 = 0;
	A_s32 = 0;
	A_u32 = 0;
	B_s8 = 0;
	B_u8 = 0;
	B_s16 = 0;
	B_u16 = 0;
	B_s32 = 0;
	B_u32 = 0;
	A = 0;
	B = 0;
	AU = 0;
	BU = 0;
	
	EA = false;
	PC_Step = 0;
	myTimerEnable = 0;
	
	Mem_Clear();
	
	AVMParamNumber = 0;
	InterruptAddIndex = 0;
	InterruptReadIndex = 0;
}
//----------------------------------------------------------------------------------
//向虚拟机添加一个系统中断
void VM_AddInterruptEvent( uint8 Addr )
{
	InterruptList[ InterruptAddIndex ] = Addr;
	++InterruptAddIndex;
	if( InterruptAddIndex == InterruptLength ) {
		InterruptAddIndex = 0;
	}
}
//----------------------------------------------------------------------------------
void VM_ClearSysValue()
{
	AVMParamNumber = 0;
}
//----------------------------------------------------------------------------------
void ReadSys_int8()
{
	if( ( A & 0x80 ) != 0 ) {
		A |= 0xFFFFFF00;
	}
}
void ReadSys_int16()
{
	if( ( A & 0x8000 ) != 0 ) {
		A |= 0xFFFF0000;
	}
}
void ReadSys_int32()
{
	//...
}
//----------------------------------------------------------------------------------
//执行一条虚拟机指令
void VM_Run()
{
	uint32 temp;
	uint32 BB;
	uint32 NBB;
	uint32 index;
	uint32 CA;
	
	PC_Step = 4;
	
	uint32 inscode = Flash_ReadUint32( PC );
	uint8 Code = (uint8)inscode;
	uint32 BValue = inscode >> 8;
	
	uint32 temp_addr;
	bool temp_ok;
	
	uint32 v;
	switch( Code ) {
		case N_Nop:			break;
		case N_Nop0:		break;
		//接口调用
		case N_Interface:
			DP = TTDP;
			v = Flash_ReadUint32( PC + 4 );
			DealInterface( v );
			PC_Step = 8;
			break;
		case N_SetStack:		DP = BValue; break;
		case N_Goto:			PC = BValue; PC_Step = 0; break;
		case N_StackInc:		TTDP = DP + BValue; TDP = TTDP; break;
		case N_StackDec:		DP -= BValue; break;
		//函数调用
		case N_Call:
			DP = TTDP;
			temp = PC + 4;
			
			BASE[ VSP ] = (uint8)temp;
			--VSP;
			temp >>= 8;
			BASE[ VSP ] = (uint8)temp;
			--VSP;
			temp >>= 8;
			BASE[ VSP ] = (uint8)temp;
			--VSP;
			
			PC = BValue;
			PC_Step = 0;
			break;
		//返回指令
		case N_Reti:
			remo_SYS_interrupt_enable();
			EA = true;
			//不需要break, 继续往下运行
		case N_Ret:
			
			++VSP;
			temp = BASE[ VSP ];
			++VSP;
			temp <<= 8;
			temp += BASE[ VSP ];
			++VSP;
			temp <<= 8;
			temp += BASE[ VSP ];
			
			PC = temp;
			PC_Step = 0;
			break;
		case N_Pop:				++VSP; DP = 256*BASE[VSP]; ++VSP; DP += BASE[VSP]; break;
		case N_Push:			BASE[VSP] = DP%256; --VSP; BASE[VSP] = DP/256; --VSP; break;
		case N_CLI:				EA = false; remo_SYS_interrupt_disable(); break;
		case N_SEI:				EA = true; remo_SYS_interrupt_enable(); break;
		case N_MoveAtoB:				B = A; break;
		case N_SETA_uint8:					A = (uint8)Flash_ReadUint32( PC + 4 ); PC_Step = 8; break;
		case N_SETA_uint16:					A = (uint16)Flash_ReadUint32( PC + 4 ); PC_Step = 8; break;
		case N_SETA_uint32:					A = (uint32)Flash_ReadUint32( PC + 4 ); PC_Step = 8; break;
		case N_SETA_sint8:					A = (int8)Flash_ReadUint32( PC + 4 ); PC_Step = 8; break;
		case N_SETA_sint16:					A = (int16)Flash_ReadUint32( PC + 4 ); PC_Step = 8; break;
		case N_SETA_sint32:					A = (int32)Flash_ReadUint32( PC + 4 ); PC_Step = 8; break;
		//case N_SETA_fix:					A = (int32)Flash_ReadUint32( PC + 4 ); PC_Step = 8; break;
		
		case N_SETB_uint8:					B = (uint8)Flash_ReadUint32( PC + 4 ); PC_Step = 8; break;
		case N_SETB_uint16:					B = (uint16)Flash_ReadUint32( PC + 4 ); PC_Step = 8; break;
		case N_SETB_uint32:					B = (uint32)Flash_ReadUint32( PC + 4 ); PC_Step = 8; break;
		case N_SETB_sint8:					B = (int8)Flash_ReadUint32( PC + 4 ); PC_Step = 8; break;
		case N_SETB_sint16:					B = (int16)Flash_ReadUint32( PC + 4 ); PC_Step = 8; break;
		case N_SETB_sint32:					B = (int32)Flash_ReadUint32( PC + 4 ); PC_Step = 8; break;
		//case N_SETB_fix:					B = (int32)Flash_ReadUint32( PC + 4 ); PC_Step = 8; break;
		//----------------------------------------------------------
		case N_SaveCurrent:
			I_DP = DP;
			I_TDP = TDP;
			I_TTDP = TTDP;
			I_A = A;
			I_B = B;
			//设置中断函数的局部变量堆栈
			DP = remo_SYS_BASE_LENGTH - 1000;
			break;
		case N_LoadCurrent:
			DP = I_DP;
			TDP = I_TDP;
			TTDP = I_TTDP;
			A = I_A;
			B = I_B;
			break;
		//----------------------------------------------------------
		case N_Load_SP_local_uint16:		VSP = Mem_Get_uint16( DP + BValue ); break;
		case N_Save_SP_local_uint16:		Mem_Set_uint16( DP + BValue, VSP ); break;
		//----------------------------------------------------------
		case N_InsAddr:							A = BValue; break;
		
		//2019.5.7 这里临时改为加载静态变量, 方便对应python读取参数
		case N_Load_PC_local_uint:
			
			//PC = Mem_Get_uint24( DP + BValue );
			PC = Mem_Get_uint24( BValue );
			
			PC_Step = 0;
			break;
		case N_ReadAddr_local_uint32:			A = DP + Flash_ReadUint32( PC + 4 ); PC_Step = 8; break;
		case N_AddrOffset_uintx_uintx_uintx:	A += B * BValue; break;
		case N_StructOffset_uintx_uintx:		A += BValue; break;
		//----------------------------------------------------------
		/*
		case N_LoadCODE_uint8:		A = Mem_GetCode_uint8( B ); break;
		case N_LoadCODE_uint16:		A = Mem_GetCode_uint16( B ); break;
		case N_LoadCODE_uint32:		A = Mem_GetCode_uint32( B ); break;
		*/
		case N_LoadBASE_uint8:
			if( (B & 0x00FF0000) == 0 ) {
				A = BASE[B];
			}
			else {
				A = Flash_ReadByte(B & 0xFFFF);
			}
			break;
		case N_LoadBASE_uint16:
			if( (B & 0x00FF0000) == 0 ) {
				A = Mem_Get_uint16( B );
			}
			else {
				A = Mem_GetCode_uint16( (B & 0xFFFF) );
			}
			break;
		case N_LoadBASE_uint32:
			if( (B & 0x00FF0000) == 0 ) {
				A = Mem_Get_uint32( B );
			}
			else {
				A = Mem_GetCode_uint32( (B & 0xFFFF) );
			}
			break;
		case N_SaveBASE_uint8:		BASE[B] = A; break;
		case N_SaveBASE_uint16:		Mem_Set_uint16( B, A ); break;
		case N_SaveBASE_uint32:		Mem_Set_uint32( B, A ); break;
		//----------------------------------------------------------
		case N_ReadSys_sint8:				ReadSys_int8(); break;
		case N_ReadSys_sint16:				ReadSys_int16(); break;
		case N_ReadSys_sint32:				ReadSys_int32(); break;
		//----------------------------------------------------------
		case N_Save_0_local_bool:		BASE[DP + BValue] = A; break;
		case N_Save_0_local_bit:		BASE[DP + BValue] = A;; break;
		case N_Save_0_local_uint8:		BASE[DP + BValue] = A;; break;
		case N_Save_0_local_uint16:		Mem_Set_uint16( DP + BValue, A ); break;
		case N_Save_0_local_uint32:		Mem_Set_uint32( DP + BValue, A ); break;
		case N_Save_0_local_int8:		BASE[DP + BValue] = A; break;
		case N_Save_0_local_int16:		Mem_Set_int16( DP + BValue, A ); break;
		case N_Save_0_local_int32:		Mem_Set_int32( DP + BValue, A ); break;
		
		case N_Load_0_local_bool:		A = BASE[ DP + BValue ]; break;
		case N_Load_0_local_bit:		A = BASE[ DP + BValue ]; break;
		case N_Load_0_local_uint8:		A = BASE[ DP + BValue ]; break;
		case N_Load_0_local_uint16:		A = Mem_Get_uint16( DP + BValue ); break;
		case N_Load_0_local_uint32:		A = Mem_Get_uint32( DP + BValue ); break;
		case N_Load_0_local_int8:		A = Mem_Get_int8( DP + BValue ); break;
		case N_Load_0_local_int16:		A = Mem_Get_int16( DP + BValue ); break;
		case N_Load_0_local_int32:		A = Mem_Get_int32( DP + BValue ); break;
		
		case N_Load_1_local_bool:		B = BASE[ DP + BValue ]; break;
		case N_Load_1_local_bit:		B = BASE[ DP + BValue ]; break;
		case N_Load_1_local_uint8:		B = BASE[ DP + BValue ]; break;
		case N_Load_1_local_uint16:		B = Mem_Get_uint16( DP + BValue ); break;
		case N_Load_1_local_uint32:		B = Mem_Get_uint32( DP + BValue ); break;
		case N_Load_1_local_int8:		B = Mem_Get_int8( DP + BValue ); break;
		case N_Load_1_local_int16:		B = Mem_Get_int16( DP + BValue ); break;
		case N_Load_1_local_int32:		B = Mem_Get_int32( DP + BValue ); break;
		//----------------------------------------------------------
		case N_Save_0_static_bool:		BASE[BValue] = A; break;
		case N_Save_0_static_bit:		BASE[BValue] = A; break;
		case N_Save_0_static_uint8:		BASE[BValue] = A; break;
		case N_Save_0_static_uint16:	Mem_Set_uint16( BValue, A ); break;
		case N_Save_0_static_uint32:	Mem_Set_uint32( BValue, A ); break;
		case N_Save_0_static_int8:		BASE[BValue] = A; break;
		case N_Save_0_static_int16:		Mem_Set_int16( BValue, A ); break;
		case N_Save_0_static_int32:		Mem_Set_int32( BValue, A ); break;
		
		case N_Load_0_static_bool:		A = BASE[ BValue ]; break;
		case N_Load_0_static_bit:		A = BASE[ BValue ]; break;
		case N_Load_0_static_uint8:		A = BASE[ BValue ]; break;
		case N_Load_0_static_uint16:	A = Mem_Get_uint16( BValue ); break;
		case N_Load_0_static_uint32:	A = Mem_Get_uint32( BValue ); break;
		case N_Load_0_static_int8:		A = Mem_Get_int8( BValue ); break;
		case N_Load_0_static_int16:		A = Mem_Get_int16( BValue ); break;
		case N_Load_0_static_int32:		A = Mem_Get_int32( BValue ); break;
		
		case N_Load_1_static_bool:		B = BASE[ BValue ]; break;
		case N_Load_1_static_bit:		B = BASE[ BValue ]; break;
		case N_Load_1_static_uint8:		B = BASE[ BValue ]; break;
		case N_Load_1_static_uint16:	B = Mem_Get_uint16( BValue ); break;
		case N_Load_1_static_uint32:	B = Mem_Get_uint32( BValue ); break;
		case N_Load_1_static_int8:		B = Mem_Get_int8( BValue ); break;
		case N_Load_1_static_int16:		B = Mem_Get_int16( BValue ); break;
		case N_Load_1_static_int32:		B = Mem_Get_int32( BValue ); break;
		//----------------------------------------------------------
		case N_AutoSwitch_int16_int8:	break; //A &= 0xff; ReadSys_int8(); break;
		case N_AutoSwitch_int32_int8:	break; //A &= 0xff; ReadSys_int8(); break;
		case N_AutoSwitch_int32_int16:	break; //A &= 0xffff; ReadSys_int16(); break;
		case N_AutoSwitch_fix_int32:	A <<= 10; ReadSys_int32(); break;
		case N_AutoSwitch_int32_fix:	A >>= 10; ReadSys_int32(); break;
		//----------------------------------------------------------
		case N_UserSwitch_sint8_uint8:		ReadSys_int8(); break;
		case N_UserSwitch_sint16_uint16:	ReadSys_int16(); break;
		case N_UserSwitch_sint32_uint32:	break;
		case N_UserSwitch_uint8_sint8:		A &= 0xFF; break;
		case N_UserSwitch_uint16_sint16:	A &= 0xFFFF; break;
		case N_UserSwitch_uint32_sint32:	break;
		
		case N_UserSwitch_uint8_uint16:		A &= 0xFF; break;
		case N_UserSwitch_uint8_uint32:		A &= 0xFF; break;
		case N_UserSwitch_uint16_uint32:	A &= 0xFFFF; break;
		
		case N_UserSwitch_sint8_sint16:		A &= 0xFF; ReadSys_int8(); break;
		case N_UserSwitch_sint8_sint32:		A &= 0xFF; ReadSys_int8(); break;
		case N_UserSwitch_sint16_sint32:	A &= 0xFFFF; ReadSys_int16(); break;
		//----------------------------------------------------------
		case N_Not_bool_bool:				if( A == 0 ) A = 1; else A = 0; break;
		case N_And_bool_bool_bool:			if( A != 0 && B != 0 ) A = 1; else A = 0; break;
		case N_Or_bool_bool_bool:			if( A != 0 || B != 0 ) A = 1; else A = 0; break;
		//----------------------------------------------------------
		case N_Neg_int8_int8:				A = -A; break;
		case N_Neg_int16_int16:				A = -A; break;
		case N_Neg_int32_int32:				A = -A; break;
		case N_Abs_x_x:						if( A < 0 ) A = -A; break;
		//----------------------------------------------------------
		case N_Add_uint8_uint8_uint8:		A += B; A &= 0xff; break;
		case N_Add_uint16_uint16_uint16:	A += B; A &= 0xffff; break;
		case N_Add_uint32_uint32_uint32:	A += B; break;
		case N_Add_int8_int8_int8:			A += B; break;
		case N_Add_int16_int16_int16:		A += B; break;
		case N_Add_int32_int32_int32:		A += B; break;
		//----------------------------------------------------------
		case N_Sub_uint8_uint8_uint8:		A -= B; A &= 0xff; break;
		case N_Sub_uint16_uint16_uint16:	A -= B; A &= 0xffff; break;
		case N_Sub_uint32_uint32_uint32:	A -= B; break;
		case N_Sub_int8_int8_int8:			A -= B; break;
		case N_Sub_int16_int16_int16:		A -= B; break;
		case N_Sub_int32_int32_int32:		A -= B; break;
		//----------------------------------------------------------
		case N_Mult_uint8_uint8_uint8:		AU = (uint8)A; BU = (uint8)B; AU *= BU; A = (int8)AU; A &= 0xff; break;
		case N_Mult_uint16_uint16_uint16:	AU = (uint16)A; BU = (uint16)B; AU *= BU; A = (int16)AU; A &= 0xffff; break;
		case N_Mult_uint32_uint32_uint32:	AU = (uint32)A; BU = (uint32)B; AU *= BU; A = (int32)AU; break;
		case N_Mult_int8_int8_int8:			A *= B; break;
		case N_Mult_int16_int16_int16:		A *= B; break;
		case N_Mult_int32_int32_int32:		A *= B; break;
		
		case N_Mult_fix_fix_fix:			//A *= B; AU = (uint32)A; AU >>= 10; A = (int32)AU; break;
											l_a = A; l_b = B; l_a *= l_b; l_a >>= 10; A = (int32)l_a; break;
											//l_a = A; l_b = B; l_a *= l_b; l_a /= 1024; A = (int32)l_a; break;
											//l_a = A; l_b = B; l_a /= 32; l_b /= 32; l_a *= l_b; A = (int32)l_a; break;
		
		//----------------------------------------------------------
		case N_Div_uint8_uint8_uint8:		AU = (uint8)A; BU = (uint8)B; AU /= BU; A = (int8)AU; A &= 0xff; break;
		case N_Div_uint16_uint16_uint16:	AU = (uint16)A; BU = (uint16)B; AU /= BU; A = (int16)AU; A &= 0xffff; break;
		case N_Div_uint32_uint32_uint32:	AU = (uint32)A; BU = (uint32)B; AU /= BU; A = (int32)AU; break;
		case N_Div_int8_int8_int8:			A /= B; break;
		case N_Div_int16_int16_int16:		A /= B; break;
		case N_Div_int32_int32_int32:		A /= B; break;
		
		case N_Div_fix_fix_fix:				//A /= B; A <<= 10; A += ((A%B) << 10) / B; break;
											l_a = A; l_b = B; l_a <<= 10; l_a /= l_b; A = (int32)l_a; break;
											//A = 4096000; B = 40960; l_a = A; l_b = B; l_a <<= 10; l_a >>= 10; A = (int32)l_a; A = 1024 * 50 * sizeof(long); break;

		//----------------------------------------------------------
		case N_Mod_uint8_uint8_uint8:		AU = (uint8)A; BU = (uint8)B; AU %= BU; A &= 0xff; A = (int8)AU; break;
		case N_Mod_uint16_uint16_uint16:	AU = (uint16)A; BU = (uint16)B; AU %= BU; A &= 0xffff; A = (int16)AU; break;
		case N_Mod_uint32_uint32_uint32:	AU = (uint32)A; BU = (uint32)B; AU %= BU; A = (int32)AU; break;
		case N_Mod_int8_int8_int8:			A %= B; break;
		case N_Mod_int16_int16_int16:		A %= B; break;
		case N_Mod_int32_int32_int32:		A %= B; break;
		//----------------------------------------------------------
		/*
		case N_Large_bool_uint_uint:		AU = A; BU = B; if( AU > BU ) AU = 1; else AU = 0;		A = AU; break;
		case N_Large_bool_sint_sint:		if( A > B ) A = 1; else A = 0; break;
		case N_LargeEqual_bool_uint_uint:	AU = A; BU = B; if( AU >= BU ) AU = 1; else AU = 0;		A = AU; break;
		case N_LargeEqual_bool_sint_sint:	if( A >= B ) A = 1; else A = 0; break;
		case N_Small_bool_uint_uint:		AU = A; BU = B; if( AU < BU ) AU = 1; else AU = 0;		A = AU; break;
		case N_Small_bool_sint_sint:		if( A < B ) A = 1; else A = 0; break;
		case N_SmallEqual_bool_uint_uint:	AU = A; BU = B; if( AU <= BU ) AU = 1; else AU = 0;		A = AU; break;
		case N_SmallEqual_bool_sint_sint:	if( A <= B ) A = 1; else A = 0; break;
		case N_Equal_bool_uint_uint:
		case N_Equal_bool_sint_sint:		if( A == B ) A = 1; else A = 0; break;
		case N_NotEqual_bool_uint_uint:
		case N_NotEqual_bool_sint_sint:		if( A != B ) A = 1; else A = 0; break;
		*/
		
		case N_Large_bool_uint_uint:		AU = A; BU = B; AU = ( AU > BU );		A = AU; break;
		case N_Large_bool_sint_sint:		A = ( A > B ); break;
		case N_LargeEqual_bool_uint_uint:	AU = A; BU = B; AU = ( AU >= BU );		A = AU; break;
		case N_LargeEqual_bool_sint_sint:	A = ( A >= B ); break;
		case N_Small_bool_uint_uint:		AU = A; BU = B; AU = ( AU < BU );		A = AU; break;
		case N_Small_bool_sint_sint:		A = ( A < B ); break;
		case N_SmallEqual_bool_uint_uint:	AU = A; BU = B; AU = ( AU <= BU );		A = AU; break;
		case N_SmallEqual_bool_sint_sint:	A = ( A <= B ); break;
		case N_Equal_bool_uint_uint:
		case N_Equal_bool_sint_sint:		A = ( A == B ); break;
		case N_NotEqual_bool_uint_uint:
		case N_NotEqual_bool_sint_sint:		A = ( A != B ); break;
		
		//----------------------------------------------------------
		case N_switchcmp_uint32_uint8:
			temp_addr = (uint32)Flash_ReadUint32( PC + 4 );
			PC_Step = 8;
			temp_ok = false;
			A &= 0xFFFF;
			for( int i = 0; i < BValue; ++i ) {
				uint8 a = (uint8)Flash_ReadByte( A + i );
				if( a == B ) {
					B = i;
					temp_ok = true;
					break;
				}
			}
			if( !temp_ok ) {
				PC = temp_addr;
				PC_Step = 0;
			}
			break;
		case N_switchgoto_uint32:
			A &= 0xFFFF;
			A += B << 2;
			PC = Flash_ReadUint32( A );
			PC_Step = 0;
			break;
		//----------------------------------------------------------
		case N_TrueGoto_bool:
			if( A == 1 ) {
				PC = BValue;
				PC_Step = 0;
			}
			break;
		case N_FalseGoto_bool:
			if( A == 0 ) {
				PC = BValue;
				PC_Step = 0;
			}
			break;
		//----------------------------------------------------------
		case N_Com_bit_bit:					AU = A; AU ^= 0x01; A = AU; break;
		case N_Com_uint8_uint8:				AU = A; AU ^= 0xFF; A = AU; break;
		case N_Com_uint16_uint16:			AU = A; AU ^= 0xFFFF; A = AU; break;
		case N_Com_uint32_uint32:			AU = A; AU ^= 0xFFFFFFFF; A = AU; break;
		case N_And_x_x_x:					AU = A; BU = B; AU &= BU; A = AU; break;
		case N_Or_x_x_x:					AU = A; BU = B; AU |= BU; A = AU; break;
		case N_Xor_x_x_x:					AU = A; BU = B; AU ^= BU; A = AU; break;
		case N_Right_x_x_x:					AU = A; BU = B; AU >>= BU; A = AU; break;
		case N_Left_uint8_uint8_uint8:		AU = A; BU = B; AU <<= BU; AU &= 0xFF; A = AU; break;
		case N_Left_uint16_uint16_uint8:	AU = A; BU = B; AU <<= BU; AU &= 0xFFFF; A = AU; break;
		case N_Left_uint32_uint32_uint8:	AU = A; BU = B; AU <<= BU; A = AU; break;
		//----------------------------------------------------------
		
		case N_ReadSub_bit_uint8:			A >>= BValue; A &= 0x01; break;
		case N_ReadSub_bit_uint16:			A >>= BValue; A &= 0x01; break;
		case N_ReadSub_bit_uint32:			A >>= BValue; A &= 0x01; break;
		case N_ReadSub_uint8_uint32:		A >>= BValue; A &= 0xFF; break;
		case N_ReadSub_uint8_uint16:		A >>= BValue; A &= 0xFF; break;
		case N_ReadSub_uint16_uint32:		A >>= BValue; A &= 0xFFFF; break;
		
		
		case N_WriteSub_uint8_uint8_bit:
			BB = ((uint32)1 << BValue) & 0xFF;
			NBB = BB ^ 0xFF;
			AU = A;
			if( B == 0 ) AU &= NBB; else AU |= BB;
			A = AU;
			break;
		case N_WriteSub_uint16_uint16_bit:
			BB = ((uint32)1 << BValue) & 0xFFFF;
			NBB = BB ^ 0xFFFF;
			AU = A;
			if( B == 0 ) AU &= NBB; else AU |= BB;
			A = AU;
			break;
		case N_WriteSub_uint32_uint32_bit:
			BB = ((uint32)1 << BValue);
			NBB = BB ^ 0xFFFFFFFF;
			AU = A;
			if( B == 0 ) AU &= NBB; else AU |= BB;
			A = AU;
			break;
		case N_WriteSub_uint16_uint16_uint8:
			index = BValue;
			BU = B;
			BB = (BU << index);
			CA = ((uint32)0xFF << index) ^ 0xFFFF;
			AU = A;
			AU &= CA;
			AU |= BB;
			A = AU;
			break;
		case N_WriteSub_uint32_uint32_uint16:
			index = BValue;
			BU = B;
			BB = (BU << index);
			CA = ((uint32)0xFFFF << index) ^ 0xFFFFFFFF;
			AU = A;
			AU &= CA;
			AU |= BB;
			A = AU;
			break;
		case N_WriteSub_uint32_uint32_uint8:
			index = BValue;
			BU = B;
			BB = (BU << index);
			CA = ((uint32)0xFF << index) ^ 0xFFFFFFFF;
			AU = A;
			AU &= CA;
			AU |= BB;
			A = AU;
			break;
		//----------------------------------------------------------
		case N_TransVar_bit:			BASE[TDP] = A; TDP += 1; break;
		case N_TransVar_bool:			BASE[TDP] = A; TDP += 1; break;
		case N_TransVar_uint8:			BASE[TDP] = A; TDP += 1; break;
		case N_TransVar_uint16:			Mem_Set_uint16( TDP, A ); TDP += 2; break;
		case N_TransVar_uint32:			Mem_Set_uint32( TDP, A ); TDP += 4; break;
		case N_TransVar_int8:			BASE[TDP] = A; TDP += 1; break;
		case N_TransVar_int16:			Mem_Set_int16( TDP, A ); TDP += 2; break;
		case N_TransVar_int32:			Mem_Set_int32( TDP, A ); TDP += 4; break;
		
		//跳转递减
		case N_GotoDec_x:
			if( A == 0 ) {
				PC = BValue;
				PC_Step = 0;
			}
			else {
				--A;
			}
			break;
		//出错处理
		default:
			Running = false;
			Error( E_VM_InsError, Code );
			break;
	}
	PC += PC_Step;
}
uint8 VM_GetInterrupt()
{
	if( InterruptReadIndex != InterruptAddIndex ) {
		return InterruptList[ InterruptReadIndex ];
	}
	else {
		return 0;
	}
}
void VM_Interrupt()
{
	//处理虚拟机中断
	if( InterruptReadIndex != InterruptAddIndex ) {
		
		uint32 Addr = PC;
		
		BASE[ VSP ] = (uint8)Addr;
		--VSP;
		Addr >>= 8;
		BASE[ VSP ] = (uint8)Addr;
		--VSP;
		Addr >>= 8;
		BASE[ VSP ] = (uint8)Addr;
		--VSP;
		
		EA = false;
		uint8 TPC = InterruptList[ InterruptReadIndex ];
		PC = TPC * 4;
		++InterruptReadIndex;
		if( InterruptReadIndex == InterruptLength ) {
			InterruptReadIndex = 0;
		}
	}
}

//------------------------------------------------------------------------------------

void Mem_Clear()
{
	for( uint32 i = 0; i < remo_SYS_BASE_LENGTH; ++i ) {
		BASE[i] = 0;
	}
}
uint16 Mem_GetCode_uint8( uint16 Addr )
{
	return Flash_ReadByte( Addr );
}
uint16 Mem_GetCode_uint16( uint16 Addr )
{
	uint16 d = Flash_ReadByte( Addr+1 ); d <<= 8;
	d += Flash_ReadByte( Addr );
	return d;
}
uint32 Mem_GetCode_uint32( uint16 Addr )
{
	uint32 d = Flash_ReadByte( Addr+3 ); d <<= 8;
	d += Flash_ReadByte( Addr+2 ); d <<= 8;
	d += Flash_ReadByte( Addr+1 ); d <<= 8;
	d += Flash_ReadByte( Addr );
	return d;
}
void Mem_Set_uint8( uint16 Addr, uint8 d )
{
	BASE[Addr] = d;
}
void Mem_Set_uint16( uint16 Addr, uint16 d )
{
	BASE[Addr] = d; d >>= 8;
	BASE[Addr+1] = d;
}
void Mem_Set_uint32( uint16 Addr, uint32 d )
{
	BASE[Addr] = d; d >>= 8;
	BASE[Addr+1] = d; d >>= 8;
	BASE[Addr+2] = d; d >>= 8;
	BASE[Addr+3] = d;
}
uint8 Mem_Get_uint8( uint16 Addr )
{
	return BASE[Addr];
}
uint16 Mem_Get_uint16( uint16 Addr )
{
	uint16 d = BASE[Addr+1]; d <<= 8;
	d += BASE[Addr];
	return d;
}
uint32 Mem_Get_uint24( uint16 Addr )
{
	uint32 d = BASE[Addr+2]; d <<= 8;
	d += BASE[Addr+1]; d <<= 8;
	d += BASE[Addr];
	return d;
}
uint32 Mem_Get_uint32( uint16 Addr )
{
	uint32 d = BASE[Addr+3]; d <<= 8;
	d += BASE[Addr+2]; d <<= 8;
	d += BASE[Addr+1]; d <<= 8;
	d += BASE[Addr];
	return d;
}
void Mem_Set_int8( uint16 Addr, int8 dd )
{
	BASE[Addr] = dd;
}
void Mem_Set_int16( uint16 Addr, int16 dd )
{
	uint32 d = (uint32)dd;
	BASE[Addr] = d; d >>= 8;
	BASE[Addr+1] = d;
}
void Mem_Set_int32( uint16 Addr, int32 dd )
{
	uint32 d = (uint32)dd;
	BASE[Addr] = d; d >>= 8;
	BASE[Addr+1] = d; d >>= 8;
	BASE[Addr+2] = d; d >>= 8;
	BASE[Addr+3] = d;
}
int32 Mem_Get_int8( uint16 Addr )
{
	uint32 d = BASE[Addr];

	if( ( d & 0x80 ) != 0 ) {
		d |= 0xFFFFFF00;
	}
	return d;
}
int32 Mem_Get_int16( uint16 Addr )
{
	uint32 d = BASE[Addr+1]; d <<= 8;
	d += BASE[Addr];

	if( ( d & 0x8000 ) != 0 ) {
		d |= 0xFFFF0000;
	}
	return d;
}
int32 Mem_Get_int32( uint16 Addr )
{
	uint32 d = BASE[Addr+3]; d <<= 8;
	d += BASE[Addr+2]; d <<= 8;
	d += BASE[Addr+1]; d <<= 8;
	d += BASE[Addr];
	return (int32)d;
}
//只用于ESP32
uint8* Mem_GetAddr( uint32 addr )
{
	uint8 *p;
	
	//得到text
	bool isCode = (addr & 0xFF0000) != 0;
	addr &= 0xFFFF;
	if( isCode ) {
		p = Flash_GetAddr( addr );
	}
	else {
		p = &BASE[addr];
	}
	return p;
}
//------------------------------------------------------------------------------------

void REMO_DataChannelWrite( uint32 d, uint32 Data )
{
	uint8 Addr = (uint8)(d & 0xFF);
	uint8 Type = (uint8)(d >> 8);
	
	switch( Type ) {
		case 0:		IO_DirWrite( Addr, (uint8)Data ); break;
		case 1:		IO_OutWrite( Addr, (uint8)Data ); break;
		//case 2:	IO_InWrite( Addr, (uint8)Data ); break;
		case 3:		IO_PullWrite( Addr, (uint8)Data ); break;
		case 4:		IO_AnalogOpen( Addr, (uint8)Data ); break;
		default:	Error( E_RunError, 1 ); break;
	}
}

void REMO_DataChannelRead( uint32 d )
{
	uint8 Addr = (uint8)(d & 0xFF);
	uint8 Type = (uint8)(d >> 8);
	
	switch( Type ) {
		//case 0:	SendData( IO_DirRead( Addr ) ); break;
		case 1:		A = IO_OutRead( Addr ); break;
		case 2:		A = IO_InRead( Addr ); break;
		//case 3:	A = IO_PullRead( Addr ); break;
		case 4:		A = IO_AnalogRead( Addr ); break;
		default:	Error( E_RunError, 1 ); break;
	}
}

//------------------------------------------------------------------------------------
#include "remo_SHT1106_I2C.h"
#include "remo_SHT1106_DCSPI.h"
#include "remo_ST7920.h"
#include "remo_ST7789.h"

uint32 u0;
uint32 u1;
uint32 u2;
uint32 u3;
uint32 u4;
uint32 u5;
uint32 u6;
uint32 u7;
uint32 u8;
uint32 u9;

int32 i0;
int32 i1;
int32 i2;
int32 i3;
int32 i4;
int32 i5;
int32 i6;
int32 i7;
int32 i8;
int32 i9;

uint8 u8_1;


void SystemDeal( uint32 v );
void SPI_12864Deal( uint32 v );
void I2C_12864Deal( uint32 v );
void ST7920_12864Deal( uint32 v );
void ST7789_Deal( uint32 v );
void Speak_Deal( uint32 v );
void Motor_Deal( uint32 v );
void Servo_Deal( uint32 v );
void SUART_Deal( uint32 v );
void WS2812_Deal( uint32 v );
void WS2812Array_Deal( uint32 v );
void DS18B20_Deal( uint32 v );
void DHT11_Deal( uint32 v );
void IR_Deal( uint32 v );
void ultrasonic_Deal( uint32 v );
void CrossRol_Deal( uint32 v );
void BufferOper_Deal( uint32 v );
void GUI_Deal( uint32 v );
void GUIchar_Deal( uint32 v );
void Uart_Deal( uint32 v );
void StepMotor_Deal( uint32 v );

void K210_Deal( uint32 v );

void NET_wifiAP_Deal( uint32 v );
void NET_wifiSTA_Deal( uint32 v );
void NET_TCP_Server_Deal( uint32 v );
void NET_TCP_Client_Deal( uint32 v );

void OpenHarmony_Deal( uint32 v );

//处理接口请求 (主中转)
void DealInterface( uint32 v )
{
	uint32 v24 = v >> 8;
	uint32 v8 = v & 0xFF;
	
	switch( v24 ) {
		//系统接口
		case 0:		SystemDeal( v8 ); break;
		case 1:		SPI_12864Deal( v8 ); break;
		case 2:		I2C_12864Deal( v8 ); break;
		case 3:		ST7920_12864Deal( v8 ); break;
		case 4:		ST7789_Deal( v8 ); break;
		case 5:		Speak_Deal( v8 ); break;
		case 6:		Motor_Deal( v8 ); break;
		case 7:		Servo_Deal( v8 ); break;
		case 8:		SUART_Deal( v8 ); break;
		case 9:		WS2812_Deal( v8 ); break;
		case 10:	WS2812Array_Deal( v8 ); break;
		case 11:	DS18B20_Deal( v8 ); break;
		case 12:	DHT11_Deal( v8 ); break;
		case 13:	IR_Deal( v8 ); break;
		case 14:	ultrasonic_Deal( v8 ); break;
		case 15:	CrossRol_Deal( v8 ); break;
		case 16:	BufferOper_Deal( v8 ); break;
		case 17:	GUI_Deal( v8 ); break;
		case 18:	GUIchar_Deal( v8 ); break;
		case 19:	Uart_Deal( v8 ); break;
		case 20:	StepMotor_Deal( v8 ); break;
		
		//K210专用 200 -299
		
		//ESP32专用 300 - 399
		case 300:	NET_wifiAP_Deal( v8 ); break;
		case 301:	NET_wifiSTA_Deal( v8 ); break;
		case 302:	NET_TCP_Server_Deal( v8 ); break;
		case 303:	NET_TCP_Client_Deal( v8 ); break;

		case 400:	OpenHarmony_Deal( v8 ); break;

		default:	Error( E_VM_InterfaceError, (uint8)v );
					Running = false;
					break;
	}
}
//=====================================================================
//处理系统请求
//注意: 所有系统请求函数一律添加到 OS_VM.txt文件中, 不允许分散各地
void SystemDeal( uint32 v )
{
	switch( v ) {
		case 1:
			//... 开启对应的中断
			i0 = Mem_Get_int32( DP + 0 );
			u1 = Mem_Get_uint8( DP + 4 );
			switch( i0 ) {
				case 0x11:	myTimerEnable = u1; break;
				default: break;
			}
			break;
		case 3:
			i0 = Mem_Get_uint32( DP + 0 );
			Debug( i0 );
			break;
		case 4:
			u0 = Mem_Get_uint32( DP + 0 );
			u1 = Mem_Get_uint32( DP + 4 );
			REMO_DataChannelWrite( u0, u1 );
			break;
		case 5:
			u0 = Mem_Get_uint32( DP + 0 );
			REMO_DataChannelRead( u0 );
			break;
		
		//注意: 延时微秒函数是无效的
		case 10:
			u0 = Mem_Get_uint8( DP + 0 );
			for( uint8 ii = 0; ii < u0; ++ii ) {
				//Timer_Delay_0_4us();
			}
			break;
		
		case 11:
			u0 = Mem_Get_uint8( DP + 0 );
			SoftDelay_10us( u0 );
			break;
		
		case 13:
			u0 = Mem_Get_uint32( DP + 0 );
			u1 = Mem_Get_uint32( DP + 4 );
			u2 = Mem_Get_uint16( DP + 8 );
			u3 = Mem_Get_uint32( DP + 10 );
			remo_CC_Reset( u0, u1, u2, u3 );
			break;
		case 14:
			remo_CC_Deal();
			break;
		
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			Running = false;
			break;
	}
}
//=====================================================================
//SHT1106_SPI接口
int8 SPI_12864_Offset;

void SPI_12864Deal( uint32 v )
{
	uint8 s9_line = 0;
	uint16 s9_addr = 0;
	
	switch( v ) {
		//设置引脚
		case 0:
			u0 = Mem_Get_uint8( DP + 0 );
			u1 = Mem_Get_uint8( DP + 1 );
			u2 = Mem_Get_uint8( DP + 2 );
			u3 = Mem_Get_uint8( DP + 3 );
			u4 = Mem_Get_uint8( DP + 4 );
			remo_SHT1106_DCSPI_SetPin( u0, u1, u2, u3, u4 );
			break;
		//初始化
		case 1:
			i0 = Mem_Get_int8( DP + 0 );
			SPI_12864_Offset = i0;
			remo_SHT1106_DCSPI_Init();
			break;
		//刷新
		case 2:
			u0 = Mem_Get_uint32( DP + 0 );
			s9_line = 0;
			s9_addr = u0;
			for( uint8 i = 0; i < 8; ++i ) {
				remo_SHT1106_DCSPI_write_command( 0xB0 + s9_line );
				remo_SHT1106_DCSPI_write_command( 0x00 + SPI_12864_Offset );
				remo_SHT1106_DCSPI_write_command( 0x10 );
				s9_line += 1;
				for( uint8 j = 0; j < 128; ++j ) {
					remo_SHT1106_DCSPI_write_data( BASE[s9_addr] );
					s9_addr++;
				}
			}
			break;
		//数据写入接口
		case 3:
			i0 = Mem_Get_int16( DP + 0 );
			i1 = Mem_Get_int16( DP + 2 ) + SPI_12864_Offset;
			u0 = Mem_Get_uint8( DP + 4 );
			remo_SHT1106_DCSPI_write_command( 0xB0 + i0 );
			remo_SHT1106_DCSPI_write_command( (uint8)i1 & 0x0F );
			remo_SHT1106_DCSPI_write_command( 0x10 | ((uint8)i1 >> 4) );
			remo_SHT1106_DCSPI_write_data( u0 );
			break;
		
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
//=====================================================================
//SHT1106_I2C接口
int8 I2C_12864_Offset;

void I2C_12864Deal( uint32 v )
{
	uint8 line = 0;
	uint16 addr = 0;
	
	switch( v ) {
		//设置引脚
		case 0:
			u0 = Mem_Get_uint8( DP + 0 );
			u1 = Mem_Get_uint8( DP + 1 );
			remo_SHT1106_I2C_SetPin( u0, u1 );
			break;
		//初始化
		case 1:
			i0 = Mem_Get_int8( DP + 0 );
			I2C_12864_Offset = i0;
			remo_SHT1106_I2C_Init();
			break;
		//刷新
		case 2:
			u0 = Mem_Get_uint32( DP + 0 );
			line = 0;
			addr = u0;
			for( uint8 i = 0; i < 8; ++i ) {
				remo_SHT1106_I2C_write_command( 0xB0 + line );
				remo_SHT1106_I2C_write_command( 0x00 + I2C_12864_Offset );
				remo_SHT1106_I2C_write_command( 0x10 );
				line += 1;
				for( uint8 j = 0; j < 128; ++j ) {
					remo_SHT1106_I2C_write_data( BASE[addr] );
					addr++;
				}
			}
			break;
		
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
//=====================================================================
//SHT1106_I2C接口
int8 ST7920_12864_Offset;

void ST7920_12864Deal( uint32 v )
{
	uint16 index = 960;
	uint8 addr = 0x80;
	
	switch( v ) {
		//设置引脚
		case 0:
			u0 = Mem_Get_uint8( DP + 0 );
			u1 = Mem_Get_uint8( DP + 1 );
			u2 = Mem_Get_uint8( DP + 2 );
			remo_ST7920_SetPin( u0, u1, u2 );
			break;
		//初始化
		case 1:
			i0 = Mem_Get_int8( DP + 0 );
			ST7920_12864_Offset = i0;
			remo_ST7920_Init();
			break;
		//刷新
		case 2:
			u0 = Mem_Get_uint32( DP + 0 );
			index = 960;
			addr = 0x80;
			for( uint8 i = 0; i < 32; ++i ) {
				remo_ST7920_write_command( addr );
				remo_ST7920_write_command( 0x80 );
				for( uint8 j = 0; j < 16; ++j ) {
					remo_ST7920_write_data( BASE[u0+index] );
					index -= 64;
				}
				index += 1056; //1024 + 32;
				for( uint8 j = 0; j < 16; ++j ) {
					remo_ST7920_write_data( BASE[u0+index] );
					index -= 64;
				}
				index += 993; //1024 - 32 + 1;
				addr += 1;
			}
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
//=====================================================================
//ST7789接口

void ST7789_Deal( uint32 v )
{
	switch( v ) {
		//设置引脚
		case 0:
			u0 = Mem_Get_uint8( DP + 0 );
			u1 = Mem_Get_uint8( DP + 1 );
			u2 = Mem_Get_uint8( DP + 2 );
			u3 = Mem_Get_uint8( DP + 3 );
			remo_ST7789_SetPin( u0, u1, u2, u3 );
			break;
		//初始化
		case 1:
			i0 = Mem_Get_int32( DP + 0 );
			i1 = Mem_Get_int32( DP + 4 );
			remo_ST7789_Init( i0, i1 );
			break;
		case 2:
			i0 = Mem_Get_int32( DP + 0 );
			i1 = Mem_Get_int32( DP + 4 );
			remo_ST7789_LCD_draw_pixel( i0, i1 );
			break;
		case 3:
			i0 = Mem_Get_int32( DP + 0 );
			i1 = Mem_Get_int32( DP + 4 );
			remo_ST7789_LCD_clear_pixel( i0, i1 );
			break;
		case 4:
			u0 = Mem_Get_uint16( DP + 0 );
			remo_ST7789_LCD_Clear( u0 );
			break;
		case 5:
			u0 = Mem_Get_uint16( DP + 0 );
			u1 = Mem_Get_uint8( DP + 2 );
			remo_ST7789_LCD_set_fore( u0, (bool)u1 );
			break;
		case 6:
			u0 = Mem_Get_uint16( DP + 0 );
			u1 = Mem_Get_uint8( DP + 2 );
			remo_ST7789_LCD_set_back( u0, (bool)u1 );
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
//=====================================================================
//Speak接口

void Speak_Deal( uint32 v )
{
	switch( v ) {
		//设置引脚
		case 0:
			u0 = Mem_Get_uint8( DP + 0 );
			remo_tick_speaker_SetPin( u0 );
			break;
		//播放声音
		case 1:
			i0 = Mem_Get_int32( DP + 0 );
			remo_tick_speaker_start( i0 );
			break;
		//停止播放
		case 2:
			remo_tick_speaker_stop();
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
//=====================================================================
//motor接口

void Motor_Deal( uint32 v )
{
	switch( v ) {
		//设置引脚
		case 0:
			i0 = Mem_Get_int32( DP + 0 );
			u0 = Mem_Get_uint8( DP + 4 );
			u1 = Mem_Get_uint8( DP + 5 );
			u2 = Mem_Get_uint8( DP + 6 );
			u3 = Mem_Get_uint8( DP + 7 );
			remo_tick_motor_SetPin( i0, u0, u1, u2, u3 );
			break;
		//设置参数
		case 1:
			u0 = Mem_Get_uint8( DP + 0 );
			u1 = Mem_Get_uint8( DP + 1 );
			u2 = Mem_Get_uint8( DP + 2 );
			u3 = Mem_Get_uint8( DP + 3 );
			u4 = Mem_Get_uint8( DP + 4 );
			u5 = Mem_Get_uint8( DP + 5 );
			remo_tick_motor_set_value( u0, u1, u2, u3, u4, u5 );
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
//=====================================================================
//servo接口

void Servo_Deal( uint32 v )
{
	switch( v ) {
		//设置引脚
		case 0:
			u0 = Mem_Get_uint8( DP + 0 );
			u1 = Mem_Get_uint8( DP + 1 );
			remo_tick_servo_SetPin( u0, u1 );
			break;
		//设置参数
		case 1:
			u0 = Mem_Get_uint8( DP + 0 );
			i0 = Mem_Get_int32( DP + 1 );
			remo_tick_servo_SetValue( u0, i0 );
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
//=====================================================================
//SUART接口

void SUART_Deal( uint32 v )
{
	switch( v ) {
		//向指定引脚发送数据
		case 0:
			u0 = Mem_Get_uint8( DP + 0 );
			i0 = Mem_Get_int32( DP + 1 ); //波特率-暂时未用
			u1 = Mem_Get_uint8( DP + 5 );
			remo_tick_SUART_SetValue( u0, u1 );
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
//=====================================================================
//WS2812接口

void WS2812_Deal( uint32 v )
{
	switch( v ) {
		//向指定引脚发送数据
		case 0:
			u0 = Mem_Get_uint8( DP + 0 );
			u1 = Mem_Get_uint8( DP + 1 );
			u2 = Mem_Get_uint8( DP + 2 );
			u3 = Mem_Get_uint8( DP + 3 );
			remo_tick_WS2812_SetValue( u0, u1, u2, u3 );
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
//=====================================================================
//WS2812接口

void WS2812Array_Deal( uint32 v )
{
	switch( v ) {
		//向指定引脚发送数据
		case 0:
			u0 = Mem_Get_uint8( DP + 0 );
			u1 = Mem_Get_uint16( DP + 1 );
			u2 = Mem_Get_uint32( DP + 3 );
			remo_tick_WS2812Array_SetValue( u0, u1, u2 );
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
//=====================================================================
//DS18B20接口

void DS18B20_Deal( uint32 v )
{
	switch( v ) {
		case 0:
			u0 = Mem_Get_uint8( DP + 0 );
			remo_tick_DS18B20_start_sd( u0 );
			break;
		case 1:
			u0 = Mem_Get_uint8( DP + 0 );
			u1 = Mem_Get_uint8( DP + 1 );
			remo_tick_DS18B20_write_byte( u0, u1 );
			break;
		case 2:
			u0 = Mem_Get_int8( DP + 0 );
			A = remo_tick_DS18B20_read_byte( u0 );
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
//=====================================================================
//DHT11接口

void DHT11_Deal( uint32 v )
{
	switch( v ) {
		case 0:
			u0 = Mem_Get_uint8( DP + 0 );
			A = remo_tick_DHT11_read( u0 );
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
//=====================================================================
//IR接口

void IR_Deal( uint32 v )
{
	switch( v ) {
		case 0:
			u0 = Mem_Get_uint8( DP + 0 );
			remo_tick_IR_SetPin( u0 );
			break;
		case 1:
			A = remo_tick_IR_read();
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
//=====================================================================
//ultrasonic接口

void ultrasonic_Deal( uint32 v )
{
	switch( v ) {
		case 0:
			u0 = Mem_Get_uint8( DP + 0 );
			u1 = Mem_Get_uint8( DP + 1 );
			A = remo_tick_ultrasonic_get_value( u0, u1 );
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
//=====================================================================
//CrossRol接口

void CrossRol_Deal( uint32 v )
{
	switch( v ) {
		case 0:
			u0 = Mem_Get_uint8( DP + 0 );
			u1 = Mem_Get_uint8( DP + 1 );
			A = remo_tick_CrossRol_create( u0, u1 );
			break;
		case 1:
			i0 = Mem_Get_int8( DP + 0 );
			A = remo_tick_CrossRol_get_value( i0 );
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}

//=====================================================================
//BufferOper接口

void BufferOper_Deal( uint32 v )
{
	switch( v ) {
		case 0:
			i0 = Mem_Get_int16( DP + 0 );
			u1 = Mem_Get_uint32( DP + 2 );
			i2 = Mem_Get_int32( DP + 6 );
			i3 = Mem_Get_int32( DP + 10 );
			i4 = Mem_Get_int16( DP + 14 );
			remo_tick_BufferOper_SetWH( i0, u1, i2, i3, i4 );
			break;
		case 1:
			u0 = Mem_Get_int16( DP + 0 );
			u1 = Mem_Get_uint32( DP + 1 );
			u2 = Mem_Get_int32( DP + 2 );
			remo_tick_BufferOper_SetSwap( u0, u1, u2 );
			break;
		case 2:
			i0 = Mem_Get_int32( DP + 0 );
			remo_tick_BufferOper_SetPenWidth( i0 );
			break;
		case 3:
			i0 = Mem_Get_int32( DP + 0 );
			remo_tick_BufferOper_SetForColor( i0 );
			break;
		case 4:
			i0 = Mem_Get_int32( DP + 0 );
			remo_tick_BufferOper_SetBackColor( i0 );
			break;
		case 5:
			i0 = Mem_Get_int32( DP + 0 );
			i1 = Mem_Get_int32( DP + 4 );
			remo_tick_BufferOper_pixel( i0, i1 );
			break;
		case 6:
			i0 = Mem_Get_int32( DP + 0 );
			i1 = Mem_Get_int32( DP + 4 );
			remo_tick_BufferOper_clear_pixel( i0, i1 );
			break;
		case 7:
			i0 = Mem_Get_int32( DP + 0 );
			i1 = Mem_Get_int32( DP + 4 );
			A = remo_tick_BufferOper_get_pixel( i0, i1 );
			break;
		case 8:
			u0 = Mem_Get_uint8( DP + 0 );
			remo_tick_BufferOper_Clear( u0 );
			break;
		
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}

//=====================================================================
//GUI接口

void GUI_Deal( uint32 v )
{
	switch( v ) {

		case 1:
			u0 = Mem_Get_uint8( DP + 0 );
			u1 = Mem_Get_uint32( DP + 1 );
			i0 = Mem_Get_int32( DP + 5 );
			i1 = Mem_Get_int32( DP + 9 );
			A = remo_tick_GUI_Draw( u0, u1, i0, i1 );
			break;
		
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}

//=====================================================================
//GUIchar接口

void GUIchar_Deal( uint32 v )
{
	switch( v ) {

		case 1:
			i0 = Mem_Get_int8( DP + 0 );
			u0 = Mem_Get_uint32( DP + 1 );
			u1 = Mem_Get_uint16( DP + 5 );
			i1 = Mem_Get_int16( DP + 7 );
			i2 = Mem_Get_int16( DP + 9 );
			remo_tick_GUIchar_Draw( i0, u0, u1, i1, i2 );
			break;
		
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}

//=====================================================================
//Uart接口

void Uart_Deal( uint32 v )
{
	switch( v ) {
		case 0:
			i0 = Mem_Get_int8( DP + 0 );
			u1 = Mem_Get_uint32( DP + 1 );
			i2 = Mem_Get_int16( DP + 5 );
			i3 = Mem_Get_int32( DP + 7 );
			remo_USART_set( i0, u1, i2, i3 );
			break;
		case 1:
			i0 = Mem_Get_int8( DP + 0 );
			i1 = Mem_Get_int32( DP + 1 );
			remo_USART_Baudrate( i0, i1 );
			break;
		case 2:
			i0 = Mem_Get_int8( DP + 0 );
			u1 = Mem_Get_uint8( DP + 1 );
			remo_USART_write_byte( i0, u1 );
			break;
		/*
		case 3:
			u0 = Mem_Get_uint8( DP + 0 );
			A = Proto_GetData( u0 );
			break;
		*/
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}

//=====================================================================
//StepMotor接口

void StepMotor_Deal( uint32 v )
{
	switch( v ) {
		case 0:
			u0 = Mem_Get_uint8( DP + 0 );
			u1 = Mem_Get_uint8( DP + 1 );
			remo_tick_StepMotor_create( u0, u1 );
			break;
		case 1:
			u0 = Mem_Get_int8( DP + 0 );
			u1 = Mem_Get_uint32( DP + 4 );
			u2 = Mem_Get_uint32( DP + 8 );
			u3 = Mem_Get_uint32( DP + 12 );
			u4 = Mem_Get_uint32( DP + 16 );
			u5 = Mem_Get_uint32( DP + 20 );
			u6 = Mem_Get_uint32( DP + 24 );
			u7 = Mem_Get_uint32( DP + 28 );
			u8 = Mem_Get_uint32( DP + 32 );
			remo_tick_StepMotor_Set( u0, u1, u2, u3, u4, u5, u6, u7, u8 );
			break;
		case 2:
			u0 = Mem_Get_uint8( DP + 0 );
			remo_tick_StepMotor_SetParam( u0 );
			break;
		
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}

//=====================================================================

//NET wifi AP接口
void wifi_init_softap(uint8 *p_SSID, uint8 *p_PASS);
void NET_wifiAP_softap( uint32 ssid, uint32 pass );

extern bool wifiAP_isConnected;

void NET_wifiAP_Deal( uint32 v )
{
	switch( v ) {
		case 0:
			u0 = Mem_Get_uint32( DP + 0 );
			u1 = Mem_Get_uint32( DP + 4 );
			NET_wifiAP_softap( u0, u1 );
			break;
		case 1:
			A = wifiAP_isConnected;
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
void NET_wifiAP_softap( uint32 ssid, uint32 pass )
{
	uint8 *p_SSID = Mem_GetAddr( ssid );
	uint8 *p_PASS = Mem_GetAddr( pass );;
	
	//初始化wifi
	wifi_init_softap( p_SSID, p_PASS );
}

//=====================================================================

//NET wifi接口
void NET_wifiSTA_softap( uint32 ssid, uint32 pass );
void wifi_init_sta(uint8 *p_SSID, uint8 *p_PASS);
void wifiSTA_VOS_Reconnect(void);
void wifiSTA_VOS_Disconnect(void);

extern bool wifiSTA_isConnected;

void NET_wifiSTA_Deal( uint32 v )
{
	switch( v ) {
		case 0:
			
			break;
		case 1:
			u0 = Mem_Get_uint32( DP + 0 );
			u1 = Mem_Get_uint32( DP + 4 );
			NET_wifiSTA_softap( u0, u1 );
			break;
		case 2:
			A = wifiSTA_isConnected;
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
void NET_wifiSTA_softap( uint32 ssid, uint32 pass )
{
	uint8 *p_SSID = Mem_GetAddr( ssid );
	uint8 *p_PASS = Mem_GetAddr( pass );;
	
	//初始化wifi
	wifi_init_sta( p_SSID, p_PASS );
}
//=====================================================================
//NET TCP_Server接口

void Main_TCP_Init( uint32 buf, int32 n );
void Main_StartTCP( int32 port );
void NET_TCP_Server_SendString( uint32 text );
void TCP_Server_SendString(uint8 *p );

extern bool TCP_isConnected;
extern int32 TCP_RLength;

void NET_TCP_Server_Deal( uint32 v )
{
	switch( v ) {
		case 0:
			u0 = Mem_Get_uint32( DP + 0 );
			i0 = Mem_Get_int32( DP + 4 );
			Main_TCP_Init( u0, i0 );
			break;
		case 1:
			i0 = Mem_Get_int32( DP + 0 );
			Main_StartTCP( i0 );
			break;
		case 2:
			A = TCP_isConnected;
			break;
		case 3:
			u0 = Mem_Get_uint32( DP + 0 );
			NET_TCP_Server_SendString( u0 );
			break;
		case 4:
			A = TCP_RLength;
			TCP_RLength = 0;
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
void NET_TCP_Server_SendString( uint32 addr )
{
	uint8 *p = Mem_GetAddr( addr );
	//发送字符串
	TCP_Server_SendString( p );
}

//=====================================================================
//NET TCP_Client接口

void Main_TCP_Client_Init( uint32 buf, int32 n );
void Main_TCP_Client_Connect( uint32 IP, int32 port );
void NET_TCP_Client_SendString( uint32 text );
void TCP_Client_SendString(uint8 *p );

extern bool TCP_Client_isConnected;
extern int32 TCP_Client_RLength;

void NET_TCP_Client_Deal( uint32 v )
{
	switch( v ) {
		case 0:
			u0 = Mem_Get_uint32( DP + 0 );
			i0 = Mem_Get_int32( DP + 4 );
			Main_TCP_Client_Init( u0, i0 );
			break;
		case 1:
			u0 = Mem_Get_uint32( DP + 0 );
			i0 = Mem_Get_int32( DP + 4 );
			Main_TCP_Client_Connect( u0, i0 );
			break;
		case 2:
			A = TCP_Client_isConnected;
			break;
		case 3:
			u0 = Mem_Get_uint32( DP + 0 );
			NET_TCP_Client_SendString( u0 );
			break;
		case 4:
			A = TCP_Client_RLength;
			TCP_Client_RLength = 0;
			break;
		//其他
		default:
			Error( E_VM_InterfaceError, (uint8)v );
			break;
	}
}
void NET_TCP_Client_SendString( uint32 addr )
{
	uint8 *p = Mem_GetAddr( addr );
	
	//发送字符串
	TCP_Client_SendString( p );
}

//=====================================================================
//Openharmony 接口

#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"

void OpenHarmony_Deal( uint32 v )
{
	switch( v ) {
		case 0:
			i0 = Mem_Get_int32( DP + 0 );
			usleep( i0 );
			break;
		
		default: break;
	}
}










