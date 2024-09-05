#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"


#include "gui.h"
#include "selector.h"
#include "serial_comm.h"
#include "persistent_flag.h"

void SystemClock_Config(void);

uint8_t buffer[BUFFER_SIZE_COMM];
void clear_buffer( void ) { memset (buffer, '\0', BUFFER_SIZE_COMM); }


int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USB_DEVICE_Init();
  
  // 1/16 Microstepping
  __init_serial_com(); clear_buffer();
  HAL_GPIO_WritePin(MS1_GPIO_Port, MS1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(MS2_GPIO_Port, MS2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(MS3_GPIO_Port, MS3_Pin, GPIO_PIN_SET);

  init_screen();
  init_selector();

  uint8_t event = false;
  uint8_t press = false;
  uint8_t non_repeat_flag = true;
  draw_screen( &SEL, false );



  while (1)
  {
  press = HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin);
	if ( press && non_repeat_flag == false ) { event = true; non_repeat_flag = true; }
	if ( !press ) non_repeat_flag = false;
	//HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, press);


  if( SEL.isChanged(&SEL) || event || get_page() == AUTO_PAGE ){
    draw_screen( &SEL, event );
    event = false;
    SEL.reset(&SEL);
  }
  SEL.enable(&SEL);

	uint8_t len = (uint8_t) strlen( (char *) buffer );
	if ( len && ( get_page() == AUTO_PAGE ) ){
		if ( !get_connected() ) {
      set_connected( true );
      draw_connected();
      draw_bussy();
    }
    comm_manager(buffer, len);
		clear_buffer();
	}
  
  draw_unbussy( command_manager() );
  //persistent_flag_t pflag = persistent_flag( command_manager() );
  //if ( pflag.sw ) draw_unbussy( pflag.f );
  
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {}
#endif /* USE_FULL_ASSERT */
