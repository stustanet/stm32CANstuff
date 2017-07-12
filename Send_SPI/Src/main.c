#include "main.h"
#include "can.h"
#include "mcp2515.h"

static void _uart_init(void);

GPIO_InitTypeDef GPIO_InitStruct;
UART_HandleTypeDef UartHandle;

#define BUFFERSIZE (64)

/* Buffer used for transmission */
uint8_t tx_buffer[BUFFERSIZE] = "WULULULULU";
uint8_t rx_buffer[BUFFERSIZE];

int main(void) {
	// Init HAL and GPIOs
	uint8_t *eol = (uint8_t *)"\r\n";

	HAL_Init();
	__GPIOA_CLK_ENABLE();

	// Init USART
	_uart_init();
	mcp2515_init();
	can_init();

	//mcp2515_reset();
	while(1) {
		HAL_Delay(200);
		struct can_message msg = CAN_INIT_MESSAGE(0x1a1, 2);
		msg.data[0] = 0xbe;
		msg.data[1] = 0xef;

		can_send_message(&msg);

		uint8_t buffer;
		mcp2515_read(MCP2515_TXB0CTRL, &buffer, 1);
		for (uint8_t i = 0; i < 8; ++i) {
			uint8_t b;
			if (((buffer >> i) & 0x1) == 0x0) {
				b = '0';
			} else {
				b = '1';
			}
			HAL_UART_Transmit(&UartHandle, &b, 1, 5000);
		}

        HAL_UART_Transmit(&UartHandle, eol, 2, 5000);
	}
}

static void _uart_init() {

  __USART1_CLK_ENABLE();

  UartHandle.Instance = USART1;
  UartHandle.Init.BaudRate = 115200;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits = UART_STOPBITS_1;
  UartHandle.Init.Parity = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode = UART_MODE_TX_RX;
  UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&UartHandle);

  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

