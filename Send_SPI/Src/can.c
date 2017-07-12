/*
 * can.c
 *
 * Created: 19.09.2016 21:39:48
 *  Author: Johannes
 */

#include "can.h"
#include "mcp2515.h"

#include <stdint.h>

#define CAN_OVER_SPI_DEBUG

void can_init() {
	mcp2515_init();
	uint8_t stat=0;

	// Set configuration mode:
	// Configuration mode
	// disable external clock,
	// abort pending transmissions
	do {
		mcp2515_bit_modify(MCP2515_CANCTRL, 0b11101100, 0b10000000);
		// Read stat register to enshure that we could talk to the chip
		mcp2515_read(MCP2515_CANSTAT, &stat, 1);
	} while ((stat & 0b11100000) != 0b10000000);

	//Turn mask/filters off; receive any message for RXB0
	mcp2515_bit_modify(MCP2515_RXB0CTRL, 0xff, 0b01100000);

	//Set masks to 0 anyways
	mcp2515_bit_modify(MCP2515_RXF0SIDH, 0xff, 0b00000000);
	mcp2515_bit_modify(MCP2515_RXF0SIDL, 0xff, 0b00000000);
	mcp2515_bit_modify(MCP2515_TXB0CTRL, 0xff, 0b00000000);
	mcp2515_bit_modify(MCP2515_TXB1CTRL, 0xff, 0b00000000);


	//set normal mode and ready for rumble
	do {
		mcp2515_bit_modify(MCP2515_CANCTRL, 0b11100000, 0b00000000);
		// Read stat register to enshure that we could talk to the chip
		stat=0;
		mcp2515_read(MCP2515_CANSTAT, &stat, 1);
	} while ((stat & 0b11100000) != 0b00000000);
}


void can_send_message(struct can_message *message) {
	// Store the message in the can buffer
	mcp2515_write(MCP2515_TXB0CTRL, (uint8_t *)message, 6 + message->dlc);

	// Send the message
	mcp2515_bit_modify(MCP2515_TXB0CTRL, 0x08, 0x08);
}

int8_t can_has_message(void) {
	uint8_t data = mcp2515_read_status();
	return (data & 0x3);
}

void can_get_message(int8_t buffer_id, struct can_message *message) {
	buffer_id &= 0x04; // always start at sidh, so do not set bit 6

	// FIXME: read dlc and only receive dlc bytes
	mcp2515_read_rx(buffer_id, (uint8_t *)message, sizeof(*message));
}

