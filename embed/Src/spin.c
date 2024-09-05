#include "spin.h"

int16_t prev_value;

void __init_spin( spin_t *self,uint16_t x,uint16_t y,uint16_t ri, uint16_t re)
{
	self->center.x = (int) x - 30 > 0 ? (uint16_t)( (int) x - 30 ) : 0;;
	self->center.y = y;
	self->value  = 0;
	self->draw   = __draw_spin;
	for( int i = 0; i < MAX_POINTS; i++ ){
		float angle = 2 * i * PI / MAX_POINTS;

		float px = -ri * sin( angle ) + x; float py =  ri * cos( angle ) + y;
		point_t p = { (uint16_t)px, (uint16_t)py };
		self->segments[i][0] = p;

		px = -re * sin( angle ) + x; py =  re * cos( angle ) + y;
		self->segments[i][1].x = (uint16_t)px;
		self->segments[i][1].y = (uint16_t)py;


		angle = 2 * (i+1) * PI / MAX_POINTS;

		px = -ri * sin( angle ) + x; py =  ri * cos( angle ) + y;
		self->segments[i][2].x = (uint16_t)px;
		self->segments[i][2].y = (uint16_t)py;

		px = -re * sin( angle ) + x; py =  re * cos( angle ) + y;
		self->segments[i][3].x = (uint16_t)px;
		self->segments[i][3].y = (uint16_t)py;
	}
	prev_value = 0;
}

void __draw_segment( point_t s[4], uint16_t color )
{
	ST7735_FillTriangle(s[0].x, s[0].y, s[1].x, s[1].y, s[3].x, s[3].y, color);
	ST7735_FillTriangle(s[2].x, s[2].y, s[3].x, s[3].y, s[0].x, s[0].y, color);
}

void __draw_positive_values( spin_t *self )
{
	uint16_t i = self->value % MAX_POINTS;
	if( i == 0 && self->value != 0 ){
		for( int j = 1; j < MAX_POINTS; j++ ){
			__draw_segment( self->segments[j], BLACK );
		}
	} else if( prev_value == 0 && self->value % MAX_POINTS == MAX_POINTS - 1 ) {
		for( int j = 0; j < MAX_POINTS - 1; j++ ){
			__draw_segment( self->segments[j], WHITE );
		}
	} else {
		if ( i > prev_value ) {
			__draw_segment( self->segments[i], WHITE );
		} else {
			i = i+1 < MAX_POINTS ? i + 1 : i;
			__draw_segment( self->segments[i], BLACK );
		}
	}

	prev_value = self->value % ( MAX_POINTS );
}

void __draw_negative_values( spin_t *self )
{
	uint16_t i = (-self->value) % MAX_POINTS;
	if( i == 0 && self->value != 0 ){
		for( int j = 1; j < MAX_POINTS; j++ ){
			__draw_segment( self->segments[MAX_POINTS-1-j], BLACK );
		}
	} else if( prev_value == 0 && (-self->value) % MAX_POINTS == MAX_POINTS - 1 ) {
		for( int j = 0; j < MAX_POINTS - 1; j++ ){
			__draw_segment( self->segments[MAX_POINTS-1-j], WHITE );
		}
	}else {
		if ( i > prev_value ) {
			__draw_segment( self->segments[MAX_POINTS-1-i], WHITE );
		} else {
			i = i+1 < MAX_POINTS ? i + 1 : i;
			__draw_segment( self->segments[MAX_POINTS-1-i], BLACK );
		}
	}

	prev_value = (-self->value) % MAX_POINTS;
}

void __draw_spin( spin_t *self )
{
	if( self->value > 0 ){
		__draw_positive_values(self);
	} else if( self->value < 0 ){
		__draw_negative_values(self);
	}

	int angle = self->value * 360 / MAX_POINTS ;
	char  l[7], lo[8];
	sprintf( l, "%i", angle );
	sprintf( lo, "%7s", l );
	ST7735_WriteString(self->center.x, self->center.y, lo, Font_7x10, WHITE, BLACK);
}
