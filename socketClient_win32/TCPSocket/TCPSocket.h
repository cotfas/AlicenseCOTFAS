#ifndef __TCPSOCKET_H__
#define __TCPSOCKET_H__

#include <cassert>
#include <winsock.h>
//#include <Winsock2.h>
#include <string>
using namespace std;

class TCPSocket
{
public:
    static bool initSocketLibrary();
    static void closeSocketLibrary();

public:
    TCPSocket();
    virtual ~TCPSocket();

    const string getHostIP() const;

    int getStatus();
    void setStatus( int iStatus );

    int getData( char* cBuffer, int iSize );
    int sendData( const char* cBuffer, int iSize );

    TCPSocket* acceptSocket();
    bool makeserverSockListener( int port );
    bool makeClientSocket( string host, int port );
    
public:
	bool isSocketInit();
public:
    static const int SOCKET_INVALIDATE;

protected:
    static bool   m_bInitialized;

protected:
    int         m_hSocket;
    int         m_iSocketStatus;
    string      m_psRemoteHostIP;
};

#endif //__TCPSOCKET_H__

