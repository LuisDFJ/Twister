#ifndef __BUTTON_H
#define __BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gui_commons.h"

typedef struct button{
	point_t center;
	dim_t   size;
	uint8_t selected;
	point_t pText;
	char    text[10];
	void (*draw)(struct button*);
	void (*select)(struct button*);
	void (*unselect)(struct button*);
}button_t;


void __init_button(button_t*,uint16_t,uint16_t,uint16_t,uint16_t,const char*);
void __draw_button(button_t*);
void __select_button(button_t*);
void __unselect_button(button_t*);


#ifdef __cplusplus
}
#endif

#endif /* __BUTTON_H */
