/*
 * can.c
 *
 * Created: 19.09.2016 21:39:48
 *  Author: Johannes
 */

#include "can.h"
#include "mcp2515.h"

#include <stdint.h>
#include <stdbool.h>

//#define OPMODE_LOOPBACK
#define CAN_OVER_SPI_DEBUG

void can_init() {
	mcp2515_init();

	// Set configuration mode
  mcp2515_bit_modify(MCP2515_CANCTRL, 0b11101100, 0b10000000);

  // Read stat register
  uint8_t stat=0;
  mcp2515_read(MCP2515_CANSTAT, &stat, 1);
  if ((stat & 0b11100000) != 0b10000000) {
    for(;;) {}
  }

  //	//Turn mask/filters off; receive any message for RXB0
  //	can_set_register(MCP2515_RXB0CTRL, 0b01100000);
  //
  //	//Set masks to 0 anyways
  //	can_set_register(MCP2515_RXF0SIDH, 0b00000000);
  //	can_set_register(MCP2515_RXF0SIDL, 0b00000000);
  //
  //	//set normal mode and ready for rumble
  //#ifndef OPMODE_LOOPBACK
  //	can_set_register(MCP2515_CANCTRL, 0b00000000);
  //	_delay_ms(10); //Give him time to settle.
  //	//DIE MOTHERFUCKER
  //
  //	//can_read_register(MCP2515_CANSTAT);
  //
  //	while ((can_read_register(MCP2515_CANSTAT) & 0xe0) != 0) {
  //		can_set_register(MCP2515_CANCTRL, 0b00000000);
  //		_delay_ms(10);
  //		send_debug_byte('\n');
  //		send_debug_byte('\n');
  //		send_debug_byte('\n');
  //		send_debug_byte(can_read_register(MCP2515_CANSTAT));
  //		send_debug_byte('\n');
  //	}
  //#else
  //	//set loopback mode
  //	can_set_register(MCP2515_CANCTRL, 0b01000000);
  //	_delay_ms(10); //Give him time to settle.
  //	//DIE MOTHERFUCKER
  //
  //	while ((can_read_register(MCP2515_CANSTAT) & 0xe0) != 0b01000000) {
  //		can_set_register(MCP2515_CANCTRL, 0b01000000);
  //		_delay_ms(10);
  //		send_debug_byte('\n');
  //		send_debug_byte('\n');
  //		send_debug_byte('\n');
  //		send_debug_byte(can_read_register(MCP2515_CANSTAT));
  //		send_debug_byte('\n');
  //	}
  //#endif
  //	zero_bytes(MCP2515_TXB0CTRL, 14);
  //	zero_bytes(MCP2515_TXB1CTRL, 14);
  //	zero_bytes(MCP2515_TXB2CTRL, 14);
}

//void can_set_register(int8_t addr, int8_t data) {
//	 mcp2515_select();
//	 spi_transfer(SPI_WRITE);
//	 spi_transfer(addr);
//	 spi_transfer(data);
//	 mcp2515_deselect();
// }
//
// uint8_t can_read_register(int8_t addr) {
//
//	 int8_t data;
//	 mcp2515_select();
//
//	 spi_transfer(SPI_READ);
//	 spi_transfer(addr);
//	 data = spi_transfer(0xff);
//
//	 mcp2515_deselect();
//
//	 return data;
// }
//
//
//void can_send_message(int16_t code, int8_t dlc, int8_t *data) {
//	mcp2515_select();
//
//	//Load TX Buffer, Start at TXB0SIDH
//	spi_transfer(SPI_WRITE_TX);
//	spi_transfer(code >> 3); //high byte
//	spi_transfer(code << 5); //low 3 bit
//	spi_transfer(0); //unused
//	spi_transfer(0); //unused
//	spi_transfer(dlc); //dlc
//
//	for (int i = 0; i < dlc && i < 8; ++i) {
//		spi_transfer(data[i]);
//	}
//
//	mcp2515_deselect();
//	_delay_ms(1);
//#ifdef CAN_OVER_SPI_DEBUG
//	int8_t r[] = {
//		can_read_register(MCP2515_CANSTAT),
//		0xff,
//		can_read_register(MCP2515_TXB0SIDH),
//		can_read_register(MCP2515_TXB0SIDL),
//		can_read_register(MCP2515_TXB0EID8),
//		can_read_register(MCP2515_TXB0EID0),
//		can_read_register(MCP2515_TXB0DLC),
//		can_read_register(MCP2515_TXB0D0),
//		can_read_register(MCP2515_TXB0D1),
//		can_read_register(MCP2515_TXB0D2),
//		can_read_register(MCP2515_TXB0D3),
//		can_read_register(MCP2515_TXB0D4),
//		can_read_register(MCP2515_TXB0D5),
//		can_read_register(MCP2515_TXB0D6),
//		can_read_register(MCP2515_TXB0D7),
//		'\n'
//	};
//	send_debug_multibyte(sizeof(r), r);
//
//#endif
//
//	can_set_register(MCP2515_TXB0CTRL, (1<<3)); //Request for Transmission
//	can_set_register(MCP2515_TXB1CTRL, 0);
//	can_set_register(MCP2515_TXB2CTRL, 0);
//	while(0) {
//
//		int8_t status = can_read_register(MCP2515_TXB0CTRL); //Request for Transmission
//
//		/*mcp2515_select();
//		spi_transfer(SPI_READ_STATUS);
//		int8_t status = spi_transfer(0xff);
//		mcp2515_deselect();
//		*/
//		send_debug_byte(0xde);
//		send_debug_byte(0xad);
//		send_debug_byte(0xbe);
//		send_debug_byte(0xef);
//
//		send_debug_byte(status);
//
//		if (!(status & (1 << 3))) 
//			break;
//
//		if (status & 0x50) {
//		//	_delay_ms(1);
//		//	can_set_register(MCP2515_TXB0CTRL, 0); //Reset and request for transmission again
//		//	_delay_ms(10);
//		//	can_set_register(MCP2515_TXB0CTRL, (1<<3)); //Reset and request for transmission again
//			send_debug_byte(can_read_register(MCP2515_TEC));
//			send_debug_byte(can_read_register(MCP2515_EFLG));
//
//		}
//
//		send_debug_byte('\n');
//	}
//	//send_debug_byte(2);
//}
//
//int8_t can_has_message(void) {
//	uint8_t data = can_read_register(MCP2515_CANINTF);
//	can_set_register(MCP2515_CANINTF, data & 0xb11111100);
//	can_set_register(MCP2515_CANINTF, 0);
//
///*	mcp2515_select();
//
//	spi_transfer(SPI_RX_STATUS);
//	uint8_t data = spi_transfer(0xff);
//
//	mcp2515_deselect();
//*/
//	send_debug_byte(data);
//	send_debug_byte('\n');
//
//	return data & 0x03;
//}
//
//void can_get_message(int16_t *code, int8_t *dlc, int8_t *data) {
//	//Read SIP
//  *code = ((int16_t)can_read_register(MCP2515_RXB0SIDH))<<3;
//  *code |= can_read_register(MCP2515_RXB0SIDL) >> 5;
//
//  //Read DLC
//  *dlc = can_read_register(MCP2515_RXB0DLC) & 0x03;
//
//	//Read Data
//  for (int i = 0; i < *dlc; ++i) {
//    data[i] = can_read_register(MCP2515_RXB0D0 + i);
//  }
//
//  int8_t ctrl = can_read_register(MCP2515_CANINTF);
//	//Clear RX0IF
//	ctrl &= ~0x01;
//	can_set_register(MCP2515_CANINTF, ctrl);
//}

