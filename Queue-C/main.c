/*
 * mian.c
 *
 *  Created on: 2020年10月12日
 *      Author: hello
 */

#include <stdio.h>
#include <string.h>
#include "queue.h"

/* 队列大小 */
#define QUEUE_SIZE 100

/* 队列数据空间。请使用宏QALIGN4，目的是为了根据队列大小计算实际需要的队列存储空间大小并对齐4字节 */
uint8_t QueueBuffer[QALIGN4(QUEUE_SIZE)];

/* 队列句柄 */
Queue queue;
QueuePtr queueHandle = &queue;

int main()
{
	char buf[20];

	memset(buf, 0X00, sizeof(buf));

	// 静态初始化
	Queue_Init(&queue, QUEUE_SIZE, QueueBuffer);

	// 写
	Queue_Write(queueHandle, "HelloWorld", 10);

	// 读
	Queue_Read(queueHandle, buf, 10);

	// 打印
	printf("read:%s\n", buf);

	return 0;
}
