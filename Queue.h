#ifndef __QUEUE_H_
#define __QUEUE_H_

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus


#include <stdint.h>
#include <string.h>
#include <malloc.h>

typedef struct queue
{
    uint32_t read;
    uint32_t write;
    uint32_t size;
    uint8_t* payload;
}Queue, *QueuePtr;


uint32_t Queue_GetUsed(const QueuePtr queue);
uint32_t Queue_GetFree(const QueuePtr queue);
uint32_t Queue_Write(QueuePtr queue, const void* buf, uint32_t length);
uint32_t Queue_Read(QueuePtr queue, void* buf, uint32_t length);
void Queue_Clear(QueuePtr queue);
void Queue_Init(QueuePtr queue, uint32_t size, void* payload);
void Queue_Destory(QueuePtr queue);
int Queue_isFull(const QueuePtr queue);
int Queue_isEmpty(const QueuePtr queue);
QueuePtr Queue_Create(uint32_t size);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __QUEUE_H_
