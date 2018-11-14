#include "Queue.h"

/**
 * 获得队列使用量
 * @param  queue
 * @return       返回已使用量
 */
uint32_t Queue_GetUsed(const QueuePtr queue)
{
    int len = queue->write - queue->read;

    if(len >= 0)
        return (uint32_t)len;
    else
        return (queue->size + len);
}

/**
 * 获得队列空余空间
 * @param  queue
 * @return       队列空余数
 */
uint32_t Queue_GetFree(const QueuePtr queue)
{
    return queue->size - Queue_GetUsed(queue) - 1;
}

/**
 * 检测队列是否满
 * @param  queue
 * @return       1:满    0:不满
 */
int Queue_isFull(const QueuePtr queue)
{
    if(queue->read == (queue->write + 1) % queue->size)
        return 1;
    else
        return 0;
}

/**
 * 检测队列是否为空
 * @param  queue
 * @return       1:空    0:非空
 */
int Queue_isEmpty(const QueuePtr queue)
{
    if(queue->read == queue->write)
        return 1;
    else
        return 0;
}

/**
 * 数据入队
 * @param  queue
 * @param  buf    要入队的数据
 * @param  length 要入队的数据长度
 * @return        返回入队的字节数
 */
uint32_t Queue_Write(QueuePtr queue, const void* buf, uint32_t length)
{
    uint8_t *dataptr = (uint8_t *)buf;
    uint32_t offset = 0;
    uint32_t nwrite = 0;
    uint32_t nfree = 0;

    /* 传入的数据长度为0, 直接返回 */
    if(!length)		return 0;

    /* 队列没有空间, 直接返回 */
    if(!(nfree = Queue_GetFree(queue)))		return 0;

    /* 计算实际能够入队的数据长度 */
    nwrite = nfree>=length ? length : nfree;

    /* 判断队列是否跨尾 */
    offset = queue->size - queue->write;
    if (offset >= nwrite)
    {
        memcpy(queue->payload + queue->write, dataptr, nwrite);
        queue->write += nwrite;
    }
    else
    {
        memcpy(queue->payload + queue->write, dataptr, offset);
        memcpy(queue->payload, dataptr + offset, nwrite - offset);
        queue->write = nwrite - offset;
    }
    return nwrite;
}

/**
 * 数据出队
 * @param  queue
 * @param  buf    存放出队的数据
 * @param  length 出队的数据长度
 * @return        返回出队字节数
 */
uint32_t Queue_Read(QueuePtr queue, void* buf, uint32_t length)
{
    uint8_t *dataptr = (uint8_t *)buf;
    uint32_t offset = 0;
    uint32_t nused = 0;
    uint32_t nread = 0;

    /* 出队数据长度为0, 直接返回 */
    if(!length)		return 0;

    /* 计算实际能够出队的数据长度 */
    if(!(nused = Queue_GetUsed(queue)))		return 0;

    /* 计算实际能够读到的数据长度 */
    nread = nused>=length ? length : nused;

    /* 判断要读的数据是否跨尾 */
    offset = queue->size - queue->read;
    if( offset >= nread)
    {
        memcpy(dataptr, queue->payload + queue->read, nread);
        queue->read += nread;
    }
    else
    {
        memcpy(dataptr, queue->payload + queue->read, offset);
        memcpy(dataptr + offset, queue->payload, nread - offset);
        queue->read = nread - offset;
    }

    return nread;
}

/**
 * 初始化一个队列
 * @param queue
 * @param size 队列大小
 * @param payload 队列缓存地址
 */
void Queue_Init(QueuePtr queue, uint32_t size, void* payload)
{
    queue->read = 0;
    queue->write = 0;
    queue->payload = (uint8_t *)payload;
    queue->size = size;
}

/**
 * 清理队列
 * @param queue
 */
void Queue_Clear(QueuePtr queue)
{
    queue->read = queue->write;
}

/**
 * 动态创建一个队列
 * @param  size 队列大小
 * @return      成功返回队列对象指针, 失败返回NULL
 */
QueuePtr Queue_Create(uint32_t size)
{
    QueuePtr queue = NULL;

    if(!(queue = (QueuePtr)malloc(sizeof(Queue))))
        return NULL;

    queue->size = size;
    queue->read = 0;
    queue->write = 0;
    queue->payload = NULL;

    if(!(queue->payload = (uint8_t *)malloc(size)))
    {
        free(queue);
        return NULL;
    }

    return queue;
}

/**
 * 对于动态创建的队列进行清理工作
 * @param queue
 */
void Queue_Destory(QueuePtr queue)
{
    free(queue->payload);
    free(queue);
    queue = NULL;
}
