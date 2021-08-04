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

#define N_CLI 		220
#define N_SEI 		221

#define N_Interface  1	//接口调用
#define N_SetStack  2		//设置堆栈
#define N_Goto  3			//跳转指令
#define N_Call  4			//函数调用
#define N_Ret  5			//返回指令
#define N_Reti 	6			//中断返回指令
#define N_StackInc  7		//跳转指令
#define N_StackDec  8		//跳转指令

//#define N_TransXValue  9
#define N_SaveCurrent  10
#define N_LoadCurrent  11
#define N_Nop  12
#define N_Nop0  0

#define N_Pop 	13
#define N_Push 	14

#define N_MoveAtoB	 	225

#define N_SETA_uint8 	15			//设置A寄存器
#define N_SETA_uint16 	205		//设置A寄存器
#define N_SETA_uint32 	206		//设置A寄存器
#define N_SETA_sint8 	207		//设置A寄存器
#define N_SETA_sint16 	208		//设置A寄存器
#define N_SETA_sint32 	209		//设置A寄存器
#define N_SETA_fix 	209		//设置A寄存器

#define N_SETB_uint8 	226		
#define N_SETB_uint16 	227
#define N_SETB_uint32 	228
#define N_SETB_sint8 	229
#define N_SETB_sint16 	230
#define N_SETB_sint32 	231
#define N_SETB_fix 	232

#define N_Not_bool_bool 			19
#define N_And_bool_bool_bool 		20
#define N_Or_bool_bool_bool 		21

#define N_GotoDec_x 	50	//跳转递减

#define N_Load_PC_local_uint 	151
#define N_InsAddr 				152

#define N_ReadAddr_local_uint32 				16
#define N_AddrOffset_uintx_uintx_uintx 		17
#define N_StructOffset_uintx_uintx 			18

/*
#define N_LoadCODE_uint8 	77
#define N_LoadCODE_uint16 	78
#define N_LoadCODE_uint32 	80
*/
#define N_LoadBASE_uint8 	81
#define N_LoadBASE_uint16 	82
#define N_LoadBASE_uint32 	84
#define N_SaveBASE_uint8 	85
#define N_SaveBASE_uint16 	86
#define N_SaveBASE_uint32 	88

#define N_ReadSys_sint8 	63
#define N_ReadSys_sint16 	64
#define N_ReadSys_sint32 	66

#define N_AutoSwitch_int16_int8 		202		
#define N_AutoSwitch_int32_int8 		203		
#define N_AutoSwitch_int32_int16 		204
#define N_AutoSwitch_fix_int32 		222
#define N_AutoSwitch_int32_fix 		233

#define N_UserSwitch_sint8_uint8 				198
#define N_UserSwitch_sint16_uint16 			199
#define N_UserSwitch_sint32_uint32 			201

#define N_UserSwitch_uint8_sint8 				216
#define N_UserSwitch_uint16_sint16 			217
#define N_UserSwitch_uint32_sint32 			219

#define N_UserSwitch_uint8_uint16 				192
#define N_UserSwitch_uint8_uint32 				193
#define N_UserSwitch_uint16_uint32 			194
#define N_UserSwitch_sint8_sint16 				195
#define N_UserSwitch_sint8_sint32 				196
#define N_UserSwitch_sint16_sint32 			197

#define N_Neg_int8_int8 		22
#define N_Neg_int16_int16 		153
#define N_Neg_int32_int32 		155
#define N_Abs_x_x 				23

#define N_Add_uint8_uint8_uint8 		156
#define N_Add_uint16_uint16_uint16 	157
#define N_Add_uint32_uint32_uint32 	159
#define N_Add_int8_int8_int8 			160
#define N_Add_int16_int16_int16 		161
#define N_Add_int32_int32_int32 		163

#define N_Sub_uint8_uint8_uint8 		25
#define N_Sub_uint16_uint16_uint16 	164
#define N_Sub_uint32_uint32_uint32 	166
#define N_Sub_int8_int8_int8 			167
#define N_Sub_int16_int16_int16 		168
#define N_Sub_int32_int32_int32 		170

#define N_Mult_uint8_uint8_uint8 		26
#define N_Mult_uint16_uint16_uint16 	171
#define N_Mult_uint32_uint32_uint32 	173
#define N_Mult_int8_int8_int8 			174
#define N_Mult_int16_int16_int16 		175
#define N_Mult_int32_int32_int32 		177
#define N_Mult_fix_fix_fix 			223

#define N_Div_uint8_uint8_uint8 		27
#define N_Div_uint16_uint16_uint16 	178
#define N_Div_uint32_uint32_uint32 	180
#define N_Div_int8_int8_int8 			181
#define N_Div_int16_int16_int16 		182
#define N_Div_int32_int32_int32 		184
#define N_Div_fix_fix_fix 				224

#define N_Mod_uint8_uint8_uint8 		28
#define N_Mod_uint16_uint16_uint16 	185
#define N_Mod_uint32_uint32_uint32 	187
#define N_Mod_int8_int8_int8 			188
#define N_Mod_int16_int16_int16 		189
#define N_Mod_int32_int32_int32 		191

#define N_Large_bool_uint_uint 		29
#define N_Large_bool_sint_sint 		210
#define N_LargeEqual_bool_uint_uint 	30
#define N_LargeEqual_bool_sint_sint 	211
#define N_Small_bool_uint_uint 		31
#define N_Small_bool_sint_sint 		212
#define N_SmallEqual_bool_uint_uint 	32
#define N_SmallEqual_bool_sint_sint 	213
#define N_Equal_bool_uint_uint 		33
#define N_Equal_bool_sint_sint 		214
#define N_NotEqual_bool_uint_uint 		34
#define N_NotEqual_bool_sint_sint 		215

#define N_TrueGoto_bool 	48
#define N_FalseGoto_bool 	49

#define N_switchcmp_uint32_uint8 		234
#define N_switchgoto_uint32 			235

#define N_Com_bit_bit 					35
#define N_Com_uint8_uint8 				36
#define N_Com_uint16_uint16  			37
#define N_Com_uint24_uint24 			38
#define N_Com_uint32_uint32 			39
#define N_And_x_x_x 					40
#define N_Or_x_x_x 					41
#define N_Xor_x_x_x 					42
#define N_Left_uint8_uint8_uint8 		43
#define N_Left_uint16_uint16_uint8 	44
#define N_Left_uint24_uint24_uint8 	45
#define N_Left_uint32_uint32_uint8 	46
#define N_Right_x_x_x 					47

#define N_ReadSub_bit_uint8 				51
#define N_ReadSub_bit_uint16 				52
#define N_ReadSub_bit_uint32 				53
#define N_ReadSub_uint8_uint32 			54
#define N_ReadSub_uint8_uint16 			55
#define N_ReadSub_uint16_uint32 			56
#define N_WriteSub_uint8_uint8_bit 		57
#define N_WriteSub_uint16_uint16_bit 		58
#define N_WriteSub_uint32_uint32_bit 		59
#define N_WriteSub_uint16_uint16_uint8 	60
#define N_WriteSub_uint32_uint32_uint8 	61
#define N_WriteSub_uint32_uint32_uint16 	62

#define N_Load_0_static_bool 		89
#define N_Load_0_static_bit 		90
#define N_Load_0_static_uint8 		91
#define N_Load_0_static_uint16 	92
#define N_Load_0_static_uint32 	94
#define N_Load_0_static_int8 		95
#define N_Load_0_static_int16 		96
#define N_Load_0_static_int32 		98
#define N_Load_1_static_bool 		99
#define N_Load_1_static_bit 		100
#define N_Load_1_static_uint8 		101
#define N_Load_1_static_uint16 	102
#define N_Load_1_static_uint32 	104
#define N_Load_1_static_int8 		105
#define N_Load_1_static_int16 		106
#define N_Load_1_static_int32 		108
#define N_Save_0_static_bool 		109
#define N_Save_0_static_bit 		110
#define N_Save_0_static_uint8 		111
#define N_Save_0_static_uint16 	112
#define N_Save_0_static_uint32 	114
#define N_Save_0_static_int8 		115
#define N_Save_0_static_int16 		116
#define N_Save_0_static_int32 		118

#define N_Save_0_local_bool 		139
#define N_Save_0_local_bit 		140
#define N_Save_0_local_uint8 		141
#define N_Save_0_local_uint16 		142
#define N_Save_0_local_uint32 		144
#define N_Save_0_local_int8 		145
#define N_Save_0_local_int16 		146
#define N_Save_0_local_int32 		148

#define N_Load_SP_local_uint16 	149
#define N_Save_SP_local_uint16 	150

#define N_Load_0_local_bool 		119
#define N_Load_0_local_bit 		120
#define N_Load_0_local_uint8 		121
#define N_Load_0_local_uint16 		122
#define N_Load_0_local_uint32 		124
#define N_Load_0_local_int8 		125
#define N_Load_0_local_int16 		126
#define N_Load_0_local_int32 		128

#define N_Load_1_local_bool 		129
#define N_Load_1_local_bit 		130
#define N_Load_1_local_uint8 		131
#define N_Load_1_local_uint16 		132
#define N_Load_1_local_uint32 		134
#define N_Load_1_local_int8 		135
#define N_Load_1_local_int16 		136
#define N_Load_1_local_int32 		138

#define N_TransVar_bit 			67
#define N_TransVar_bool 			68
#define N_TransVar_uint8 			69
#define N_TransVar_uint16 			70
#define N_TransVar_uint32 			72
#define N_TransVar_int8 			73
#define N_TransVar_int16 			74
#define N_TransVar_int32 			76


