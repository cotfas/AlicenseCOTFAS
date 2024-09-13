
// socketClient.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "..\res\resource.h"		// main symbols


// CsocketClientApp:
// See socketClient.cpp for the implementation of this class
//

class CsocketClientApp : public CWinApp
{
public:
	CsocketClientApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CsocketClientApp theApp;