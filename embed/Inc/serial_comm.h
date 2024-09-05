#ifndef __SERIAL_COMM_H
#define __SERIAL_COMM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "usbd_cdc_if.h"
#include "gui_commons.h"
#include <string.h>
#define BUFFER_SIZE_COMM 64

void __init_serial_com(void);
void __motor_step(uint8_t);
void comm_manager(uint8_t*,uint8_t);
uint8_t command_manager(void);
int  get_position(void);
void set_position(int);



#ifdef __cplusplus
}
#endif

#endif /* __SERIAL_COMM_H */
