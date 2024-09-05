#ifndef __STEPPER_H
#define __STEPPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "spin.h"

typedef struct stepper{
	void (*move)(spin_t*);
}stepper_t;


void __init_stepper(stepper_t*);
void __move_stepper(spin_t*);


#ifdef __cplusplus
}
#endif


#endif /* __STEPPER_H */
