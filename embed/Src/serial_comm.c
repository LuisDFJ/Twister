#include "serial_comm.h"

uint16_t pending_steps;
uint8_t dir_steps;
int32_t auto_position;

void __motor_step( uint8_t dir )
{
	HAL_GPIO_TogglePin( LED_GPIO_Port, LED_Pin );
	if ( dir ) HAL_GPIO_WritePin( DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
	else       HAL_GPIO_WritePin( DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET);
	// 1/16 Microstepping
	for (uint8_t i = 0; i < 16; i++){
		HAL_GPIO_WritePin( STEP_GPIO_Port, STEP_Pin, GPIO_PIN_SET );
		HAL_GPIO_WritePin( LED_GPIO_Port, LED_Pin, GPIO_PIN_SET );
		HAL_Delay(1);
		HAL_GPIO_WritePin( STEP_GPIO_Port, STEP_Pin, GPIO_PIN_RESET );
		HAL_GPIO_WritePin( LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET );
	}
	if (dir_steps)
		auto_position = auto_position + 1;
	else
		auto_position = auto_position - 1;
}

void __init_serial_com( void )
{
	pending_steps = 0;
	dir_steps = 1;
	auto_position = 0;
}


uint8_t __validate_checksum( uint8_t *buffer, uint8_t len )
{
	uint16_t checksum = *((uint16_t*)(&(buffer[len-2])));
	uint16_t cs = 0;
	for ( uint8_t i = 0; i < len - 3; i++ ) cs += buffer[i];
	return checksum == cs;
}

void __move_commands( uint8_t *buffer, uint8_t len )
{
	uint16_t steps = *((uint16_t*)(&(buffer[len-2])));
	uint8_t dir;
	switch( buffer[1] ){
		case '0':
			dir = 1;
		break;
		case '1':
			dir = 0;
		break;
	}
	if ( pending_steps > 0 ){
		char msg[] = "FAIL\n";
		CDC_Transmit_FS( (uint8_t*)msg, strlen(msg) );
	} else {
		pending_steps = steps;
		dir_steps = dir;
		char msg[] = "OK\n";
		CDC_Transmit_FS( (uint8_t*)msg, strlen(msg) );
	}
}

void __setup_commands( uint8_t *buffer, uint8_t len )
{
	char msg[] = "OK\nDONE\n";
	if( buffer[1] == '0' ){
		//ST7735_FillScreen(BLACK);
		auto_position = 0;
	} else if ( buffer[1] == '1'){
		CDC_Transmit_FS( (uint8_t*)(&auto_position), 4 );
	}
	HAL_Delay(1);
	CDC_Transmit_FS( (uint8_t*)msg, strlen(msg) );
	return;
}

/*
 * M0 II*CS // Move II steps in positive direction.
 * M1 II*CS // Move II steps in negative direction.
 * P0 *CS   // Zero current position.
 * P1 *CS	// Send current position.
 */
void comm_manager( uint8_t *buffer, uint8_t len )
{
	//char msg[BUFFER_SIZE_COMM];
	switch( buffer[0] ){
		case 'M':
			len = 8;
		break;
		case 'P':
			len = 6;
		break;
	}
	if( __validate_checksum( buffer, len ) ){
		switch( buffer[0] ){
			case 'M':
				__move_commands(buffer, len-3);
			break;
			case 'P':
				__setup_commands(buffer, len-3);
			break;
		}
	} else {
		char msg_fail[] = "FAIL\n";
		CDC_Transmit_FS( (uint8_t*)msg_fail, strlen(msg_fail) );
	}
}

uint8_t command_manager( void )
{
	if ( pending_steps > 0 ){
		__motor_step(dir_steps);
		pending_steps -= 1;
		if( pending_steps == 0 ){
			char msg[] = "DONE\n";
			CDC_Transmit_FS( (uint8_t*)msg, strlen(msg) );
		}
		return true;
	}
	return false;
}

int get_position( void )
{
	return auto_position;
}

void set_position( int pos )
{
	auto_position = pos;
}
