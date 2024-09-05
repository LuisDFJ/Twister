#include "gui.h"

uint8_t n;

layout_t L1;
button_t b1;
button_t b2;

spin_t s1;
stepper_t motor;

uint8_t page;
uint8_t connected;

void init_screen(void) {
	ST7735_Init(3);
	ST7735_FillScreen(BLACK);
	__init_button( &b1, 20, 15, 120, 45, "MANUAL" );
	__init_button( &b2, 20, 75, 120, 45, "AUTO" );
	__init_spin( &s1, 75, 60, 40,50);
	__init_stepper( &motor );
	L1.widgets[0] = (widget_t){ (void*)&b1, TYPE_BUTTON };
	L1.widgets[1] = (widget_t){ (void*)&b2, TYPE_BUTTON };
	L1.size = 2;
	n = 0;
	page = HOME_PAGE;
	connected = false;
}

void _draw_layout( widget_t *widgets, uint8_t N ){
	for ( int i = 0; i < N; i++){
		switch( widgets[i].type ){
			case TYPE_BUTTON: ;
				button_t *b = (button_t*) ( widgets[i].widget );
				if( i == n ) b->select(b);
				else b->unselect(b);
				b->draw(b);
			break;
		}
	}
}

void __return_home_page( void ){
	page = HOME_PAGE;
	s1.value = 0;
	set_position(0);
	n = 0;
	connected = false;
}

void draw_screen( selector_t *sel, uint8_t event ) {
	/*
	if ( sel->state == STATE_UP )
	{
		s1.value = s1.value + 1;
		HAL_GPIO_WritePin( DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
		HAL_GPIO_TogglePin( STEP_GPIO_Port, STEP_Pin );
	}
	else if ( sel->state == STATE_DOWN )
	{
		s1.value = s1.value - 1;
		HAL_GPIO_WritePin( DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET);
		HAL_GPIO_TogglePin( STEP_GPIO_Port, STEP_Pin );
	}

	*/
	//motor.move(&s1);


	if( event ){
		ST7735_FillScreen(BLACK);
		switch( page ){
		case HOME_PAGE:
			if( n==0 ) page = MANUAL_PAGE;
			if( n==1 ) {
				page = AUTO_PAGE;
				ST7735_WriteString( 10, 10, "Serial Com Mode", Font_7x10, WHITE, BLACK );
			}
		break;
		case MANUAL_PAGE: __return_home_page(); break;
		case AUTO_PAGE:   __return_home_page(); break;
	} }


	switch( page )
	{
		case HOME_PAGE:
			switch( sel->state ){
				case STATE_UP:
					if( n<(L1.size-1) ){
						n = n + 1;
						_draw_layout(L1.widgets, L1.size );
					}
				break;
				case STATE_DOWN:
					if( n>0 ){
						n = n - 1;
						_draw_layout(L1.widgets, L1.size );
					}
				break;
			}
		break;

		case MANUAL_PAGE:
			if ( sel->state == STATE_UP )
			{
				s1.value = s1.value + 1;
				__motor_step(1);
				//HAL_GPIO_WritePin( DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
				//HAL_GPIO_TogglePin( STEP_GPIO_Port, STEP_Pin );
			}
			else if ( sel->state == STATE_DOWN )
			{
				s1.value = s1.value - 1;
				__motor_step(0);
				//HAL_GPIO_WritePin( DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET);
				//HAL_GPIO_TogglePin( STEP_GPIO_Port, STEP_Pin );
			}
			s1.draw(&s1);
		break;

		case AUTO_PAGE:
			//s1.value = get_position();
			//s1.draw(&s1);
		break;
	}
}

int get_page( void )
{
	return page;
}

uint8_t get_connected( void ){
	return connected;
}

void set_connected( uint8_t v ){
	connected = v;
}

void draw_connected( void ){
	ST7735_DrawCircle(10,28,3,RED);
	ST7735_WriteString(15,25,"Connected",Font_7x10, WHITE, BLACK);
}

void draw_bussy( void ){
	ST7735_DrawCircle(10,43,3,BLUE);
	ST7735_WriteString(15,40,"Bussy",Font_7x10, WHITE, BLACK);
}

void draw_unbussy( uint8_t f ){
	ST7735_DrawCircle(10,43,3, f ? BLUE : BLACK );
}