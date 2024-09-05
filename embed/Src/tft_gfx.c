#include "tft_gfx.h"
#include "tft_st7735.h"

#include <stdlib.h>

extern int16_t _width;
extern int16_t _height;
extern int16_t cursor_x;
extern int16_t cursor_y;
extern uint8_t rotation;
extern uint8_t _colstart;
extern uint8_t _rowstart;
extern uint8_t _xstart;
extern uint8_t _ystart;

#define _swap_int16_t(a, b)\
	{ \
    int16_t t = a; \
    a = b; \
    b = t; \
	}
#define min(a, b) (((a) < (b)) ? (a) : (b))

void ST7735_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	ST7735_FillRectangle(x, y, w, h, color);
}

void ST7735_WriteLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if(steep){
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }
    if(x0 > x1){
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if(y0 < y1){
        ystep = 1;
    }else{
        ystep = -1;
    }
    for(; x0<=x1; x0++){
        if(steep){
        	ST7735_DrawPixel(y0, x0, color);
        }else{
        	ST7735_DrawPixel(x0, y0, color);
        }
        err -= dy;
        if(err < 0){
            y0 += ystep;
            err += dx;
        }
    }
}

void ST7735_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	ST7735_WriteLine(x, y, x, y + h - 1, color);
}

void ST7735_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	ST7735_WriteLine(x, y, x + w - 1, y, color);
}

void ST7735_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    if(x0 == x1){
        if(y0 > y1) _swap_int16_t(y0, y1);
        ST7735_DrawFastVLine(x0, y0, y1 - y0 + 1, color);
    }else if(y0 == y1){
    	if(x0 > x1) _swap_int16_t(x0, x1);
        ST7735_DrawFastHLine(x0, y0, x1 - x0 + 1, color);
    }else{
    	ST7735_WriteLine(x0, y0, x1, y1, color);
    }
}

void ST7735_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    ST7735_DrawPixel(x0  , y0+r, color);
    ST7735_DrawPixel(x0  , y0-r, color);
    ST7735_DrawPixel(x0+r, y0  , color);
    ST7735_DrawPixel(x0-r, y0  , color);

    while(x < y){
        if(f >= 0){
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        ST7735_DrawPixel(x0 + x, y0 + y, color);
        ST7735_DrawPixel(x0 - x, y0 + y, color);
        ST7735_DrawPixel(x0 + x, y0 - y, color);
        ST7735_DrawPixel(x0 - x, y0 - y, color);
        ST7735_DrawPixel(x0 + y, y0 + x, color);
        ST7735_DrawPixel(x0 - y, y0 + x, color);
        ST7735_DrawPixel(x0 + y, y0 - x, color);
        ST7735_DrawPixel(x0 - y, y0 - x, color);
    }
}

void ST7735_DrawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while(x < y){
        if(f >= 0){
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if(cornername & 0x04){
        	ST7735_DrawPixel(x0 + x, y0 + y, color);
        	ST7735_DrawPixel(x0 + y, y0 + x, color);
        }
        if(cornername & 0x02){
        	ST7735_DrawPixel(x0 + x, y0 - y, color);
        	ST7735_DrawPixel(x0 + y, y0 - x, color);
        }
        if(cornername & 0x08){
        	ST7735_DrawPixel(x0 - y, y0 + x, color);
        	ST7735_DrawPixel(x0 - x, y0 + y, color);
        }
        if(cornername & 0x01){
        	ST7735_DrawPixel(x0 - y, y0 - x, color);
        	ST7735_DrawPixel(x0 - x, y0 - y, color);
        }
    }
}

void ST7735_FillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    int16_t px = x;
    int16_t py= y;

    delta++;
    while(x < y){
        if (f >= 0){
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if(x < (y + 1)){
            if(corners & 1) ST7735_DrawFastVLine(x0+x, y0-y, 2*y+delta, color);
            if(corners & 2) ST7735_DrawFastVLine(x0-x, y0-y, 2*y+delta, color);
        }
        if(y != py) {
            if(corners & 1) ST7735_DrawFastVLine(x0+py, y0-px, 2*px+delta, color);
            if(corners & 2) ST7735_DrawFastVLine(x0-py, y0-px, 2*px+delta, color);
            py = y;
        }
        px = x;
    }
}

void ST7735_FillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    ST7735_DrawFastVLine(x0, y0-r, 2*r+1, color);
    ST7735_FillCircleHelper(x0, y0, r, 3, 0, color);
}

void ST7735_DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	ST7735_DrawFastHLine(x, y, w, color);
	ST7735_DrawFastHLine(x, y+h-1, w, color);
    ST7735_DrawFastVLine(x, y, h, color);
    ST7735_DrawFastVLine(x+w-1, y, h, color);
}

void ST7735_DrawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
    int16_t max_radius = ((w < h) ? w : h) / 2;
    if(r > max_radius) r = max_radius;

    ST7735_DrawFastHLine(x+r, y, w-2*r, color);
    ST7735_DrawFastHLine(x+r, y+h-1, w-2*r, color);
    ST7735_DrawFastVLine(x, y+r, h-2*r, color);
    ST7735_DrawFastVLine(x+w-1, y+r, h-2*r, color);

    ST7735_DrawCircleHelper(x+r, y+r, r, 1, color);
    ST7735_DrawCircleHelper(x+w-r-1, y+r, r, 2, color);
    ST7735_DrawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
    ST7735_DrawCircleHelper(x+r, y+h-r-1, r, 8, color);
}

void ST7735_FillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
    int16_t max_radius = ((w < h) ? w : h) / 2;
    if(r > max_radius) r = max_radius;

    ST7735_FillRect(x+r, y, w-2*r, h, color);
    ST7735_FillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
    ST7735_FillCircleHelper(x+r, y+r, r, 2, h-2*r-1, color);
}

void ST7735_DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	ST7735_DrawLine(x0, y0, x1, y1, color);
	ST7735_DrawLine(x1, y1, x2, y2, color);
    ST7735_DrawLine(x2, y2, x0, y0, color);
}

void ST7735_FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	int16_t a, b, y, last;

    if(y0 > y1){
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }
    if(y1 > y2){
        _swap_int16_t(y2, y1); _swap_int16_t(x2, x1);
    }
    if(y0 > y1){
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }
    if(y0 == y2){
    	a = b = x0;
        if(x1 < a)      a = x1;
        else if(x1 > b) b = x1;
        if(x2 < a)      a = x2;
        else if(x2 > b) b = x2;
        ST7735_DrawFastHLine(a, y0, b-a+1, color);
        return;
    }

    int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
    int32_t
    sa = 0,
    sb = 0;

    if(y1 == y2) last = y1;
    else last = y1-1;

    for(y=y0; y<=last; y++){
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        if(a > b) _swap_int16_t(a,b);
        ST7735_DrawFastHLine(a, y, b-a+1, color);
    }

    sa = (int32_t)dx12 * (y - y1);
    sb = (int32_t)dx02 * (y - y0);
    for(; y<=y2; y++) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        if(a > b) _swap_int16_t(a,b);
        ST7735_DrawFastHLine(a, y, b-a+1, color);
    }
}

void ST7735_FillScreen(uint16_t color)
{
	ST7735_FillRect(0, 0, _width, _height, color);
}
