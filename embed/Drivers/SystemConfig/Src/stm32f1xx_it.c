#include "main.h"
#include "stm32f1xx_it.h"

extern PCD_HandleTypeDef hpcd_USB_FS;
extern TIM_HandleTypeDef htim3;

void NMI_Handler(void)
{
  while (1) {}
}

void HardFault_Handler(void)
{
  while (1) {}
}

void MemManage_Handler(void)
{
  while (1) {}
}

void BusFault_Handler(void)
{
  while (1) {}
}

void UsageFault_Handler(void)
{
  while (1) {}
}


void SVC_Handler(void)
{

}

void DebugMon_Handler(void)
{

}

void PendSV_Handler(void)
{

}

void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
}

void TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim3);
}
