#ifndef __SYSTHREAD_H__
#define __SYSTHREAD_H__

#include <windows.h>

class ObjThread
{
public:
	ObjThread();
	virtual ~ObjThread();

	bool start( unsigned long ulStack = 4096 );
	bool stop( unsigned long msTimeout = 0 );

    volatile bool isRunning();
	volatile bool isExited();

public:
	static const unsigned long INFINIT_WAIT;
	
protected:
	virtual void run() = 0;

private:
	volatile bool m_bExited;
	volatile bool m_bRunning;

private:
	friend DWORD WINAPI fRunStub( LPVOID pThread );

	HANDLE m_threadHandle;
	DWORD m_ulThredId;
};

#endif //__SYSTHREAD_H__
