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

#ifndef _remo_ST7920_h_
#define _remo_ST7920_h_

#include "remo_typedef.h"


void remo_ST7920_SetPin( uint8 rs, uint8 rw, uint8 e );
void remo_ST7920_SetValue( uint8 v );
void remo_ST7920_Init( void );
void remo_ST7920_write_command( uint8 C );
void remo_ST7920_write_data( uint8 D );


#endif
