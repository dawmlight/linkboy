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

#ifndef _remo_ST7789_h_
#define _remo_ST7789_h_

#include "remo_typedef.h"


void remo_ST7789_SetPin( uint8 vDIN, uint8 vCLK, uint8 vRES, uint8 vDC );
void remo_ST7789_Init( int32 w, int32 h );
void remo_ST7789_LCD_draw_pixel(int32 x, int32 y);
void remo_ST7789_LCD_clear_pixel(int32 x, int32 y);
void remo_ST7789_LCD_Clear(uint16 Color);

void remo_ST7789_LCD_set_fore(uint16 Color, bool t);
void remo_ST7789_LCD_set_back(uint16 Color, bool t);

#endif
