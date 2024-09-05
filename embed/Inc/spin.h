#ifndef __SPIN_H
#define __SPIN_H

#ifdef __cplusplus
extern "C" {
#endif



#include "gui_commons.h"
#include "math.h"

#define DOT_SIZE   2
#define NO_CIRCLES 2
#define MAX_POINTS 64
#define PI 3.1416

typedef struct spin {
	point_t  center;
	point_t  segments[MAX_POINTS][4];
	int16_t  value;
	void (*draw)(struct spin*);
}spin_t;

void __init_spin( spin_t*,uint16_t,uint16_t,uint16_t,uint16_t);
void __draw_spin( spin_t* );

#ifdef __cplusplus
}
#endif

#endif /* __SPIN_H */
