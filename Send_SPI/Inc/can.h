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

extern void can_init(void);
extern int8_t can_read_status(int8_t *buffer, int8_t size); 
extern int8_t can_read_register(int8_t addr);
extern void can_set_register(int8_t addr, int8_t data);
extern void can_send_message(int16_t code, int8_t dlc, int8_t *data);

extern int8_t can_has_message(void);
extern void can_get_message(int16_t *code, int8_t *dlc, int8_t *datai);


#endif /* CAN_H_ */
