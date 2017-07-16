/*
 * circular_buffer.h
 *
 * Created: 7/16/2017 10:43:02 AM
 *  Author: rgw3d
 */ 


#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_
#include <asf.h>
//
#define MAX_BUFFER_SIZE 50

typedef struct circular_buffer
{
	volatile uint8_t buffer[MAX_BUFFER_SIZE];
	volatile uint8_t front;
	volatile uint8_t back;
} circular_buffer_t;

extern void circular_buffer_push(circular_buffer_t * cb, uint8_t data);

extern uint8_t circular_buffer_pop(circular_buffer_t * cb);

extern uint8_t circular_buffer_size(circular_buffer_t * cb);


#endif /* CIRCULAR_BUFFER_H_ */