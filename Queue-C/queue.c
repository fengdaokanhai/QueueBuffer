/**
 * @file    queue.c
 * @brief   Create by AnKun on 2019/10/30
 * @version 1.0.0
 */
#include "queue.h"

static void qmemcpy(void* dst, const void* src, uint32_t len)
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

/**
 * 获得队列使用量
 * @param  queue 队列句柄
 * @return       返回已使用量
 */
uint32_t Queue_GetUsed(const QueuePtr queue)
{
	int len = queue->write - queue->read;
	return len >= 0 ? len : (queue->size + len);
}

/**
 * 获得队列空余空间
 * @param  queue 队列句柄
 * @return       队列空余数
 */
uint32_t Queue_GetFree(const QueuePtr queue)
{
	int len = queue->read - queue->write;
	return len > 0 ? (queue->size - len) : (queue->size - 1 + len);
}

/**
 * 获得队列大小
 * @param  queue 队列句柄
 * @return       队列大小
 */
uint32_t Queue_GetSize(const QueuePtr queue)
{
	return queue->size - 1;
}

/**
 * 检测队列是否满
 * @param  queue
 * @return       1:满    0:非满
 */
bool Queue_isFull(const QueuePtr queue)
{
	if (queue->read == (queue->write + 1) % queue->size)
		return 1;
	else
		return 0;
}

/**
 * 检测队列是否为空
 * @param  queue
 * @return       1:空    fasle:非空
 */
bool Queue_isEmpty(const QueuePtr queue)
{
	if (queue->read == queue->write)
		return 1;
	else
		return 0;
}

/**
 * 数据入队
 * @param  queue  队列句柄
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
	if (length == 0)		return 0;

	/* 队列没有空间, 直接返回 */
	if ((nfree = Queue_GetFree(queue)) == 0)		return 0;

	/* 计算实际能够入队的数据长度 */
	nwrite = nfree >= length ? length : nfree;

	/* 判断队列是否跨尾 */
	offset = queue->size - queue->write;
	if (offset >= nwrite)
	{
		qmemcpy(queue->payload + queue->write, dataptr, nwrite);
		queue->write += nwrite;
	}
	else
	{
		qmemcpy(queue->payload + queue->write, dataptr, offset);
		qmemcpy(queue->payload, dataptr + offset, nwrite - offset);
		queue->write = nwrite - offset;
	}
	return nwrite;
}

/**
 * 数据出队
 * @param  queue  队列句柄
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
	if (length == 0)		return 0;

	/* 计算实际能够出队的数据长度 */
	if ((nused = Queue_GetUsed(queue)) == 0)		return 0;

	/* 计算实际能够读到的数据长度 */
	nread = nused >= length ? length : nused;

	/* 判断要读的数据是否跨尾 */
	offset = queue->size - queue->read;
	if (offset >= nread)
	{
		qmemcpy(dataptr, queue->payload + queue->read, nread);
		queue->read += nread;
	}
	else
	{
		qmemcpy(dataptr, queue->payload + queue->read, offset);
		qmemcpy(dataptr + offset, queue->payload, nread - offset);
		queue->read = nread - offset;
	}

	return nread;
}

/**
 * 初始化一个队列
 * @param queue   队列句柄
 * @param size    队列大小
 * @param payload 队列缓存地址
 */
void Queue_Init(QueuePtr queue, uint32_t size, void* payload)
{
	queue->read = 0;
	queue->write = 0;
	queue->payload = (uint8_t *)payload;
	queue->size = size + 1;
}

/**
 * 清空队列
 * @param queue 队列句柄
 */
void Queue_Clear(QueuePtr queue)
{
	queue->write = queue->read;
}

/**
 * 动态创建一个队列
 * @param  size 队列大小
 * @return      成功返回队列对象指针, 失败返回NULL
 */
QueuePtr Queue_Create(uint32_t size)
{
	QueuePtr queue = NULL;

	if ((queue = (QueuePtr)malloc(sizeof(Queue))) == NULL)
		return NULL;

	queue->size = size + 1;
	queue->read = 0;
	queue->write = 0;
	queue->payload = NULL;

	if ((queue->payload = (uint8_t *)malloc(QALIGN4(size))) == NULL)
	{
		free(queue);
		return NULL;
	}

	return queue;
}

/**
 * 对于动态创建的队列进行清理工作
 * @param queue 队列句柄
 */
void Queue_Destory(QueuePtr queue)
{
	free(queue->payload);
	queue->payload = NULL;
	free(queue);
	queue = NULL;
}

/**
 * 读指针退后len个字节，！！！慎用！！！
 * @param queue 队列句柄
 * @param len   要退后的字节数
 */
void Queue_ErrBack(const QueuePtr queue, uint32_t len)
{
	int tmp = queue->read - len;
	queue->read = tmp < 0 ? ((uint32_t)(tmp + queue->size)) : ((uint32_t)tmp);
}

bool Queue_PutByte(QueuePtr queue, uint8_t dat)
{
	if(Queue_isFull(queue))
	{
		return false;
	}

	/* 判断队列是否跨尾 */
	if((queue->size - queue->write) == 0)
	{
		queue->write = 0;
	}

	/* 写入数据 */
	queue->payload[queue->write++] = dat;

	return true;
}

bool Queue_GetByte(QueuePtr queue, uint8_t* dat)
{
	if(Queue_isEmpty(queue))
	{
		return false;
	}

	/* 检查要读的数据是否跨尾 */
	if ((queue->size - queue->read) == 0)
	{
		queue->read = 0;
	}

	/* 读取数据 */
	*dat = queue->payload[queue->read++];

	return true;
}
