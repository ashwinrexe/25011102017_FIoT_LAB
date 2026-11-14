/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */
void configure_wifi_server(void);
void handle_dynamic_http_requests(void);
void send_at_directive(const char *cmd, uint32_t timeout);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
/* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
/* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
/* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
    configure_wifi_server();

/* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
/* USER CODE END WHILE */	
    /* USER CODE BEGIN 3 */
    handle_dynamic_http_requests();

/* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
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

  /** Initializes the CPU, AHB and APB buses clocks
  */
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
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */
/* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */
/* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
/* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */
/* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */
/* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */
/* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void send_at_directive(const char *cmd, uint32_t timeout) {
    char dbg[128];
    snprintf(dbg, sizeof(dbg), "\r\n[AT] %s", cmd);
    HAL_UART_Transmit(&huart3, (uint8_t *)dbg, strlen(dbg), HAL_MAX_DELAY);

    HAL_UART_Transmit(&huart1, (uint8_t *)cmd, strlen(cmd), HAL_MAX_DELAY);

    uint8_t byte;
    uint32_t start_tick = HAL_GetTick();
    uint32_t last_tick = HAL_GetTick();

    while ((HAL_GetTick() - start_tick) < timeout) {
        if (HAL_UART_Receive(&huart1, &byte, 1, 15) == HAL_OK) {
            HAL_UART_Transmit(&huart3, &byte, 1, 50);
            last_tick = HAL_GetTick();
        } else if ((HAL_GetTick() - last_tick) > 100 && last_tick != start_tick) {
            break;
        }
    }
}

void configure_wifi_server(void) {
    HAL_Delay(2000);
    send_at_directive("AT\r\n", 1000);
    HAL_Delay(500);

    send_at_directive("AT+CWMODE=1\r\n", 1200);
    HAL_Delay(500);

    send_at_directive("AT+CWJAP=\"SNUC\",\"snu12345\"\r\n", 9000);
    HAL_Delay(1000);

    send_at_directive("AT+CIPMUX=1\r\n", 1000);
    HAL_Delay(500);

    send_at_directive("AT+CIPSERVER=1,80\r\n", 1500);
    HAL_Delay(500);

    send_at_directive("AT+CIFSR\r\n", 2000);
}

void handle_dynamic_http_requests(void) {
    uint8_t byte_in;
    char rx_window[384];
    uint16_t w_idx = 0;

    while (HAL_UART_Receive(&huart1, &byte_in, 1, 50) == HAL_OK) {
        if (w_idx < sizeof(rx_window) - 1) {
            rx_window[w_idx++] = byte_in;
            rx_window[w_idx] = '\0';
        }

        if (strstr(rx_window, "+IPD") != NULL) {
            HAL_Delay(150); // Ingest incoming GET header

            char *ipd = strstr(rx_window, "+IPD,");
            int link_num = 0;
            if (ipd != NULL) {
                link_num = ipd[5] - '0';
            }

            // Parse request endpoints for hardware control
            if (strstr(rx_window, "GET /led/on") != NULL) {
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // Active-LOW ON
            } else if (strstr(rx_window, "GET /led/off") != NULL) {
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);   // Active-LOW OFF
            }

            GPIO_PinState pin_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
            const char *state_text = (pin_state == GPIO_PIN_RESET) ? "ON" : "OFF";

            char page_buf[512];
            snprintf(page_buf, sizeof(page_buf),
                     "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n"
                     "<!DOCTYPE html><html><body style='text-align:center;font-family:sans-serif;'>"
                     "<h2>STM32 REST API Control Console</h2>"
                     "<p>Actuator Status: <span style='font-size:20px;color:%s'><b>%s</b></span></p>"
                     "<a href='/led/on'><button style='padding:12px 24px;font-size:16px;'>Turn ON</button></a> "
                     "<a href='/led/off'><button style='padding:12px 24px;font-size:16px;'>Turn OFF</button></a>"
                     "</body></html>\r\n",
                     (pin_state == GPIO_PIN_RESET) ? "green" : "red",
                     state_text);

            char send_header[32];
            snprintf(send_header, sizeof(send_header), "AT+CIPSEND=%d,%d\r\n", link_num, (int)strlen(page_buf));
            send_at_directive(send_header, 1500);
            HAL_Delay(80);

            send_at_directive(page_buf, 2500);
            HAL_Delay(80);

            char close_hdr[32];
            snprintf(close_hdr, sizeof(close_hdr), "AT+CIPCLOSE=%d\r\n", link_num);
            send_at_directive(close_hdr, 1000);

            w_idx = 0;
            rx_window[0] = '\0';
            break;
        }
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
