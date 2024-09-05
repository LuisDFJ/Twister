#include "persistent_flag.h"

uint8_t past_flag;
int same_counter;

void __init_persistent_flag( void ){
    same_counter = 0;
    past_flag = true;
}

persistent_flag_t persistent_flag( uint8_t current_flag ){
    persistent_flag_t f;
    f.sw = false;
    if ( past_flag != current_flag ){
        same_counter++;
        if ( same_counter > ( current_flag ? NPFlagt : NPFlagf ) ){
            same_counter = 0;
            past_flag = current_flag;
            f.sw = true;
        }
    } else {
        same_counter = 0;
    }
    f.f = past_flag;
    return f;
}