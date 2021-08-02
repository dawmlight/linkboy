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
#include "remo_device.h"
#include "VM_Mem.h"

	void Tick_VCC( uint16 i );
	void Tick_GND( uint16 i );
	void Run_VCC( uint16 i );
	void Run_GND( uint16 i );
	void Tick_Power( uint16 i );
	void Run_Power( uint16 i );
	void Run_Resister( uint16 i );
	void Run_Key( uint16 i );
	void Run_Diode( uint16 i );
	void Run_Capacity( uint16 i );
	void Tick_Capacity( uint16 i );
	void Run_Inductance( uint16 i );
	void Tick_Inductance( uint16 i );
	void Run_Triode( uint16 i );
	void Run_OP_Amp( uint16 i );

	//---------------------------------------------------
	
	void Run_PinInput( uint16 i );
	void Run_PinOutput( uint16 i );
	void Run_Input( uint16 i );
	void Run_Input1( uint16 i );
	//void Run_Trans_双向传输备用( uint16 i );
	void Run_Trans( uint16 i );
	void Run_Not( uint16 i );
	void Run_Or( uint16 i );
	void Run_And( uint16 i );
	void Run_Or3( uint16 i );
	void Run_And3( uint16 i );
	void Run_OrNot( uint16 i );
	void Run_AndNot( uint16 i );
	void Run_AndNot3( uint16 i );
	void Run_OrNot3( uint16 i );
	void Run_Xor( uint16 i );
	void Run_Xnor( uint16 i );

	//---------------------------------------------------
	
	void Run_TRS( uint16 i );
	void Tick_TRS( uint16 i );
	void Run_CP_D( uint16 i );
	void Tick_CP_D( uint16 i );
	void Run_CP_JK( uint16 i );
	void Tick_CP_JK( uint16 i );

	//---------------------------------------------------
	
	void Run_QCount8( uint16 i );
	void Tick_QCount8( uint16 i );

	//---------------------------------------------------
	
	void Run_QCount8L( uint16 i );
	void Tick_QCount8L( uint16 i );

	//---------------------------------------------------
	
	void Run_Lock8( uint16 i );
	void Tick_Lock8( uint16 i );

	//---------------------------------------------------
	
	void Run_OSC( uint16 i );
	void Tick_OSC( uint16 i );
	void Run_ROM8X8( uint16 i );
	void Run_CMP( uint16 i );
	void Run_DCD38( uint16 i );




const float FixScale = 1024.0f;

int32 VI_Addr;
int32 NL_Addr;

int32 StartTime;
const int32 MaxStartTime = 10;

void C_ComV( void );
void C_Run( void );
void C_Tick( void );

enum E {
		E_NULL, E_VCC, E_GND, 
		E_Capacity, E_Inductance, E_Diode, E_Key, E_OP_Amp, E_Power, E_Resister, E_Triode, E_LED, E_DResister,
		E_Input, E_Input1, E_Trans, E_Not, E_And, E_And3, E_AndNot, E_AndNot3, E_Or, E_Or3, E_OrNot, E_OrNot3, E_Xor, E_Xnor, 
		E_ZAdd, 
		E_T_RS, 
		E_M_D, 
		E_CP_D, E_CP_JK, 
		E_QCount8, E_QCount8L, 
		E_OSC, 
		E_ROM8X8, E_Lock8, E_DCD38, E_CMP, 
		E_D3Resister,
		E_PinInput,
};




	//---------------------------------------------------------------------------
	//可移植C代码
	
	//0-4 为器件参数
	const uint16 TYPE = 0;
	//const uint16 ST = 1; //已过时
	
	const uint16 X1 = 1;
	const uint16 X2 = 2;
	const uint16 X3 = 3;
	const uint16 X4 = 4;
	
	//5开始为器件动态标准参数
	const uint16 ECVI = 5;
	
	const uint16 EI1 = ECVI; const uint16 EV1 = ECVI+1;
	const uint16 EI2 = ECVI+2; const uint16 EV2 = ECVI+3;
	const uint16 EI3 = ECVI+4; const uint16 EV3 = ECVI+5;
	const uint16 EI4 = ECVI+6; const uint16 EV4 = ECVI+7;
	const uint16 EI5 = ECVI+8; const uint16 EV5 = ECVI+9;
	const uint16 EI6 = ECVI+10; const uint16 EV6 = ECVI+11;
	const uint16 EI7 = ECVI+12; const uint16 EV7 = ECVI+13;
	const uint16 EI8 = ECVI+14; const uint16 EV8 = ECVI+15;
	const uint16 EI9 = ECVI+16; const uint16 EV9 = ECVI+17;
	const uint16 EI10 = ECVI+18; const uint16 EV10 = ECVI+19;
	const uint16 EI11 = ECVI+20; const uint16 EV11 = ECVI+21;
	const uint16 EI12 = ECVI+22; const uint16 EV12 = ECVI+23;
	const uint16 EI13 = ECVI+24; const uint16 EV13 = ECVI+25;
	const uint16 EI14 = ECVI+26; const uint16 EV14 = ECVI+27;
	const uint16 EI15 = ECVI+28; const uint16 EV15 = ECVI+29;
	const uint16 EI16 = ECVI+30; const uint16 EV16 = ECVI+31;
	const uint16 EI17 = ECVI+32; const uint16 EV17 = ECVI+33;
	const uint16 EI18 = ECVI+34; const uint16 EV18 = ECVI+35;
	//上边这里修改后一定要同步修改COM.txt里的对应参数
	
	//这里修改后一定要同步修改COM.txt里的对应参数
	const uint16 VNumber = EV18 + 1;
	
	fix ElmList[50][41]; //VNumber 41
	uint16 ELength;
	
	//---------------------------------------------------
	//格式： 节点数目M + 循环[ 数目N + N*2个字节（模块ID+引脚ID）]
	uint8 NodeList[1024];
	
	uint8 ROM[256];
	
	//---------------------------------------------------
	//数字电路参数
	const fix Diogate = 500.0f;
	static const fix DC = 5000.0f;
	const fix DC_h2 = 2500.0f;
	
	//收敛速度/比例 - 一个神奇的数字
	fix SN;
	
	//支持的节点扇出 最大数量
	//const int32 LENGTH = 20;
	//public [LENGTH]->fix List;
	
	bool isOK;
	bool exist;
	
	void Run()
	{
		exist = false;
		
		if( StartTime == 0 ) {
			C_Tick();
		}
		int f = 0;
		int Number = 50; //500
		
		SN = 1;
		f = 0;
		do {
			isOK = true;
			C_Run();
			C_ComV();
			f += 1;
		}
		while( !isOK && f < Number );
		/*
		SN = 10;
		f = 0;
		do {
			isOK = true;
			C_Run();
			C_ComV();
			f += 1;
		}
		while( !isOK && f < Number );
		
		SN = 100;
		f = 0;
		do {
			isOK = true;
			C_Run();
			C_ComV();
			f += 1;
		}
		while( !isOK && f < Number );
		
		SN = 1000;
		f = 0;
		do {
			isOK = true;
			C_Run();
			C_ComV();
			f += 1;
		}
		while( !isOK && f < Number );
		
		SN = 10000;
		f = 0;
		do {
			isOK = true;
			C_Run();
			C_ComV();
			f += 1;
		}
		while( !isOK && f < 100 );
		*/
		
		if( exist ) {
			//G.MyRefresh(); G.CGPanel.MyRefresh();
			//#.OS.REMO_Refresh();
		}
		//#.F = f;
	}
	
	//=================================================================================
	
	//节点电压均衡
	void C_ComV()
	{
		//节点数目M
		uint16 M = NodeList[0];
		uint16 ci = 1;
		uint16 cn = 0;
		fix last = 0.0f;
		
		while( cn < M ) {
			uint16 nl = (uint8)(NodeList[ci] * 2);
			ci += 1;
			
			//计算累加和
			fix number = 0.0f;
			fix sum = 0.0f;
			for( uint16 i = ci; i < ci+nl; i += 2 ) {
				uint8 mi = NodeList[i];
				uint8 pi = (uint8)(NodeList[i+1] - 1);
				fix v = ElmList[mi][ECVI+pi*2+1];
				sum += v;
				number += 1.0f;
			}
			//调整为平均值
			fix B = sum / number;
			for( uint16 i = ci; i < ci+nl; i += 2 ) {
				uint8 mi = NodeList[i];
				uint8 pi = (uint8)(NodeList[i+1] - 1);
				ElmList[mi][ECVI+pi*2+1] = B;
			}
			ci += nl;
			cn += 1;
			
			//if( +(last-B) > 30.0f ) {
				isOK = false;
			//}
			last = B;
		}
	}
	
	//模块步进函数
	void C_Tick()
	{
		for( uint16 i = 0; i < ELength; i += 1 ) {
			int32 type = (int32)(ElmList[i][TYPE]);
			
			switch( type ) {
				case E_VCC:			Tick_VCC( i ); break;
				case E_GND:			Tick_GND( i ); break;
				case E_Power:		Tick_Power( i ); break;
				case E_Capacity:	Tick_Capacity( i ); break;
				case E_Inductance:	Tick_Inductance( i ); break;
				case E_CP_D:		Tick_CP_D( i ); break;
				case E_QCount8:		Tick_QCount8( i ); break;
				case E_QCount8L:	Tick_QCount8L( i ); break;
				case E_Lock8:		Tick_Lock8( i ); break;
				case E_T_RS:		Tick_TRS( i ); break;
				case E_CP_JK:		Tick_CP_JK( i ); break;
				case E_OSC:			Tick_OSC( i ); break;
				default: break;
			}
		}
	}
	
	//模块迭代函数
	void C_Run()
	{
		for( uint16 i = 0; i < ELength; i += 1 ) {
			
			if( StartTime > MaxStartTime/2 && StartTime%ELength == i ) {
				continue;
			}
			exist = true;
			
			int32 type = (int32)(ElmList[i][TYPE]);
			
			switch( type ) {
				case E_VCC:			Run_VCC( i ); break;
				case E_GND:			Run_GND( i ); break;
				case E_Capacity:	Run_Capacity( i ); break;
				case E_Inductance:	Run_Inductance( i ); break;
				case E_Diode:		Run_Diode( i ); break;
				case E_LED:			Run_Diode( i ); break;
				case E_Power:		Run_Power( i ); break;
				case E_Resister:	Run_Resister( i ); break;
				case E_DResister:	Run_Resister( i ); break;
				case E_Triode:		Run_Triode( i ); break;
				case E_OP_Amp:		Run_OP_Amp( i ); break;
				case E_Key:			Run_Key( i ); break;
				case E_PinInput:	Run_PinInput( i ); break;
				case E_Input:		Run_Input( i ); break;
				case E_Input1:		Run_Input1( i ); break;
				case E_Trans:		Run_Trans( i ); break;
				case E_Not:			Run_Not( i ); break;
				case E_Or:			Run_Or( i ); break;
				case E_And:			Run_And( i ); break;
				case E_Or3:			Run_Or3( i ); break;
				case E_And3:		Run_And3( i ); break;
				case E_OrNot:		Run_OrNot( i ); break;
				case E_AndNot:		Run_AndNot( i ); break;
				case E_AndNot3:		Run_AndNot3( i ); break;
				case E_OrNot3:		Run_OrNot3( i ); break;
				case E_Xor:			Run_Xor( i ); break;
				case E_Xnor:		Run_Xnor( i ); break;
				
				case E_CP_D:		Run_CP_D( i ); break;
				case E_QCount8:		Run_QCount8( i ); break;
				case E_QCount8L:	Run_QCount8L( i ); break;
				case E_Lock8:		Run_Lock8( i ); break;
				case E_T_RS:		Run_TRS( i ); break;
				case E_CP_JK:		Run_CP_JK( i ); break;
				case E_OSC:			Run_OSC( i ); break;
				case E_ROM8X8:		Run_ROM8X8( i ); break;
				case E_DCD38:		Run_DCD38( i ); break;
				case E_CMP:			Run_CMP( i ); break;
				default: break;
			}
		}
	}
	
	//---------------------------------------------------
	//已处理SN
	
	void Tick_VCC( uint16 i )
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		ElmList[i][X3] = I1;
	}
	void Tick_GND( uint16 i )
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		ElmList[i][X3] = I1;
	}
	void Run_VCC( uint16 i )
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		fix V = ElmList[i][X1];
		fix OFF = ElmList[i][X3];
		V += OFF;
		I1 = (V - V1);
		V1 = V;
		
		ElmList[i][EV1] = V1; ElmList[i][EI1] = I1;
	}
	void Run_GND( uint16 i )
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		fix OFF = ElmList[i][X3];
		fix G = OFF;
		I1 = (G - V1);
		V1 = G;
		
		ElmList[i][EV1] = V1; ElmList[i][EI1] = I1;
	}
	
	void Tick_Power( uint16 i )
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		fix V2 = ElmList[i][EV2]; fix I2 = ElmList[i][EI2];
		
		ElmList[i][X3] = I1;
		ElmList[i][X4] = I2;
	}
	
	void Run_Power( uint16 i )
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		fix V2 = ElmList[i][EV2]; fix I2 = ElmList[i][EI2];
		
		fix V = ElmList[i][X1];
		fix R = ElmList[i][X2];
		fix VCC_OFF = ElmList[i][X3];
		fix GND_OFF = ElmList[i][X4];
		
		V += VCC_OFF;
		fix G = GND_OFF;
		
		I1 = (V - V1);
		V1 += (V - V1)/SN;
		I2 = (G - V2);
		V2 += (G - V2)/SN;
		
		/*
		I1 = (V - V1) * 100;
		V1 = V + (V-V1) * 2;
		I2 = (0.0f - V2) * 100;
		V2 = - V2 * 2;
		*/
		
		ElmList[i][EV1] = V1; ElmList[i][EI1] = I1;
		ElmList[i][EV2] = V2; ElmList[i][EI2] = I2;
	}
	
	void Run_Resister( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		fix I1 = ElmList[i][EI1];
		fix I2 = ElmList[i][EI2];
		
		fix R = ElmList[i][X1];
		
		fix vv = V1 - V2;
		vv = vv / (R*1000+1);
		V1 += -vv / SN;
		V2 += vv / SN;
		I1 = -vv;
		I2 = -I1;
		
		ElmList[i][EV1] = V1;
		ElmList[i][EV2] = V2;
		ElmList[i][EI1] = I1;
		ElmList[i][EI2] = I2;
	}
	void Run_Key( uint16 i )
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		fix V2 = ElmList[i][EV2]; fix I2 = ElmList[i][EI2];
		
		fix key = ElmList[i][X1];
		
		if( key > 0.5f ) {
			fix vv = V1 - V2;
			//vv /= 2.0f;
			V1 -= vv/SN;
			V2 += vv/SN;
			I1 = -vv;
			I2 = -I1;
		}
		else {
			I1 = 0.0f;
			I2 = 0.0f;
		}
		ElmList[i][EV1] = V1; ElmList[i][EI1] = I1;
		ElmList[i][EV2] = V2; ElmList[i][EI2] = I2;
	}
	void Run_Diode( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		fix I1 = ElmList[i][EI1];
		fix I2 = ElmList[i][EI2];
		
		fix v0 = (V1 - V2);
		if( v0 >= Diogate ) {
			fix vv = v0 - Diogate;
			vv /= 10; //等效电阻
			V1 -= vv / SN;
			V2 += vv / SN;
			I1 = -vv;
			I2 = -I1;
		}
		else {
			I2 = 0.0f;
			I1 = 0.0f;
		}
		ElmList[i][EV1] = V1;
		ElmList[i][EV2] = V2;
		ElmList[i][EI1] = I1;
		ElmList[i][EI2] = I2;
	}
	
	void Run_Capacity( uint16 i )
	{
		fix Q = ElmList[i][X2];
		fix C = ElmList[i][X1];
		
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		fix V2 = ElmList[i][EV2]; fix I2 = ElmList[i][EI2];
		
		fix dv = V1 -(V2 + Q/C);
		dv /= 2.0f;
		V1 -= dv / SN;
		V2 += dv / SN;
		
		I1 = -dv;
		I2 = -I1;
		
		ElmList[i][EV1] = V1; ElmList[i][EI1] = I1;
		ElmList[i][EV2] = V2; ElmList[i][EI2] = I2;
	}
	void Tick_Capacity( uint16 i )
	{
		fix Q = ElmList[i][X2];
		fix C = ElmList[i][X1];
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		
		fix dv = V1 -(V2 + Q/C);
		dv /= 2.0f;
		
		Q += dv*50;
		//dv /= SN;
		//V1 -= dv;
		//V2 += dv;
		
		ElmList[i][X2] = Q;
	}
	
	void Run_Inductance( uint16 i )
	{
		fix Q = ElmList[i][X2];
		fix L = ElmList[i][X1];
		
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		fix V2 = ElmList[i][EV2]; fix I2 = ElmList[i][EI2];
		
		fix cv = (Q - I2) * L;
		V1 += cv/2;
		V2 -= cv/2;
		
		//fix dv = V1 -(V2 + Q/L);
		//dv /= 2.0f;
		//V1 -= dv / SN;
		//V2 += dv / SN;
		
		//I1 = -dv;
		//I2 = -I1;
		
		ElmList[i][EV1] = V1; ElmList[i][EI1] = I1;
		ElmList[i][EV2] = V2; ElmList[i][EI2] = I2;
	}
	void Tick_Inductance( uint16 i )
	{
		fix Q = ElmList[i][X2];
		fix L = ElmList[i][X1];
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		fix V2 = ElmList[i][EV2]; fix I2 = ElmList[i][EI2];
		
		Q = I2 / L;
		
		//fix dv = V1 -(V2 + cv);
		//dv /= 2.0f;
		
		//Q += dv*50;
		//dv /= SN;
		//V1 -= dv;
		//V2 += dv;
		
		ElmList[i][X2] = Q;
	}
	
	void Run_Triode( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		fix V3 = ElmList[i][EV3];
		fix I1 = ElmList[i][EI1];
		fix I2 = ElmList[i][EI2];
		fix I3 = ElmList[i][EI3];
		
		fix v0 = (V1 - V2);
		if( v0 >= Diogate ) {
			fix vv = v0 - Diogate;
			vv /= 500; //一定要加一个电阻, 否则无法收敛, 折腾好几天了....
			V1 -= vv / SN;
			fix ttt = vv;
			I1 = -vv;
			vv = vv*60;
			fix vv1 = V3 - V2;
			//vv1 /= 2.0f;
			if( vv > vv1 ) {
				vv = vv1;
			}
			if( vv < 0.0f ) {
				vv = 0.0f;
			}
			vv /= 2;
			V3 -= vv / SN;
			V2 += vv / SN + ttt / SN;
			I3 = -vv;
			I2 = vv + ttt;
		}
		else {
			I1 = 0.0f;
			I2 = 0.0f;
			I3 = 0.0f;
		}
		ElmList[i][EV1] = V1;
		ElmList[i][EV2] = V2;
		ElmList[i][EV3] = V3;
		ElmList[i][EI1] = I1;
		ElmList[i][EI2] = I2;
		ElmList[i][EI3] = I3;
	}
	void Run_OP_Amp( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		
		fix V3 = ElmList[i][EV3];
		fix I3 = ElmList[i][EI3];
		
		fix V4 = ElmList[i][EV4];
		fix I4 = ElmList[i][EI4];
		
		fix V5 = ElmList[i][EV5];
		fix I5 = ElmList[i][EI5];
		
		/*
		fix vv = (V1 - V2);
		vv *= 100.0f;
		vv += DC_h2;
		if( vv < 0.0f ) vv = 0.0f;
		//if( vv < -DC ) vv = -DC;
		if( vv > DC ) vv = DC;
		*/
		
		fix vv = (V1 - V2);
		if( vv >= 0.0f ) {
			vv = (V4 - V3);
			vv /= 1000;
			vv /= 2.0f;
			V4 -= vv;
			V3 += vv;
			I4 = -vv;
			I3 = -I4;
		}
		else {
			vv = (V5 - V3);
			vv /= 1000;
			vv /= 2.0f;
			V5 -= vv;
			V3 += vv;
			I5 = -vv;
			I3 = -I5;
		}
		
		ElmList[i][EV3] = V3;
		ElmList[i][EI3] = I3;
		
		ElmList[i][EV4] = V4;
		ElmList[i][EI4] = I4;
		
		ElmList[i][EV5] = V5;
		ElmList[i][EI5] = I5;
	}
	
	//---------------------------------------------------
	void Run_PinInput( uint16 i )
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		
		fix key = ElmList[i][X1];
		
		bool Out1 = key > 0.5;
		
		if( Out1 ) {
			I1 = DC - V1;
			V1 = DC;
		}
		else {
			I1 = -V1;
			V1 = 0.0f;
		}
		ElmList[i][EV1] = V1; ElmList[i][EI1] = I1;
	}
	//---------------------------------------------------
	void Run_PinOutput( uint16 i )
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		//...
	}
	void Run_Input( uint16 i )
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];

		fix key = ElmList[i][X1];

		bool Out1 = key > 0.5;

		if( Out1 ) {
			I1 = DC - V1;
			V1 = DC;
		}
		else {
			I1 = -V1;
			V1 = 0.0f;
		}
		
		ElmList[i][EV1] = V1; ElmList[i][EI1] = I1;
	}
	void Run_Input1( uint16 i )
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		
		fix key = ElmList[i][X1];
		
		bool Out1 = key > 0.5;
		Out1 = !Out1;
		
		if( Out1 ) {
			I1 = DC - V1;
			V1 = DC;
		}
		else {
			I1 = -V1;
			V1 = 0.0f;
		}
		
		ElmList[i][EV1] = V1; ElmList[i][EI1] = I1;
	}
	void Run_Trans_S( uint16 i ) //双向传输备用
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		fix V2 = ElmList[i][EV2]; fix I2 = ElmList[i][EI2];
		fix V3 = ElmList[i][EV3]; fix I3 = ElmList[i][EI3];
		
		if( V3 > DC_h2 ) {
			fix vv = V1 - V2;
			vv /= 2.0f;
			V1 -= vv;
			V2 += vv;
			I1 = -vv;
			I2 = -I1;
		}
		else {
			I1 = 0.0f;
			I2 = 0.0f;
		}
		ElmList[i][EV1] = V1; ElmList[i][EI1] = I1;
		ElmList[i][EV2] = V2; ElmList[i][EI2] = I2;
	}
	void Run_Trans( uint16 i )
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		fix V2 = ElmList[i][EV2]; fix I2 = ElmList[i][EI2];
		fix V3 = ElmList[i][EV3]; fix I3 = ElmList[i][EI3];
		
		if( V3 > DC_h2 ) {
			bool Out1 = V1 > DC_h2;
			if( Out1 ) {
				I2 = DC - V2; V2 = DC;
			}
			else {
				I2 = -V2; V2 = 0.0f;
			}
		}
		else {
			I2 = 0.0f;
		}
		ElmList[i][EV2] = V2; ElmList[i][EI2] = I2;
	}
	void Run_Not( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		
		fix V2 = ElmList[i][EV2];
		fix I2 = ElmList[i][EI2];
		
		bool Out1 = V1 <= DC_h2;
		
		if( Out1 ) {
			I2 = DC - V2;
			V2 = DC;
		}
		else {
			I2 = -V2;
			V2 = 0.0f;
		}
		
		ElmList[i][EV2] = V2;
		ElmList[i][EI2] = I2;
	}
	void Run_Or( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		
		fix V3 = ElmList[i][EV3];
		fix I3 = ElmList[i][EI3];
		
		bool Out1 = V1 > DC_h2 || V2 > DC_h2;

		if( Out1 ) {
			I3 = DC - V3;
			V3 = DC;
		}
		else {
			I3 = -V3;
			V3 = 0.0f;
		}
		
		ElmList[i][EV3] = V3;
		ElmList[i][EI3] = I3;
	}
	void Run_And( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		
		fix V3 = ElmList[i][EV3];
		fix I3 = ElmList[i][EI3];
		
		bool Out1 = V1 > DC_h2 && V2 > DC_h2;

		if( Out1 ) {
			I3 = DC - V3;
			V3 = DC;
		}
		else {
			I3 = -V3;
			V3 = 0.0f;
		}
		
		ElmList[i][EV3] = V3;
		ElmList[i][EI3] = I3;
	}
	void Run_Or3( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		fix V3 = ElmList[i][EV3];
		
		fix V4 = ElmList[i][EV4];
		fix I4 = ElmList[i][EI4];
		
		bool Out1 = V1 > DC_h2 || V2 > DC_h2 || V3 > DC_h2;

		if( Out1 ) {
			I4 = DC - V4;
			V4 = DC;
		}
		else {
			I4 = -V4;
			V4 = 0.0f;
		}
		
		ElmList[i][EV4] = V4;
		ElmList[i][EI4] = I4;
	}
	void Run_And3( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		fix V3 = ElmList[i][EV3];
		
		fix V4 = ElmList[i][EV4];
		fix I4 = ElmList[i][EI4];
		
		bool Out1 = V1 > DC_h2 && V2 > DC_h2 && V3 > DC_h2;

		if( Out1 ) {
			I4 = DC - V4;
			V4 = DC;
		}
		else {
			I4 = -V4;
			V4 = 0.0f;
		}
		
		ElmList[i][EV4] = V4;
		ElmList[i][EI4] = I4;
	}
	void Run_OrNot( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		
		fix V3 = ElmList[i][EV3];
		fix I3 = ElmList[i][EI3];
		
		bool Out1 = V1 > DC_h2 || V2 > DC_h2;
		Out1 = !Out1;

		if( Out1 ) {
			I3 = DC - V3;
			V3 = DC;
		}
		else {
			I3 = -V3;
			V3 = 0.0f;
		}
		
		ElmList[i][EV3] = V3;
		ElmList[i][EI3] = I3;
	}
	void Run_AndNot( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		
		fix V3 = ElmList[i][EV3];
		fix I3 = ElmList[i][EI3];
		
		bool Out1 = V1 > DC_h2 && V2 > DC_h2;
		Out1 = !Out1;
		
		if( Out1 ) {
			I3 = DC - V3;
			V3 = DC;
		}
		else {
			I3 = -V3;
			V3 = 0.0f;
		}
		
		ElmList[i][EV3] = V3;
		ElmList[i][EI3] = I3;
	}
	void Run_AndNot3( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		fix V3 = ElmList[i][EV3];
		
		fix V4 = ElmList[i][EV4];
		fix I4 = ElmList[i][EI4];
		
		bool Out1 = V1 > DC_h2 && V2 > DC_h2 && V3 > DC_h2;
		Out1 = !Out1;
		
		if( Out1 ) {
			I4 = DC - V4;
			V4 = DC;
		}
		else {
			I4 = -V4;
			V4 = 0.0f;
		}
		
		ElmList[i][EV4] = V4;
		ElmList[i][EI4] = I4;
	}
	void Run_OrNot3( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		fix V3 = ElmList[i][EV3];
		
		fix V4 = ElmList[i][EV4];
		fix I4 = ElmList[i][EI4];
		
		bool Out1 = V1 > DC_h2 || V2 > DC_h2 || V3 > DC_h2;
		Out1 = !Out1;
		
		if( Out1 ) {
			I4 = DC - V4;
			V4 = DC;
		}
		else {
			I4 = -V4;
			V4 = 0.0f;
		}
		
		ElmList[i][EV4] = V4;
		ElmList[i][EI4] = I4;
	}
	void Run_Xor( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		
		fix V3 = ElmList[i][EV3];
		fix I3 = ElmList[i][EI3];
		
		bool Out1 = (V1 > DC_h2) != (V2 > DC_h2);
		
		if( Out1 ) {
			I3 = DC - V3;
			V3 = DC;
		}
		else {
			I3 = -V3;
			V3 = 0.0f;
		}
		
		ElmList[i][EV3] = V3;
		ElmList[i][EI3] = I3;
	}
	void Run_Xnor( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		
		fix V3 = ElmList[i][EV3];
		fix I3 = ElmList[i][EI3];
		
		bool Out1 = (V1 > DC_h2) == (V2 > DC_h2);

		if( Out1 ) {
			I3 = DC - V3;
			V3 = DC;
		}
		else {
			I3 = -V3;
			V3 = 0.0f;
		}
		
		ElmList[i][EV3] = V3;
		ElmList[i][EI3] = I3;
	}
	//---------------------------------------------------
	
	void Run_TRS( uint16 i )
	{
		fix V3 = ElmList[i][EV3]; fix I3 = ElmList[i][EI3];
		fix V4 = ElmList[i][EV4]; fix I4 = ElmList[i][EI4];
		
		fix V3S = ElmList[i][X2];
		
		bool Out2 = V3S > DC_h2;
		if( Out2 ) {
			I3 = DC - V3;
			V3 = DC;
		}
		else {
			I3 = -V3;
			V3 = 0.0f;
		}
		if( !Out2 ) {
			I4 = DC - V4;
			V4 = DC;
		}
		else {
			I4 = -V4;
			V4 = 0.0f;
		}
		ElmList[i][EV3] = V3; ElmList[i][EI3] = I3;
		ElmList[i][EV4] = V4; ElmList[i][EI4] = I4;
	}
	void Tick_TRS( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		
		fix x2 = ElmList[i][X2];
		
		bool Out1 = (V1 <= DC_h2 && V2 > DC_h2);
		if( Out1 ) {
			x2 = DC;
		}
		Out1 = (V1 > DC_h2 && V2 <= DC_h2);
		if( Out1 ) {
			x2 = 0.0f;
		}
		ElmList[i][X2] = x2;
	}
	
	void Run_CP_D( uint16 i )
	{
		fix V3 = ElmList[i][EV3]; fix I3 = ElmList[i][EI3];
		fix V4 = ElmList[i][EV4]; fix I4 = ElmList[i][EI4];
		
		fix V3S = ElmList[i][X2];
		
		bool Out2 = V3S > DC_h2;
		if( Out2 ) {
			I3 = DC - V3;
			V3 = DC;
		}
		else {
			I3 = -V3;
			V3 = 0.0f;
		}
		if( !Out2 ) {
			I4 = DC - V4;
			V4 = DC;
		}
		else {
			I4 = -V4;
			V4 = 0.0f;
		}
		ElmList[i][EV3] = V3; ElmList[i][EI3] = I3;
		ElmList[i][EV4] = V4; ElmList[i][EI4] = I4;
	}
	void Tick_CP_D( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		
		fix x1 = ElmList[i][X1];
		fix x2 = ElmList[i][X2];
		
		bool Out1 = (x1 <= DC_h2 && V1 > DC_h2);
		if( Out1 ) {
			x2 = V2;
		}
		
		ElmList[i][X1] = V1;
		ElmList[i][X2] = x2;
	}
	
	void Run_CP_JK( uint16 i )
	{
		fix V4 = ElmList[i][EV4]; fix I4 = ElmList[i][EI4];
		fix V5 = ElmList[i][EV5]; fix I5 = ElmList[i][EI5];
		
		fix V3S = ElmList[i][X2];
		
		bool Out2 = V3S > DC_h2;
		if( Out2 ) {
			I4 = DC - V4;
			V4 = DC;
		}
		else {
			I4 = -V4;
			V4 = 0.0f;
		}
		if( !Out2 ) {
			I5 = DC - V5;
			V5 = DC;
		}
		else {
			I5 = -V5;
			V5 = 0.0f;
		}
		ElmList[i][EV4] = V4; ElmList[i][EI4] = I4;
		ElmList[i][EV5] = V5; ElmList[i][EI5] = I5;
	}
	void Tick_CP_JK( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		fix V3 = ElmList[i][EV3];
		
		fix x1 = ElmList[i][X1];
		fix x2 = ElmList[i][X2];
		
		bool Out1 = (x1 <= DC_h2 && V2 > DC_h2);
		//bool Out1 = V2 > DC_h2;
		if( Out1 ) {
			Out1 = (V1 <= DC_h2 && V3 > DC_h2);
			if( Out1 ) {
				x2 = 0.0f;
			}
			Out1 = (V1 > DC_h2 && V3 <= DC_h2);
			if( Out1 ) {
				x2 = DC;
			}
			Out1 = (V1 > DC_h2 && V3 > DC_h2);
			if( Out1 ) {
				if( x2 > DC_h2 ) {
					x2 = 0.0f;
				}
				else {
					x2 = DC;
				}
			}
		}
		
		ElmList[i][X1] = V2;
		ElmList[i][X2] = x2;
	}
	
	//---------------------------------------------------
	
	void Run_QCount8( uint16 i )
	{
		fix V2 = ElmList[i][EV2]; fix I2 = ElmList[i][EI2];
		fix V3 = ElmList[i][EV3]; fix I3 = ElmList[i][EI3];
		fix V4 = ElmList[i][EV4]; fix I4 = ElmList[i][EI4];
		fix V5 = ElmList[i][EV5]; fix I5 = ElmList[i][EI5];
		fix V6 = ElmList[i][EV6]; fix I6 = ElmList[i][EI6];
		fix V7 = ElmList[i][EV7]; fix I7 = ElmList[i][EI7];
		fix V8 = ElmList[i][EV8]; fix I8 = ElmList[i][EI8];
		fix V9 = ElmList[i][EV9]; fix I9 = ElmList[i][EI9];
		
		//寄存器的值
		int xx2 = (int)ElmList[i][X2];
		
		if( (xx2 & 0x01) != 0 ) {
			I2 = DC - V2;
			V2 = DC;
		}
		else {
			I2 = -V2;
			V2 = 0.0f;
		}
		if( (xx2 & 0x02) != 0 ) {
			I3 = DC - V3;
			V3 = DC;
		}
		else {
			I3 = -V3;
			V3 = 0.0f;
		}
		if( (xx2 & 0x04) != 0 ) {
			I4 = DC - V4;
			V4 = DC;
		}
		else {
			I4 = -V4;
			V4 = 0.0f;
		}
		if( (xx2 & 0x08) != 0 ) {
			I5 = DC - V5;
			V5 = DC;
		}
		else {
			I5 = -V5;
			V5 = 0.0f;
		}
		if( (xx2 & 0x10) != 0 ) {
			I6 = DC - V6;
			V6 = DC;
		}
		else {
			I6 = -V6;
			V6 = 0.0f;
		}
		if( (xx2 & 0x20) != 0 ) {
			I7 = DC - V7;
			V7 = DC;
		}
		else {
			I7 = -V7;
			V7 = 0.0f;
		}
		if( (xx2 & 0x40) != 0 ) {
			I8 = DC - V8;
			V8 = DC;
		}
		else {
			I8 = -V8;
			V8 = 0.0f;
		}
		if( (xx2 & 0x80) != 0 ) {
			I9 = DC - V9;
			V9 = DC;
		}
		else {
			I9 = -V9;
			V9 = 0.0f;
		}
		ElmList[i][EV2] = V2; ElmList[i][EI2] = I2;
		ElmList[i][EV3] = V3; ElmList[i][EI3] = I3;
		ElmList[i][EV4] = V4; ElmList[i][EI4] = I4;
		ElmList[i][EV5] = V5; ElmList[i][EI5] = I5;
		ElmList[i][EV6] = V6; ElmList[i][EI6] = I6;
		ElmList[i][EV7] = V7; ElmList[i][EI7] = I7;
		ElmList[i][EV8] = V8; ElmList[i][EI8] = I8;
		ElmList[i][EV9] = V9; ElmList[i][EI9] = I9;
	}
	void Tick_QCount8( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		
		fix x1 = ElmList[i][X1];
		fix x2 = ElmList[i][X2];
		
		bool Out1 = (x1 <= DC_h2 && V1 > DC_h2);
		if( Out1 ) {
			int xx = (int)x2;
			xx += 1;
			xx %= 256;
			x2 = xx + 0.5f;
		}
		
		ElmList[i][X1] = V1;
		ElmList[i][X2] = x2;
	}
	
	//---------------------------------------------------
	
	void Run_QCount8L( uint16 i )
	{
		fix V11 = ElmList[i][EV11]; fix I11 = ElmList[i][EI11];
		fix V12 = ElmList[i][EV12]; fix I12 = ElmList[i][EI12];
		fix V13 = ElmList[i][EV13]; fix I13 = ElmList[i][EI13];
		fix V14 = ElmList[i][EV14]; fix I14 = ElmList[i][EI14];
		fix V15 = ElmList[i][EV15]; fix I15 = ElmList[i][EI15];
		fix V16 = ElmList[i][EV16]; fix I16 = ElmList[i][EI16];
		fix V17 = ElmList[i][EV17]; fix I17 = ElmList[i][EI17];
		fix V18 = ElmList[i][EV18]; fix I18 = ElmList[i][EI18];
		
		//寄存器的值
		int xx3 = (int)ElmList[i][X3];
		
		if( (xx3 & 0x01) != 0 ) {
			I11 = DC - V11;
			V11 = DC;
		}
		else {
			I11 = -V11;
			V11 = 0.0f;
		}
		if( (xx3 & 0x02) != 0 ) {
			I12 = DC - V12;
			V12 = DC;
		}
		else {
			I12 = -V12;
			V12 = 0.0f;
		}
		if( (xx3 & 0x04) != 0 ) {
			I13 = DC - V13;
			V13 = DC;
		}
		else {
			I13 = -V13;
			V13 = 0.0f;
		}
		if( (xx3 & 0x08) != 0 ) {
			I14 = DC - V14;
			V14 = DC;
		}
		else {
			I14 = -V14;
			V14 = 0.0f;
		}
		if( (xx3 & 0x10) != 0 ) {
			I15 = DC - V15;
			V15 = DC;
		}
		else {
			I15 = -V15;
			V15 = 0.0f;
		}
		if( (xx3 & 0x20) != 0 ) {
			I16 = DC - V16;
			V16 = DC;
		}
		else {
			I16 = -V16;
			V16 = 0.0f;
		}
		if( (xx3 & 0x40) != 0 ) {
			I17 = DC - V17;
			V17 = DC;
		}
		else {
			I17 = -V17;
			V17 = 0.0f;
		}
		if( (xx3 & 0x80) != 0 ) {
			I18 = DC - V18;
			V18 = DC;
		}
		else {
			I18 = -V18;
			V18 = 0.0f;
		}
		ElmList[i][EV11] = V11; ElmList[i][EI11] = I11;
		ElmList[i][EV12] = V12; ElmList[i][EI12] = I12;
		ElmList[i][EV13] = V13; ElmList[i][EI13] = I13;
		ElmList[i][EV14] = V14; ElmList[i][EI14] = I14;
		ElmList[i][EV15] = V15; ElmList[i][EI15] = I15;
		ElmList[i][EV16] = V16; ElmList[i][EI16] = I16;
		ElmList[i][EV17] = V17; ElmList[i][EI17] = I17;
		ElmList[i][EV18] = V18; ElmList[i][EI18] = I18;
	}
	void Tick_QCount8L( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		fix V2 = ElmList[i][EV2];
		fix V3 = ElmList[i][EV3];
		fix V4 = ElmList[i][EV4];
		fix V5 = ElmList[i][EV5];
		fix V6 = ElmList[i][EV6];
		fix V7 = ElmList[i][EV7];
		fix V8 = ElmList[i][EV8];
		fix V9 = ElmList[i][EV9];
		fix V10 = ElmList[i][EV10];
		
		fix x1 = ElmList[i][X1];
		fix x2 = ElmList[i][X2];
		
		fix x3 = ElmList[i][X3];
		
		bool Out1 = (x1 <= DC_h2 && V1 > DC_h2);
		if( Out1 ) {
			int xx = (int)x3;
			xx += 1;
			xx %= 256;
			x3 = xx + 0.5f;
		}
		bool Out2 = (x2 <= DC_h2 && V2 > DC_h2);
		if( Out2 ) {
			int Addr = 0;
			if( V3 > DC_h2 ) Addr |= 0x01;
			if( V4 > DC_h2 ) Addr |= 0x02;
			if( V5 > DC_h2 ) Addr |= 0x04;
			if( V6 > DC_h2 ) Addr |= 0x08;
			if( V7 > DC_h2 ) Addr |= 0x10;
			if( V8 > DC_h2 ) Addr |= 0x20;
			if( V9 > DC_h2 ) Addr |= 0x40;
			if( V10 > DC_h2 ) Addr |= 0x80;
			x3 = Addr + 0.5f;
		}
		ElmList[i][X1] = V1;
		ElmList[i][X2] = V2;
		ElmList[i][X3] = x3;
	}
	
	//---------------------------------------------------
	
	void Run_Lock8( uint16 i )
	{
		fix V10 = ElmList[i][EV10]; fix I10 = ElmList[i][EI10];
		fix V11 = ElmList[i][EV11]; fix I11 = ElmList[i][EI11];
		fix V12 = ElmList[i][EV12]; fix I12 = ElmList[i][EI12];
		fix V13 = ElmList[i][EV13]; fix I13 = ElmList[i][EI13];
		fix V14 = ElmList[i][EV14]; fix I14 = ElmList[i][EI14];
		fix V15 = ElmList[i][EV15]; fix I15 = ElmList[i][EI15];
		fix V16 = ElmList[i][EV16]; fix I16 = ElmList[i][EI16];
		fix V17 = ElmList[i][EV17]; fix I17 = ElmList[i][EI17];
		
		//寄存器的值
		int xx2 = (int)ElmList[i][X2];
		
		if( (xx2 & 0x01) != 0 ) {
			I10 = DC - V10;
			V10 = DC;
		}
		else {
			I10 = -V10;
			V10 = 0.0f;
		}
		if( (xx2 & 0x02) != 0 ) {
			I11 = DC - V11;
			V11 = DC;
		}
		else {
			I11 = -V11;
			V11 = 0.0f;
		}
		if( (xx2 & 0x04) != 0 ) {
			I12 = DC - V12;
			V12 = DC;
		}
		else {
			I12 = -V12;
			V12 = 0.0f;
		}
		if( (xx2 & 0x08) != 0 ) {
			I13 = DC - V13;
			V13 = DC;
		}
		else {
			I13 = -V13;
			V13 = 0.0f;
		}
		if( (xx2 & 0x10) != 0 ) {
			I14 = DC - V14;
			V14 = DC;
		}
		else {
			I14 = -V14;
			V14 = 0.0f;
		}
		if( (xx2 & 0x20) != 0 ) {
			I15 = DC - V15;
			V15 = DC;
		}
		else {
			I15 = -V15;
			V15 = 0.0f;
		}
		if( (xx2 & 0x40) != 0 ) {
			I16 = DC - V16;
			V16 = DC;
		}
		else {
			I16 = -V16;
			V16 = 0.0f;
		}
		if( (xx2 & 0x80) != 0 ) {
			I17 = DC - V17;
			V17 = DC;
		}
		else {
			I17 = -V17;
			V17 = 0.0f;
		}
		ElmList[i][EV10] = V10; ElmList[i][EI10] = I10;
		ElmList[i][EV11] = V11; ElmList[i][EI11] = I11;
		ElmList[i][EV12] = V12; ElmList[i][EI12] = I12;
		ElmList[i][EV13] = V13; ElmList[i][EI13] = I13;
		ElmList[i][EV14] = V14; ElmList[i][EI14] = I14;
		ElmList[i][EV15] = V15; ElmList[i][EI15] = I15;
		ElmList[i][EV16] = V16; ElmList[i][EI16] = I16;
		ElmList[i][EV17] = V17; ElmList[i][EI17] = I17;
	}
	void Tick_Lock8( uint16 i )
	{
		fix V1 = ElmList[i][EV1];
		
		fix V2 = ElmList[i][EV2];
		fix V3 = ElmList[i][EV3];
		fix V4 = ElmList[i][EV4];
		fix V5 = ElmList[i][EV5];
		fix V6 = ElmList[i][EV6];
		fix V7 = ElmList[i][EV7];
		fix V8 = ElmList[i][EV8];
		fix V9 = ElmList[i][EV9];
		
		fix x1 = ElmList[i][X1];
		fix x2 = ElmList[i][X2];
		
		bool Out1 = (x1 <= DC_h2 && V1 > DC_h2);
		if( Out1 ) {
			int Addr = 0;
			if( V2 > DC_h2 ) Addr |= 0x01;
			if( V3 > DC_h2 ) Addr |= 0x02;
			if( V4 > DC_h2 ) Addr |= 0x04;
			if( V5 > DC_h2 ) Addr |= 0x08;
			if( V6 > DC_h2 ) Addr |= 0x10;
			if( V7 > DC_h2 ) Addr |= 0x20;
			if( V8 > DC_h2 ) Addr |= 0x40;
			if( V9 > DC_h2 ) Addr |= 0x80;
			x2 = Addr + 0.5f;
		}
		ElmList[i][X1] = V1;
		ElmList[i][X2] = x2;
	}
	
	//---------------------------------------------------
	
	void Run_OSC( uint16 i )
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		
		fix V1S = ElmList[i][X2];
		
		bool Out2 = V1S > DC_h2;
		if( Out2 ) {
			I1 = DC - V1;
			V1 = DC;
		}
		else {
			I1 = -V1;
			V1 = 0.0f;
		}
		ElmList[i][EV1] = V1; ElmList[i][EI1] = I1;
	}
	void Tick_OSC( uint16 i )
	{
		fix x1 = ElmList[i][X1]; //频率
		fix x2 = ElmList[i][X2]; //当前输出值
		fix x3 = ElmList[i][X3]; //当前计数值
		if( x1 > 50 ) x1 = 50;
		if( x1 < 0.001f ) x1 = 0.001f;
		fix nts = 50 / x1;
		x3 += 1;
		if( x3 >= nts ) {
			x3 -= nts;
			bool Out1 = (x2 <= DC_h2);
			if( Out1 ) {
				x2 = DC;
			}
			else {
				x2 = 0.0f;
			}
		}
		ElmList[i][X2] = x2;
		ElmList[i][X3] = x3;
	}
	
	void Run_ROM8X8( uint16 i )
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		fix V2 = ElmList[i][EV2]; fix I2 = ElmList[i][EI2];
		fix V3 = ElmList[i][EV3]; fix I3 = ElmList[i][EI3];
		fix V4 = ElmList[i][EV4]; fix I4 = ElmList[i][EI4];
		fix V5 = ElmList[i][EV5]; fix I5 = ElmList[i][EI5];
		fix V6 = ElmList[i][EV6]; fix I6 = ElmList[i][EI6];
		fix V7 = ElmList[i][EV7]; fix I7 = ElmList[i][EI7];
		fix V8 = ElmList[i][EV8]; fix I8 = ElmList[i][EI8];
		fix V9 = ElmList[i][EV9]; fix I9 = ElmList[i][EI9];
		fix V10 = ElmList[i][EV10]; fix I10 = ElmList[i][EI10];
		fix V11 = ElmList[i][EV11]; fix I11 = ElmList[i][EI11];
		fix V12 = ElmList[i][EV12]; fix I12 = ElmList[i][EI12];
		fix V13 = ElmList[i][EV13]; fix I13 = ElmList[i][EI13];
		fix V14 = ElmList[i][EV14]; fix I14 = ElmList[i][EI14];
		fix V15 = ElmList[i][EV15]; fix I15 = ElmList[i][EI15];
		fix V16 = ElmList[i][EV16]; fix I16 = ElmList[i][EI16];
		
		int Addr = 0;
		if( V1 > DC_h2 ) Addr |= 0x01;
		if( V2 > DC_h2 ) Addr |= 0x02;
		if( V3 > DC_h2 ) Addr |= 0x04;
		if( V4 > DC_h2 ) Addr |= 0x08;
		if( V5 > DC_h2 ) Addr |= 0x10;
		if( V6 > DC_h2 ) Addr |= 0x20;
		if( V7 > DC_h2 ) Addr |= 0x40;
		if( V8 > DC_h2 ) Addr |= 0x80;
		
		int xx2 = ROM[Addr];
		
		if( (xx2 & 0x01) != 0 ) {
			I9 = DC - V9;
			V9 = DC;
		}
		else {
			I9 = -V9;
			V9 = 0.0f;
		}
		if( (xx2 & 0x02) != 0 ) {
			I10 = DC - V10;
			V10 = DC;
		}
		else {
			I10 = -V10;
			V10 = 0.0f;
		}
		if( (xx2 & 0x04) != 0 ) {
			I11 = DC - V11;
			V11 = DC;
		}
		else {
			I11 = -V11;
			V11 = 0.0f;
		}
		if( (xx2 & 0x08) != 0 ) {
			I12 = DC - V12;
			V12 = DC;
		}
		else {
			I12 = -V12;
			V12 = 0.0f;
		}
		if( (xx2 & 0x10) != 0 ) {
			I13 = DC - V13;
			V13 = DC;
		}
		else {
			I13 = -V13;
			V13 = 0.0f;
		}
		if( (xx2 & 0x20) != 0 ) {
			I14 = DC - V14;
			V14 = DC;
		}
		else {
			I14 = -V14;
			V14 = 0.0f;
		}
		if( (xx2 & 0x40) != 0 ) {
			I15 = DC - V15;
			V15 = DC;
		}
		else {
			I15 = -V15;
			V15 = 0.0f;
		}
		if( (xx2 & 0x80) != 0 ) {
			I16 = DC - V16;
			V16 = DC;
		}
		else {
			I16 = -V16;
			V16 = 0.0f;
		}
		ElmList[i][EV9] = V9; ElmList[i][EI9] = I9;
		ElmList[i][EV10] = V10; ElmList[i][EI10] = I10;
		ElmList[i][EV11] = V11; ElmList[i][EI11] = I11;
		ElmList[i][EV12] = V12; ElmList[i][EI12] = I12;
		ElmList[i][EV13] = V13; ElmList[i][EI13] = I13;
		ElmList[i][EV14] = V14; ElmList[i][EI14] = I14;
		ElmList[i][EV15] = V15; ElmList[i][EI15] = I15;
		ElmList[i][EV16] = V16; ElmList[i][EI16] = I16;
	}
	
	void Run_CMP( uint16 i )
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		fix V2 = ElmList[i][EV2]; fix I2 = ElmList[i][EI2];
		fix V3 = ElmList[i][EV3]; fix I3 = ElmList[i][EI3];
		fix V4 = ElmList[i][EV4]; fix I4 = ElmList[i][EI4];
		
		fix V5 = ElmList[i][EV5]; fix I5 = ElmList[i][EI5];
		fix V6 = ElmList[i][EV6]; fix I6 = ElmList[i][EI6];
		fix V7 = ElmList[i][EV7]; fix I7 = ElmList[i][EI7];
		fix V8 = ElmList[i][EV8]; fix I8 = ElmList[i][EI8];
		
		fix V9 = ElmList[i][EV9]; fix I9 = ElmList[i][EI9];
		
		int data1 = 0;
		if( V1 > DC_h2 ) data1 |= 0x01;
		if( V2 > DC_h2 ) data1 |= 0x02;
		if( V3 > DC_h2 ) data1 |= 0x04;
		if( V4 > DC_h2 ) data1 |= 0x08;
		
		int data2 = 0;
		if( V5 > DC_h2 ) data2 |= 0x01;
		if( V6 > DC_h2 ) data2 |= 0x02;
		if( V7 > DC_h2 ) data2 |= 0x04;
		if( V8 > DC_h2 ) data2 |= 0x08;
		
		if( data1 > data2 ) {
			I9 = DC - V9;
			V9 = DC;
		}
		else {
			I9 = -V9;
			V9 = 0.0f;
		}
		ElmList[i][EV9] = V9; ElmList[i][EI9] = I9;
	}
	
	void Run_DCD38( uint16 i )
	{
		fix V1 = ElmList[i][EV1]; fix I1 = ElmList[i][EI1];
		fix V2 = ElmList[i][EV2]; fix I2 = ElmList[i][EI2];
		fix V3 = ElmList[i][EV3]; fix I3 = ElmList[i][EI3];
		
		fix V4 = ElmList[i][EV4]; fix I4 = ElmList[i][EI4];
		fix V5 = ElmList[i][EV5]; fix I5 = ElmList[i][EI5];
		fix V6 = ElmList[i][EV6]; fix I6 = ElmList[i][EI6];
		fix V7 = ElmList[i][EV7]; fix I7 = ElmList[i][EI7];
		fix V8 = ElmList[i][EV8]; fix I8 = ElmList[i][EI8];
		fix V9 = ElmList[i][EV9]; fix I9 = ElmList[i][EI9];
		fix V10 = ElmList[i][EV10]; fix I10 = ElmList[i][EI10];
		fix V11 = ElmList[i][EV11]; fix I11 = ElmList[i][EI11];
		
		int Addr = 0;
		if( V1 > DC_h2 ) Addr |= 0x01;
		if( V2 > DC_h2 ) Addr |= 0x02;
		if( V3 > DC_h2 ) Addr |= 0x04;
		
		int xx2 = 0;
		switch( Addr ) {
			case 0: xx2 = 0x01; break;
			case 1: xx2 = 0x02; break;
			case 2: xx2 = 0x04; break;
			case 3: xx2 = 0x08; break;
			case 4: xx2 = 0x10; break;
			case 5: xx2 = 0x20; break;
			case 6: xx2 = 0x40; break;
			case 7: xx2 = 0x80; break;
			default: break;
		}
		
		if( (xx2 & 0x01) != 0 ) {
			I4 = DC - V4;
			V4 = DC;
		}
		else {
			I4 = -V4;
			V4 = 0.0f;
		}
		if( (xx2 & 0x02) != 0 ) {
			I5 = DC - V5;
			V5 = DC;
		}
		else {
			I5 = -V5;
			V5 = 0.0f;
		}
		if( (xx2 & 0x04) != 0 ) {
			I6 = DC - V6;
			V6 = DC;
		}
		else {
			I6 = -V6;
			V6 = 0.0f;
		}
		if( (xx2 & 0x08) != 0 ) {
			I7 = DC - V7;
			V7 = DC;
		}
		else {
			I7 = -V7;
			V7 = 0.0f;
		}
		if( (xx2 & 0x10) != 0 ) {
			I8 = DC - V8;
			V8 = DC;
		}
		else {
			I8 = -V8;
			V8 = 0.0f;
		}
		if( (xx2 & 0x20) != 0 ) {
			I9 = DC - V9;
			V9 = DC;
		}
		else {
			I9 = -V9;
			V9 = 0.0f;
		}
		if( (xx2 & 0x40) != 0 ) {
			I10 = DC - V10;
			V10 = DC;
		}
		else {
			I10 = -V10;
			V10 = 0.0f;
		}
		if( (xx2 & 0x80) != 0 ) {
			I11 = DC - V11;
			V11 = DC;
		}
		else {
			I11 = -V11;
			V11 = 0.0f;
		}
		ElmList[i][EV4] = V4; ElmList[i][EI4] = I4;
		ElmList[i][EV5] = V5; ElmList[i][EI5] = I5;
		ElmList[i][EV6] = V6; ElmList[i][EI6] = I6;
		ElmList[i][EV7] = V7; ElmList[i][EI7] = I7;
		ElmList[i][EV8] = V8; ElmList[i][EI8] = I8;
		ElmList[i][EV9] = V9; ElmList[i][EI9] = I9;
		ElmList[i][EV10] = V10; ElmList[i][EI10] = I10;
		ElmList[i][EV11] = V11; ElmList[i][EI11] = I11;
	}

//============================================================

//初始化
void remo_CC_Init( void )
{
	
}

//设置
void remo_CC_Reset( uint32 buf1, uint32 buf2, uint16 n, uint32 bufROM )
{
	StartTime = 0;

	ELength = n;
	VI_Addr = buf1;
	NL_Addr = buf2 % 65536;
	bufROM = bufROM % 65536;
	//?? NL ??
	for( int32 i = 0; i < 1024; ++i ) {
		NodeList[i] = Flash_ReadByte(NL_Addr + i);
	}
	for( int32 i = 0; i < 256; ++i ) {
		ROM[i] = Flash_ReadByte(bufROM + i);
	}
}

//处理 
void remo_CC_Deal( void )
{
	//?? UI ??
	int32 vnunber = VNumber;
	if( StartTime < MaxStartTime/2 ) {
		vnunber = ECVI; //????????????????, ???????????
	}
	for( int32 i = 0; i < ELength; ++i ) {
		for( int32 j = 0; j < vnunber; ++j ) {
			int32 idx = VI_Addr + i*VNumber*4 + j*4;
			int32 d = Mem_Get_int32( idx );
			ElmList[i][j] = d / FixScale;
		}
	}
	//????
	//Run();Run();Run();Run();Run();
	//Run();Run();Run();Run();Run();
	Run();
	
	//??VI??
	for( int32 i = 0; i < ELength; ++i ) {
		for( int j = 0; j < VNumber; ++j ) {
			fix d = ElmList[i][j];
			int32 idx = VI_Addr + i*VNumber*4 + j*4;
			Mem_Set_int32( idx, (int32)(d*FixScale) );
		}
	}
	if( StartTime > 0 ) {
		StartTime--;
	}
}




