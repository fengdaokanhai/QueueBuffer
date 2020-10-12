/*
 * queue.c
 *
 *  Created on: 2020Äê9ÔÂ11ÈÕ
 *      Author: hello
 */

#include "cqueue.h"

CQueue::CQueue(uint32_t size)
{
	this->rindex = 0;
	this->windex = 0;
	this->size = size + 1;
	this->payload = new uint8_t[((this->size + 5) >> 2) << 2];
}

CQueue::~CQueue(void)
{
	delete[] this->payload;
}

void CQueue::qmemcpy(void* dst, const void* src, uint32_t len)
{
	char *d = (char *) dst;
	const char *s = (const char *) src;

	while (len >= sizeof(int))
	{
		*(int*) d = *(int*) s;
		d += sizeof(int);
		s += sizeof(int);
		len -= sizeof(int);
	}
	while ((len) > 0U)
	{
		*d = *s;
		d += 1U;
		s += 1U;
		len -= 1U;
	}
}

uint32_t CQueue::read(void *buf, uint32_t length)
{
	uint8_t* p = (uint8_t *) buf;
	uint32_t offset = 0, nused = 0, nread = 0;

	if (!length)
		return 0;

	if (!(nused = getUsed()))
		return 0;

	nread = nused >= length ? length : nused;
	offset = size - rindex;

	if (offset >= nread)
	{
		qmemcpy(p, payload + rindex, nread);
		rindex += nread;
	}
	else
	{
		qmemcpy(p, payload + rindex, offset);
		qmemcpy(p + offset, payload, nread - offset);
		rindex = nread - offset;
	}
	return nread;
}

uint32_t CQueue::write(const void* buf, uint32_t length)
{
	const uint8_t* p = (const uint8_t *) buf;
	uint32_t offset = 0, nwrite = 0, nfree = 0;

	if (!length)
		return 0;

	if (!(nfree = getFree()))
		return 0;

	nwrite = nfree >= length ? length : nfree;
	offset = size - windex;

	if (offset >= nwrite)
	{
		qmemcpy(payload + windex, p, nwrite);
		windex += nwrite;
	}
	else
	{
		qmemcpy(payload + windex, p, offset);
		qmemcpy(payload, p + offset, nwrite - offset);
		windex = nwrite - offset;
	}
	return nwrite;
}

bool CQueue::isFull()
{
	return (rindex == (windex + 1) % size) ? true : false;
}

bool CQueue::isEmpty()
{
	return rindex == windex ? true : false;
}

uint32_t CQueue::getUsed()
{
	int len = windex - rindex;
	return len >= 0 ? len : (size + len);
}

uint32_t CQueue::getFree()
{
	int len = rindex - windex;
	return len > 0 ? (size - len) : (size - 1 + len);
}

uint32_t CQueue::getSize()
{
	return this->size - 1;
}

void CQueue::clear()
{
	windex = rindex;
}

