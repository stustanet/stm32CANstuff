/*
 * can.h
 *
 * Created: 19.09.2016 21:39:37
 *  Author: Johannes
 */ 


#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>
#include <stdbool.h>


struct __attribute__((packed)) can_message {
	int8_t control;
	int8_t sidh;
	int8_t sidl;
	int8_t eid8;
	int8_t eid0;
	int8_t dlc;
	int8_t data[8];
};

#define CAN_INIT_MESSAGE(IDENTIFIER,LENGTH) {\
	.control = 0, \
	.sidh=(int8_t)((IDENTIFIER) >> 6), \
	.sidl=(int8_t)((IDENTIFIER) << 5), \
	.eid8=0, \
	.eid0=0, \
	.dlc=(LENGTH), \
	.data={0}, \
}

#define CAN_INIT_EXTENDED_MESSAGE(IDENTIFIER,LENGTH) { \
	NOT IMPLEMENTED YET

extern void can_init(void);
extern void can_send_message(struct can_message *);
extern int8_t can_has_message(void);
extern void can_get_message(int8_t buffer, struct can_message *message);


#endif /* CAN_H_ */
