#ifndef __SYSMUTEX_H__
#define __SYSMUTEX_H__

#include <windows.h>

class ObjMutex
{
public:
	ObjMutex();
	virtual ~ObjMutex();
	
	void lock( void );
	void unlock( void );

private:
	CRITICAL_SECTION* m_pMutex;
};

#endif //__SYSMUTEX_H__
