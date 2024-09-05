#include "button.h"

void __init_button(button_t *self, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *text)
{
	point_t center = {x,y}; dim_t  size   = {w,h};
	point_t ptext  = {
			(uint16_t)((int)(x + w/2) - (int)(16*strlen(text)/2)),
			(uint16_t)((int)(y + h/2) - 13)
	};
	self->center   = center;
	self->size     = size;
	self->selected = false;
	strcpy(self->text,text);
	self->pText    = ptext;
	self->draw     = __draw_button;
	self->select   = __select_button;
	self->unselect = __unselect_button;
}
void __draw_button(button_t *self)
{
	// Draw Contour
	ST7735_DrawRect(
		self->center.x,
		self->center.y,
		self->size.w+2,
		self->size.h+2,
		0xFF00 );
	if(self->selected){
		ST7735_FillRect(
			self->center.x+1,
			self->center.y+1,
			self->size.w,
			self->size.h,
			RED );
		ST7735_WriteString(
			self->pText.x,
			self->pText.y,
			self->text,
			Font_16x26,
			WHITE,
			RED );
	} else {
		ST7735_FillRect(
			self->center.x+1,
			self->center.y+1,
			self->size.w,
			self->size.h,
			BLACK );
		ST7735_WriteString(
			self->pText.x,
			self->pText.y,
			self->text,
			Font_16x26,
			WHITE,
			BLACK );
	}
}
void __select_button(button_t *self)
{
	self->selected = true;
	self->draw(self);
}
void __unselect_button(button_t *self)
{
	self->selected = false;
	self->draw(self);
}
