// OneConsumerMultiProducer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<Windows.h>
#include <list>
#include <vector>
#include <iostream>
#include <queue>

#include "Mutex.h"

std::list<std::string> strList;
std::queue<std::string> strQueue;


bool consermerEnabele = false;

Mutex g_Lock;

//producer线程函数的定义
DWORD WINAPI producer(LPVOID IpParameter)
{	
	
	//for (int i = 0; i < 5; i++)
	int i = 0;
	while (1)
	{		
		g_Lock.Lock();
		char buf[10];
		_snprintf_s(buf, sizeof(buf), "***%d", i++);
		//strList.push_front(buf);
		strQueue.push(buf);

		g_Lock.Unlock();
		//Sleep(50);
	}
		

	printf("producer\n");

	return 0;
}

DWORD WINAPI producer_2(LPVOID IpParameter)
{	
	//for (int i = 0; i < 5; i++)
	int i = 0;
	while (1)
	{
		//CLock lock(g_Lock);
		g_Lock.Lock();
		char buf[10];
		_snprintf_s(buf, sizeof(buf), "$$$%d", i++);
		//strList.push_front(buf);

		strQueue.push(buf);
		
		g_Lock.Unlock();
		//Sleep(50);
	}

	printf("producer_2\n");

	return 0;
}

DWORD WINAPI consumer(LPVOID IpParameter)
{	
	while (consermerEnabele)
	{
		g_Lock.Lock();
		//if (!strList.empty())
		//{
		//	std::list<std::string>::iterator it = strList.begin();
		//	if (it != strList.end())
		//	{
		//		//std::string str = *it;
		//		//printf("%s\n", str.c_str());
		//		printf("list size: %d\n", strList.size());
		//		strList.erase(it);
		//	}
		//}

		if (!strQueue.empty())
		{
			printf("list size: %d\n", strQueue.size());
			strQueue.pop();
		}

		g_Lock.Unlock();	
	}

	printf("consumer\n");
	return 0;
}

void TraversingList()
{
	printf("list size: %d\n", strList.size());

	std::list<std::string>::iterator it = strList.begin();	
	int i = 0;
	for (; it != strList.end(); it++, i++)
	{
		std::string str = *it;
		printf("%s\n", str.c_str());		
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE ConsumerThread;
	HANDLE ConsumerThread_2;
	HANDLE ProducerThread_1;
	HANDLE ProducerThread_2;
	
	ProducerThread_1 = CreateThread(NULL, 0, producer, NULL, 0, NULL);
	ProducerThread_2 = CreateThread(NULL, 0, producer_2, NULL, 0, NULL);

	consermerEnabele = true;

	ConsumerThread = CreateThread(NULL, 0, consumer, NULL, 0, NULL);
	ConsumerThread_2 = CreateThread(NULL, 0, consumer, NULL, 0, NULL);
	
	WaitForSingleObject(ProducerThread_1, INFINITE);
	WaitForSingleObject(ProducerThread_2, INFINITE);	

	consermerEnabele = false;

	WaitForSingleObject(ConsumerThread, INFINITE);
	WaitForSingleObject(ConsumerThread_2, INFINITE);

	//关闭线程句柄
	CloseHandle(ConsumerThread);
	CloseHandle(ConsumerThread_2);
	CloseHandle(ProducerThread_1);
	CloseHandle(ProducerThread_2);

	//TraversingList();

	getchar();
	return 0;
}