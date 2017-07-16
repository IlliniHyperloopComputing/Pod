/*
 * circular_buffer.c
 *
 * Created: 7/16/2017 10:42:39 AM
 *  Author: rgw3d
 */ 

#include "circular_buffer.h"

void circular_buffer_push(circular_buffer_t * cb, uint8_t data){
	cb->buffer[cb->front] = data;
	cb->front++;
	cb->front =  cb->front % MAX_BUFFER_SIZE;
	//cb->len++;
}

uint8_t circular_buffer_pop(circular_buffer_t * cb){
	uint8_t ret = cb->buffer[cb->back];
	cb->back++;
	cb->back = cb->back % MAX_BUFFER_SIZE;
	//cb->len--;
	return ret;
}

inline uint8_t circular_buffer_size(circular_buffer_t * cb){
	return ((uint8_t)(cb->front - cb->back))%MAX_BUFFER_SIZE;
}

