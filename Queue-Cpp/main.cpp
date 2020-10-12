/*
 * main.cpp
 *
 *  Created on: 2020年10月12日
 *      Author: hello
 */


#include <iostream>
#include <stdio.h>
#include <string.h>
#include "cqueue.h"

using namespace std;

int main()
{
	char buf[20];

	// 实例对象
	CQueue queue(20);

	// 清除缓存
	memset(buf, 0X00, sizeof(buf));

	// 读
	queue.write("helloworld", 10);

	// 写
	queue.read(buf, 10);

	// 打印
	printf("read:%s\n", buf);

	return 0;
}

