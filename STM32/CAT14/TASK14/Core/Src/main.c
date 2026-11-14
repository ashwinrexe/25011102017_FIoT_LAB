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
#include "bme280.h"
#include "ESPLib.h"
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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Sensor & Telemetry handles */
struct bme280_dev bme280_inst;
struct bme280_settings bme_cfg;
static uint8_t bme_bus_addr = BME280_I2C_ADDR_PRIM << 1;

static const char NET_SSID[]       = "SNUC";
static const char NET_KEY[]        = "snu12345";
static const char BROKER_HOST[]    = "broker.emqx.io";
static const int  BROKER_PORT      = 1883;
static const char TOPIC_PUB[]      = "stm32/telemetry/bme280";
static const char TOPIC_SUB[]      = "stm32/control";

static uint32_t last_publish_timestamp = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void emit_debug_log(const char *msg);
int8_t bme_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
int8_t bme_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);
void bme_delay(uint32_t period_us, void *intf_ptr);
void setup_environmental_sensors(void);
void MQTT_MessageReceivedCallback(char *topic, char *payload);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void emit_debug_log(const char *msg) {
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 100);
}

int8_t bme_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    if (HAL_I2C_Mem_Read(&hi2c1, bme_bus_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, reg_data, len, 100) == HAL_OK) {
        return BME280_OK;
    }
    return BME280_E_COMM_FAIL;
}

int8_t bme_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    if (HAL_I2C_Mem_Write(&hi2c1, bme_bus_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)reg_data, len, 100) == HAL_OK) {
        return BME280_OK;
    }
    return BME280_E_COMM_FAIL;
}

void bme_delay(uint32_t period_us, void *intf_ptr) {
    (void)intf_ptr;
    uint32_t count = (period_us * (SystemCoreClock / 1000000)) / 7;
    while (count--) {
        __NOP();
    }
}

void setup_environmental_sensors(void) {
    bme280_inst.intf_ptr = &bme_bus_addr;
    bme280_inst.intf     = BME280_I2C_INTF;
    bme280_inst.read     = bme_i2c_read;
    bme280_inst.write    = bme_i2c_write;
    bme280_inst.delay_us = bme_delay;

    if (bme280_init(&bme280_inst) == BME280_OK) {
        emit_debug_log("[OK] BME280 initialized.\r\n");
        bme_cfg.osr_h = BME280_OVERSAMPLING_1X;
        bme_cfg.osr_p = BME280_OVERSAMPLING_4X;
        bme_cfg.osr_t = BME280_OVERSAMPLING_2X;
        bme_cfg.filter = BME280_FILTER_COEFF_16;
        bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, &bme_cfg, &bme280_inst);
        bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &bme280_inst);
    } else {
        emit_debug_log("[ERR] BME280 init failed.\r\n");
    }
}

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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
    HAL_Delay(1500);
    emit_debug_log("\r\n=== STM32 IoT Environmental & MQTT Edge Node ===\r\n");

    setup_environmental_sensors();

    emit_debug_log("Connecting to Wi-Fi via ESP8266...\r\n");
    ESP8266_Init(1, huart1); // Station Mode

    if (ESP8266_ConnectWiFi((char *)NET_SSID, (char *)NET_KEY) == ESP8266_OK) {
        emit_debug_log("[OK] Wi-Fi Associated.\r\n");

        if (MQTT_Connect((char *)BROKER_HOST, BROKER_PORT, "STM32_EdgeNode_01", "", "") == ESP8266_OK) {
            emit_debug_log("[OK] Connected to MQTT Broker.\r\n");
            MQTT_Subscribe((char *)TOPIC_SUB, 0);
        } else {
            emit_debug_log("[ERR] MQTT Broker Connection Failed.\r\n");
        }
    } else {
        emit_debug_log("[ERR] Wi-Fi Connection Failed.\r\n");
    }

/* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
/* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
    // Transmit environmental telemetry payload every 5 seconds
    if ((HAL_GetTick() - last_publish_timestamp) > 5000) {
        last_publish_timestamp = HAL_GetTick();

        struct bme280_data data;
        if (bme280_get_sensor_data(BME280_ALL, &data, &bme280_inst) == BME280_OK) {
            char json_payload[140];
            snprintf(json_payload, sizeof(json_payload),
                     "{\"device\":\"STM32-IoT\",\"temp\":%.2f,\"press\":%.2f,\"hum\":%.2f}",
                     data.temperature, data.pressure / 100.0f, data.humidity);

            emit_debug_log("[PUB] Sending MQTT Telemetry: ");
            emit_debug_log(json_payload);
            emit_debug_log("\r\n");

            MQTT_Publish((char *)TOPIC_PUB, json_payload, 0);
        }
    }

    // Process background MQTT packets
    MQTT_Yield();
    HAL_Delay(10);

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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */
/* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */
/* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */
/* USER CODE END I2C1_Init 2 */

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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */
/* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */
/* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
/* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void MQTT_MessageReceivedCallback(char *topic, char *payload) {
    if (strcmp(topic, TOPIC_SUB) == 0) {
        if (strcmp(payload, "ON") == 0) {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // Active-LOW LED ON
            emit_debug_log("[CMD] Remote LED activated.\r\n");
        } else if (strcmp(payload, "OFF") == 0) {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);   // Active-LOW LED OFF
            emit_debug_log("[CMD] Remote LED deactivated.\r\n");
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
