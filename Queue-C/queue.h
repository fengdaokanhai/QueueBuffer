/**
 * @file    queue.h
 * @brief   Create by AnKun on 2019/10/30
 * @version 1.0.0
 */
#ifndef __QUEUE_H_
#define __QUEUE_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/// 队列控制结构体
typedef struct
{
    uint32_t read;			///< 读指针
    uint32_t write;			///< 写指针
    uint32_t size;			///< 队列大小(字节)
    uint8_t* payload;		///< 队列负载数据
}Queue, *QueuePtr;

/// 对齐四字节并预留出空间
#define QALIGN4(size)  (((size + 5) >> 2) << 2)

/// 函数倒出
uint32_t Queue_GetSize(const QueuePtr queue);
uint32_t Queue_GetUsed(const QueuePtr queue);
uint32_t Queue_GetFree(const QueuePtr queue);
uint32_t Queue_Write(QueuePtr queue, const void* buf, uint32_t length);
uint32_t Queue_Read(QueuePtr queue, void* buf, uint32_t length);
void Queue_Clear(QueuePtr queue);
void Queue_ErrBack(QueuePtr queue, uint32_t len);
void Queue_Init(QueuePtr queue, uint32_t size, void* payload);
void Queue_Destory(QueuePtr queue);
int Queue_isFull(const QueuePtr queue);
int Queue_isEmpty(const QueuePtr queue);
QueuePtr Queue_Create(uint32_t size);

#endif // __QUEUE_H_




