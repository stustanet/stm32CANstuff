/*
 * mcp2515.c
 *
 * Created: 17.09.2016 18:53:38
 *  Author: Johannes
 */

#include "mcp2515.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_conf.h"

#define SPI_RESET       (0xC0)
#define	SPI_READ		    (0x03)
#define	SPI_READ_RX		  (0x90)
#define	SPI_WRITE		    (0x02)
#define	SPI_WRITE_TX	  (0x40)
#define	SPI_RTS			    (0x80)
#define SPI_READ_STATUS	(0xA0)
#define	SPI_RX_STATUS	  (0xB0)
#define	SPI_BIT_MODIFY  (0x05)

// SPI handle
static SPI_HandleTypeDef spi = { .Instance = SPI1 };

static void _spi_init(void);
static void _mcp2515_select(void);
static void _mcp2515_deselect(void);

static void _spi_init() {

  __SPI1_CLK_ENABLE();
  spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  spi.Init.Direction = SPI_DIRECTION_2LINES;
  spi.Init.CLKPhase = SPI_PHASE_2EDGE;
  spi.Init.CLKPolarity = SPI_POLARITY_HIGH;
  spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
  spi.Init.DataSize = SPI_DATASIZE_8BIT;
  spi.Init.FirstBit = SPI_FIRSTBIT_LSB;
  spi.Init.NSS = SPI_NSS_SOFT;
  spi.Init.TIMode = SPI_TIMODE_DISABLED;
  spi.Init.Mode = SPI_MODE_MASTER;
  HAL_SPI_Init(&spi);
  GPIO_InitTypeDef  GPIO_InitStruct;

  GPIO_InitStruct.Pin       = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin  = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void mcp2515_init() {
	//Use DDRB-3 as OC1B and send a PWN to the MCP2515 (with blackjack and hookers)

  // Init SPI
  _spi_init();
  _mcp2515_deselect();
  mcp2515_reset();
}

static void _mcp2515_select() {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

static void _mcp2515_deselect() {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

void mcp2515_reset() {
  _mcp2515_select();
  uint8_t c = SPI_RESET;
  HAL_SPI_Transmit(&spi, &c, (uint8_t) 1, HAL_MAX_DELAY);
  HAL_Delay(200);
  _mcp2515_deselect();
}

void mcp2515_rts(uint8_t mode) {
  _mcp2515_select();
  uint8_t c = SPI_RTS | mode;
  HAL_SPI_Transmit(&spi, &c, (uint8_t) 1, HAL_MAX_DELAY);
  _mcp2515_deselect();
}

void mcp2515_write(uint8_t addr,
                   uint8_t *tx,
                   uint16_t size)
{
  _mcp2515_select();
  uint8_t c = SPI_WRITE;
  HAL_SPI_Transmit(&spi, &c, (uint16_t) 1, HAL_MAX_DELAY);
  HAL_SPI_Transmit(&spi, &addr, (uint16_t) 1, HAL_MAX_DELAY);
  HAL_SPI_Transmit(&spi, tx, size, HAL_MAX_DELAY);
  _mcp2515_deselect();
}

void mcp2515_read(uint8_t addr,
                  uint8_t *rx,
                  uint16_t size)
{
  _mcp2515_select();
  uint8_t c = SPI_READ;
  HAL_SPI_Transmit(&spi, &c, (uint16_t) 1, HAL_MAX_DELAY);
  HAL_SPI_Transmit(&spi, &addr, (uint16_t) 1, HAL_MAX_DELAY);
  HAL_SPI_Receive(&spi, rx, size, HAL_MAX_DELAY);
  _mcp2515_deselect();
}

void mcp2515_read_rx(uint8_t mode,
                     uint8_t *rx,
                     uint16_t size)
{
  _mcp2515_select();
  uint8_t c = SPI_READ_RX | mode;
  HAL_SPI_Transmit(&spi, &c, (uint16_t) 1, HAL_MAX_DELAY);
  HAL_SPI_Receive(&spi, rx, size, HAL_MAX_DELAY);
  _mcp2515_deselect();
}

void mcp2515_write_tx(uint8_t mode,
                      uint8_t *tx,
                      uint16_t size)
{
  _mcp2515_select();
  uint8_t c = SPI_WRITE_TX | mode;
  HAL_SPI_Transmit(&spi, &c, (uint16_t) 1, HAL_MAX_DELAY);
  HAL_SPI_Transmit(&spi, tx, size, HAL_MAX_DELAY);
  _mcp2515_deselect();
}

uint8_t mcp2515_read_status() {
  _mcp2515_select();
  uint8_t c = SPI_READ_STATUS;
  HAL_SPI_Transmit(&spi, &c, (uint16_t) 1, HAL_MAX_DELAY);
  HAL_SPI_Receive(&spi, &c, 1, HAL_MAX_DELAY);
  _mcp2515_deselect();
  return c;
}

uint8_t mcp2515_read_rx_status() {
  _mcp2515_select();
  uint8_t c = SPI_RX_STATUS;
  HAL_SPI_Transmit(&spi, &c, (uint16_t) 1, HAL_MAX_DELAY);
  HAL_SPI_Receive(&spi, &c, 1, HAL_MAX_DELAY);
  _mcp2515_deselect();
  return c;
}

void mcp2515_bit_modify(uint8_t addr,
                        uint8_t mask,
                        uint8_t data)
{
  _mcp2515_select();
  uint8_t c = SPI_BIT_MODIFY;
  HAL_SPI_Transmit(&spi, &c, (uint16_t) 1, HAL_MAX_DELAY);
  HAL_SPI_Transmit(&spi, &mask, (uint16_t) 1, HAL_MAX_DELAY);
  HAL_SPI_Transmit(&spi, &data, (uint16_t) 1, HAL_MAX_DELAY);
  _mcp2515_deselect();
}
