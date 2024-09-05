#ifndef __PERSISTENT_FLAG_H
#define __PERSISTENT_FLAG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gui_commons.h"

#define NPFlagt 50
#define NPFlagf 1

typedef struct
{
    uint8_t f;
    uint8_t sw;
} persistent_flag_t;


void __init_persistent_flag( void );
persistent_flag_t persistent_flag( uint8_t );



#ifdef __cplusplus
}
#endif

#endif /* __PERSISTENT_FLAG_H */
