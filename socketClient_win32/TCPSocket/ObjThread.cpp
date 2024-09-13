#include "./ObjThread.h"
#include <limits.h>

DWORD WINAPI fRunStub( LPVOID pThread );

const unsigned long ObjThread::INFINIT_WAIT = ULONG_MAX;

ObjThread::ObjThread()
{
    m_bExited = true;
    m_bRunning = false;
    m_ulThredId = 0;
    m_threadHandle = INVALID_HANDLE_VALUE;
}

ObjThread::~ObjThread()
{
}

bool ObjThread::start( unsigned long ulStack )
{
	if( m_bExited == true )
	{
		m_bExited = false;
		if (INVALID_HANDLE_VALUE == (m_threadHandle = CreateThread( NULL, ulStack, fRunStub, this, 0, &m_ulThredId )))
		{
			m_bRunning = false;
			m_bExited = true;
			return false;
		}
	}
	return true;
}

bool ObjThread::stop( unsigned long msTimeout )
{
	if ( m_bExited == false )
	{
		m_bRunning = false;
		WaitForSingleObject( m_threadHandle, (msTimeout == INFINIT_WAIT)?INFINITE:msTimeout );
		DWORD exitCode;
		GetExitCodeThread( m_threadHandle, &exitCode );
		if (STILL_ACTIVE != exitCode)
		{
			TerminateThread( m_threadHandle, 0 );
			GetExitCodeThread( m_threadHandle, &exitCode );
		}
		CloseHandle( m_threadHandle );
		return STILL_ACTIVE != exitCode;
	}
	return true;
}

volatile bool ObjThread::isRunning()
{
	return m_bRunning;
}

volatile bool ObjThread::isExited()
{
	return m_bExited;
}

DWORD WINAPI fRunStub( LPVOID pThread )
{
	ObjThread* pSysThread = static_cast<ObjThread* >(pThread);
	pSysThread->m_bExited = false;
	pSysThread->m_bRunning = true;
	pSysThread->run();
	pSysThread->m_bRunning = false;
	pSysThread->m_bExited = true;
	return 0;
}
