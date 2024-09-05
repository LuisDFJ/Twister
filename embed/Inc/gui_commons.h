#ifndef __GUI_COMMONS_H
#define __GUI_COMMONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tft_gfx.h"
#include "tft_st7735.h"
#include <stdio.h>
#include <string.h>

#define MAX_WIDGETS 4

enum{
	TYPE_NULL,
	TYPE_BUTTON,
};

typedef struct {
	uint16_t x;
	uint16_t y;
}point_t;

typedef struct {
	uint16_t w;
	uint16_t h;
}dim_t;

#ifdef __cplusplus
}
#endif

#endif /* __GUI_COMMONS_H */
