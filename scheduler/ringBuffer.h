/*
 * ringBuffer.h
 *
 *  Created on: 11.5.2013
 *      Author: kubanec
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

typedef struct buffer_t buffer_t;
struct buffer_t
{
	uint8_t dataSize;
	uint8_t head;
	uint8_t tail;
	uint8_t maximum;
	void * data;
};

typedef enum {COPY,NO_COPY} buffer_cpy_t;

//api
uint8_t bufPut(buffer_t * handle, const void * data);
uint8_t bufGet(buffer_t * handle, void ** retData, buffer_cpy_t cpy);
uint8_t bufIsFull(const buffer_t * handle);
uint8_t bufIsEmpty(const buffer_t * handle);

#define buf_DECLARE(name, dataPtr,dataSze,count) \
	buffer_t name = {.data = dataPtr, .dataSize = dataSze, .head = 0, .tail = 0, .maximum = count-1}

#endif /* RINGBUFFER_H_ */
