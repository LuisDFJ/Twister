#include "stepper.h"

void __init_stepper(stepper_t*self)
{
	HAL_GPIO_WritePin( MS1_GPIO_Port, MS1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin( MS2_GPIO_Port, MS2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin( MS3_GPIO_Port, MS3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin( DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin( STEP_GPIO_Port, STEP_Pin, GPIO_PIN_RESET);
}

void __move_stepper(spin_t*spin)
{
	if( spin->value > 0 ){
		HAL_GPIO_WritePin( DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
		HAL_GPIO_TogglePin( STEP_GPIO_Port, STEP_Pin );

		//HAL_GPIO_WritePin( STEP_GPIO_Port, STEP_Pin, GPIO_PIN_RESET);
	} else if( spin->value < 0 ){
		HAL_GPIO_WritePin( DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET);
		HAL_GPIO_TogglePin( STEP_GPIO_Port, STEP_Pin );
		//HAL_GPIO_WritePin( STEP_GPIO_Port, STEP_Pin, GPIO_PIN_SET);
		//HAL_Delay(5);
		//HAL_GPIO_WritePin( STEP_GPIO_Port, STEP_Pin, GPIO_PIN_RESET);
	}
}
