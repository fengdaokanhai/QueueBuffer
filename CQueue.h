//
// Created by AnKun on 2018/11/14.
//

#ifndef QUEUEBUFFER_CQUEUE_H
#define QUEUEBUFFER_CQUEUE_H

#include <stdint.h>
#include <string.h>

class CQueue
{
public:
    CQueue(uint32_t size);
    CQueue(const CQueue& cqueue);
    ~CQueue();

    CQueue operator = (const CQueue& queue)
    {
        return CQueue(queue);
    }

    uint32_t read(void* buf, uint32_t length);
    uint32_t write(const void* buf, uint32_t length);
    bool isFull();
    bool isEmpty();
    uint32_t getUsed();
    uint32_t getFree();
    uint32_t getSize();
    void clear();

private:
    uint32_t writepos;
    uint32_t readpos;
    uint32_t size;
    uint8_t* payload;
};


#endif //QUEUEBUFFER_CQUEUE_H
