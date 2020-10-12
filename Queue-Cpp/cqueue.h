/*
 * cqueue.h
 *
 *  Created on: 2020Äê9ÔÂ11ÈÕ
 *      Author: hello
 */

#ifndef CQUEUE_H_
#define CQUEUE_H_

#include <stdint.h>

class CQueue
{
public:
	CQueue(uint32_t size);
	~CQueue();
	uint32_t write(const void* buf, uint32_t len);
	uint32_t read(void* buf, uint32_t len);
	bool isEmpty(void);
	bool isFull(void);
	uint32_t getSize(void);
	uint32_t getFree(void);
	uint32_t getUsed(void);
	void clear(void);
private:
	uint32_t windex;
	uint32_t rindex;
	uint32_t size;
	uint8_t* payload;
	void qmemcpy(void* dst, const void* src, uint32_t len);
};

#endif /* CQUEUE_H_ */

