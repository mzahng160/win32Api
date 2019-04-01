#ifndef __NAMEDMUTEX__
#define __NAMEDMUTEX__
#include <windows.h>
class CNamedMutex
{
public:
	CNamedMutex()
	{
		m_hLock = NULL;
	}

	virtual ~CNamedMutex()
	{
		UninitMutex();
	}

public:
	bool InitMutex(LPCTSTR lpszName)
	{
		if (!m_hLock)
			m_hLock = CreateMutex(NULL, FALSE, lpszName);

		return (m_hLock != NULL);
	}

	void UninitMutex()
	{
		if (m_hLock)
		{
			CloseHandle(m_hLock);
			m_hLock = NULL;
		}
	}

	void Lock()
	{		
		WaitForSingleObject(m_hLock, INFINITE);
	}

	void Unlock()
	{
		ReleaseMutex(m_hLock);
	}

private:
	HANDLE m_hLock;
};
#endif