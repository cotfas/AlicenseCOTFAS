
#include "ObjMutex.h"
#include <cassert>

ObjMutex::ObjMutex()
{
    m_pMutex = new CRITICAL_SECTION;
    InitializeCriticalSection( m_pMutex );
}

ObjMutex::~ObjMutex()
{
    DeleteCriticalSection( m_pMutex );
	delete m_pMutex;
}

void ObjMutex::lock()
{
    EnterCriticalSection( m_pMutex );
}

void ObjMutex::unlock()
{
    LeaveCriticalSection( m_pMutex );
}

