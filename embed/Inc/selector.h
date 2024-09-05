#ifndef __SELECTOR_H
#define __SELECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

enum {
	STATE_IDLE,
	STATE_UP,
	STATE_DOWN
};

enum {
	SW_UP,
	SW_DOWN
};

typedef struct selector {
	int  pos;
	uint8_t  state;
	uint8_t  selected;
	uint8_t  enabled;
	void (*select)(struct selector*);
	void (*release)(struct selector*);
	void (*move)(struct selector*);
	void (*enable)(struct selector*);
	void (*unenable)(struct selector*);
	void (*reset)(struct selector*);
	uint8_t (*isChanged)(struct selector*);
}selector_t;

extern selector_t SEL;

void init_selector(void);
void __init_selector(selector_t*);
void __select(selector_t*);
void __release(selector_t*);
void __move(selector_t*);
void __enable(selector_t*);
void __unenable(selector_t*);
void __reset(selector_t*);
uint8_t __isChanged(selector_t*);

#ifdef __cplusplus
}
#endif

#endif /* __SELECTOR_H */
