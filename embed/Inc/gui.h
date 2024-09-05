#ifndef __GUI_H
#define __GUI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gui_commons.h"
#include "button.h"
#include "spin.h"
#include "stepper.h"
#include "selector.h"
#include "serial_comm.h"

enum {
	HOME_PAGE,
	MANUAL_PAGE,
	AUTO_PAGE
};

typedef struct {
	void*   widget;
	uint8_t type;
}widget_t;

typedef struct {
	widget_t widgets[MAX_WIDGETS];
	uint8_t  size;
}layout_t;

void init_screen(void);
void draw_screen(selector_t*, uint8_t);
int  get_page(void);
uint8_t get_connected(void);
void set_connected(uint8_t);
void draw_connected(void);
void draw_bussy(void);
void draw_unbussy(uint8_t);


#ifdef __cplusplus
}
#endif

#endif /* __GUI_H */
