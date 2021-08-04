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

#ifndef _remo_CC_h_
#define _remo_CC_h_

#include "remo_typedef.h"

void remo_CC_Init( void );
void remo_CC_Reset( uint32 addr1, uint32 addr2, uint16 n, uint32 addr3 );
void remo_CC_Deal( void );


#endif
