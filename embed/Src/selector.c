#include "selector.h"

selector_t SEL;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(htim == &htim3){
		if( SEL.enabled ){
			SEL.unenable(&SEL);
			SEL.move(&SEL);
			if ( SEL.state != STATE_IDLE ) HAL_GPIO_TogglePin( LED_GPIO_Port, LED_Pin );
			if ( HAL_GPIO_ReadPin( SW_GPIO_Port, SW_Pin) ) SEL.select(&SEL);
		}
	}
}

void init_selector(void)
{
	__init_selector( &SEL );
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Base_Start_IT(&htim3);
	TIM2->CNT = 0x7FFF;
}

void __init_selector(selector_t *self)
{
	self->pos   = 0;
	self->state = STATE_IDLE;
	self->selected = SW_UP;
	self->select  = __select;
	self->release = __release;
	self->move    = __move;
	self->enable  = __enable;
	self->unenable  = __unenable;
	self->reset		= __reset;
	self->isChanged = __isChanged;
}

void __select(selector_t *self)
{
	self->selected = SW_UP;
}

void __release(selector_t *self)
{
	self->selected = SW_DOWN;
}

void __move(selector_t *self)
{
	self->pos = (int)((TIM2->CNT)) - 0x7FFF;
	TIM2->CNT = 0x7FFF;
	if( self->pos > 0 ) 	 self->state = STATE_UP;
	else if( self->pos < 0 ) self->state = STATE_DOWN;
	else 					 self->state = STATE_IDLE;
}

void __enable(selector_t *self)
{
	self->enabled = 1;
}

void __unenable(selector_t *self)
{
	self->enabled = 0;
}

void __reset(selector_t *self)
{
	self->state = STATE_IDLE;
	self->selected = SW_UP;
}

uint8_t __isChanged(selector_t *self)
{
	if( (self->state != STATE_IDLE) || (self->selected == SW_DOWN) ) return 1;
	return 0;
}
