// SyncProcess.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <iostream> 

using namespace std;

const int g_Number = 5;

#define PWrite	//PRead	//PWrite

int _tmain(int argc, _TCHAR* argv[])
{

	HANDLE hThread[g_Number] = { 0 };

	HANDLE readFinish;


#ifdef PRead
	//process--1
	{
		readFinish = CreateSemaphore(NULL, 2, 2, _T("readFinish"));

		int cnt = 0;
		while (cnt++ < 10)
		{
			WaitForSingleObject(readFinish, INFINITE);
			for (int i = 0; i < 10; i++)
			{
				cout << "[" << i << "]"<< cnt << "|" << cnt <<"|"<< cnt << endl;
				Sleep(1000);
			}
			ReleaseSemaphore(readFinish, 1, NULL);
			//ReleaseSemaphore(writeFinish, 1, NULL);

			Sleep(2000);
		}

	}

#endif

#ifdef PWrite
	//process--2

	{
		readFinish = CreateSemaphore(NULL, 2, 2, _T("readFinish"));			
		
		//blockade shared memory about feature
		cout << "blockade shared memory" << endl;
		WaitForSingleObject(readFinish, INFINITE);
		WaitForSingleObject(readFinish, INFINITE);

		cout << "write start" << endl;

		for (int i = 0; i < 10; i++)
		{
			cout << i << "|"<<"22222" << endl;
			Sleep(3000);
		}

		ReleaseSemaphore(readFinish, 1, NULL);
		ReleaseSemaphore(readFinish, 1, NULL);

		cout << "write finish" << endl;
	}

#endif

	//CloseHandle(readFinish);
	//CloseHandle(writeFinish);

	return 0;
}
