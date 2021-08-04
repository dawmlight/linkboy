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

#ifndef _remo_SHT1106_I2C_h_
#define _remo_SHT1106_I2C_h_

#include "remo_typedef.h"


void remo_SHT1106_I2C_SetPin( uint8 SCL, uint8 SDA );
void remo_SHT1106_I2C_Init( void );
void remo_SHT1106_I2C_write_command( uint8 C );
void remo_SHT1106_I2C_write_data( uint8 D );


#endif
