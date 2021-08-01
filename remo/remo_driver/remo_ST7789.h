
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
