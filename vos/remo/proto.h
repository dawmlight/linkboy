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

#ifndef _proto_h_
#define _proto_h_

#include "remo_typedef.h"

extern bool Running;
extern bool Write4Byte;

void setup( void );
void loop( void );
//int16 Proto_GetData( uint8 id );
void Deal( uint8 d );
void Error( uint32 ET, uint32 EC );
void Debug( uint32 data );

#endif



