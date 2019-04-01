// OneConsumerMultiProducer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<Windows.h>
#include <list>
#include <vector>
#include <iostream>
#include <queue>

#include "CNamedMutex.h"
#include "Mutex.h"

std::list<std::string> strList;
std::queue<std::string> strQueue;
std::queue<std::string> strQueueWrite;

bool consermerEnabele = false;

CNamedMutex g_Lock;
//Mutex g_Lock;
const int COUNTVALUE = 1000000;

const int bufLen = 128;

//producer线程函数的定义
DWORD WINAPI producer(LPVOID IpParameter)
{	
	int arg = *(int*)IpParameter;	
	int i = COUNTVALUE;
	char buf[bufLen];

	char data[bufLen-6];
	for (int index = 0; index < bufLen - 7; ++index)
	{
		data[index] = '$';
	}
	data[bufLen - 7] = '\0';

	while (1)
	{
		_snprintf(buf, sizeof(buf), "%s%d-%d", data, arg, --i);
		buf[bufLen-1] = '\0';

		g_Lock.Lock();
		strQueue.push(buf);
		//strList.push_back(buf);
		g_Lock.Unlock();

		if (0 == i)
			break;
	}
		

	printf("producer\n");

	return 0;
}

DWORD WINAPI consumer(LPVOID IpParameter)
{	
	int arg = *(int*)IpParameter;
	while (consermerEnabele)
	{
		std::string str;
		g_Lock.Lock();
		if (!strQueue.empty())
		{					
			str = strQueue.front();	
			strQueue.pop();
		}


		//if (!strList.empty())
		//{
		//	str = *(strList.begin());
		//	strList.pop_front();			
		//}
		g_Lock.Unlock();		
	}

	printf("consumer-finish\n");
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
	HANDLE ConsumerThread = NULL;
	HANDLE ConsumerThread_2 = NULL;
	HANDLE ProducerThread_1 = NULL;
	HANDLE ProducerThread_2 = NULL;
	
	g_Lock.InitMutex(L"g_Lock");

	DWORD start = GetTickCount();

	int p1 = 1, p2 = 2;
	ProducerThread_1 = CreateThread(NULL, 0, producer, (LPVOID)&p1, 0, NULL);
	//ProducerThread_2 = CreateThread(NULL, 0, producer, (LPVOID)&p2, 0, NULL);

	consermerEnabele = true;

	ConsumerThread = CreateThread(NULL, 0, consumer, (LPVOID)&p1, 0, NULL);
	//ConsumerThread_2 = CreateThread(NULL, 0, consumer, (LPVOID)&p2, 0, NULL);
	
	WaitForSingleObject(ProducerThread_1, INFINITE);
	WaitForSingleObject(ProducerThread_2, INFINITE);	

	consermerEnabele = false;

	WaitForSingleObject(ConsumerThread, INFINITE);
	WaitForSingleObject(ConsumerThread_2, INFINITE);

	printf("cost time %d\n", GetTickCount() - start);

	//关闭线程句柄	
	
	CloseHandle(ProducerThread_1);
	CloseHandle(ProducerThread_2);
	CloseHandle(ConsumerThread);	
	CloseHandle(ConsumerThread_2);

	g_Lock.UninitMutex();

	getchar();
	return 0;
}