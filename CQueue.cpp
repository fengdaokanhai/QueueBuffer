//
// Created by AnKun on 2018/11/14.
//

#include "CQueue.h"

CQueue::CQueue(uint32_t size) {
    writepos = 0;
    readpos = 0;
    this->size = size;
    payload = new uint8_t[size];
}

CQueue::~CQueue() {
    delete[] payload;
}

uint32_t CQueue::read(void *buf, uint32_t length) {
    uint8_t* dataptr = (uint8_t *)buf;
    uint32_t offset = 0, nused = 0, nread = 0;

    if (!length) return 0;
    if (!(nused = getUsed())) return 0;

    nread = nused>=length?length:nused;
    offset = size - readpos;

    if (offset >= nread)
    {
        memcpy(dataptr, payload + readpos, nread);
        readpos += nread;
    } else
    {
        memcpy(dataptr, payload + readpos, offset);
        memcpy(dataptr + offset, payload, nread - offset);
        readpos = nread - offset;
    }
    return nread;
}

uint32_t CQueue::write(const void* buf, uint32_t length) {
    uint8_t* dataptr = (uint8_t *)buf;
    uint32_t offset = 0, nwrite = 0, nfree = 0;

    if (!length) return 0;
    if (!(nfree = getFree())) return 0;

    nwrite = nfree>=length?length:nfree;
    offset = size - writepos;

    if (offset >= nwrite)
    {
        memcpy(payload + writepos, dataptr, nwrite);
        writepos += nwrite;
    } else
    {
        memcpy(payload + writepos, dataptr, offset);
        memcpy(payload, dataptr + offset, nwrite - offset);
        writepos = nwrite - offset;
    }
    return nwrite;
}

bool CQueue::isFull() {
    return (readpos == (writepos + 1) % size)? true: false;
}

bool CQueue::isEmpty() {
    return readpos == writepos ? true: false;
}

uint32_t CQueue::getUsed() {
    int len = writepos - readpos;

    if (len >= 0)
        return (uint32_t)len;
    else
        return (size + len);
}

uint32_t CQueue::getFree() {
    return size - getUsed() - 1;
}

uint32_t CQueue::getSize() {
    return size;
}

void CQueue::clear() {
    writepos = readpos;
}

CQueue::CQueue(const CQueue &cqueue) {
    readpos = cqueue.readpos;
    writepos = cqueue.writepos;
    size = cqueue.size;
    payload = new uint8_t[size];
    memcpy(payload, cqueue.payload, size);
}
