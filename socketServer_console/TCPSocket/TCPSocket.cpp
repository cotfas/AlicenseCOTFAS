#include "./TCPSocket.h"

const int TCPSocket::SOCKET_INVALIDATE = -1;
bool TCPSocket::m_bInitialized = false;

#define SD_RECEIVE		0x00
#define SD_SEND			0x01
#define SD_BOTH			0x02

bool TCPSocket::isSocketInit()
{
	return m_bInitialized;
}

bool TCPSocket::initSocketLibrary()
{
	if ( true == m_bInitialized )
        return true;
	
	WORD socketVersionRequested;
	WSADATA wsaData;
	socketVersionRequested = MAKEWORD (2,0);
	if ( SOCKET_ERROR == WSAStartup( socketVersionRequested, &wsaData ) )
	{ 
		WSACleanup();
		return false;
	}
	m_bInitialized = true;
	
	return true;
}

void TCPSocket::closeSocketLibrary()
{
	WSACleanup();
	m_bInitialized = false;
}


TCPSocket::TCPSocket()
{
	m_hSocket = INVALID_SOCKET;
	m_iSocketStatus = SOCKET_INVALIDATE;
}

TCPSocket::~TCPSocket()
{
    setStatus( SOCKET_INVALIDATE );
}

const string TCPSocket::getHostIP() const
{
    return m_psRemoteHostIP;
}

int TCPSocket::getStatus()
{
    return m_iSocketStatus;
}

void TCPSocket::setStatus( int iStatus )
{
	if (iStatus <= 0) ////////////////
	{
		if (SOCKET_INVALIDATE == m_iSocketStatus)
		{
			return;
		}
		shutdown( m_hSocket, SD_BOTH );
		closesocket( m_hSocket );

		m_hSocket = INVALID_SOCKET;//////////////
	}
	m_iSocketStatus = iStatus<0?SOCKET_INVALIDATE:iStatus;
}

int TCPSocket::getData( char* cBuffer, int iSize )
{
	int iCurrentSize = 0;
	while (iCurrentSize < iSize) //////////////
	{
		int iReaded = recv( m_hSocket, &cBuffer [iCurrentSize], iSize-iCurrentSize, 0 );
		if ((SOCKET_ERROR == iReaded)||(iReaded == 0)) //////
		{
			setStatus( SOCKET_INVALIDATE );
			return iCurrentSize?iCurrentSize:SOCKET_INVALIDATE;
		}
		iCurrentSize += iReaded;
	}
	return iCurrentSize;
}

int TCPSocket::sendData( const char* cBuffer, int iSize )
{
	int iCurrentSize = 0;
	while (iCurrentSize < iSize)
	{
		int iSended = send( m_hSocket, cBuffer + iCurrentSize, iSize-iCurrentSize, 0 );
		if ((SOCKET_ERROR == iSended)||(iSended == 0)) //////
		{
			setStatus( SOCKET_INVALIDATE );
			return iCurrentSize?iCurrentSize:SOCKET_INVALIDATE;
		}
		iCurrentSize += iSended;
	}
	return iCurrentSize;
}

TCPSocket* TCPSocket::acceptSocket()
{
	TCPSocket* pTCPSocket = NULL;
	if ( m_hSocket == INVALID_SOCKET )
	{
		return ( pTCPSocket );
	}
	
	struct sockaddr_in socketAddress;
	int size = sizeof( struct sockaddr );
	SOCKET sock = accept( m_hSocket, ( sockaddr* ) &socketAddress, &size );

	if ( sock != INVALID_SOCKET )
	{
		pTCPSocket = new TCPSocket();
		pTCPSocket->m_hSocket = sock;
		pTCPSocket->m_iSocketStatus = 1;
		pTCPSocket->m_psRemoteHostIP = string( inet_ntoa( socketAddress.sin_addr ) );
	}

	return ( pTCPSocket );
}

bool TCPSocket::makeserverSockListener( int port )
{
    if ( m_hSocket != INVALID_SOCKET )
    {
        return false;
    }

	struct sockaddr_in socketInfo;
	SOCKET sock = socket( AF_INET, SOCK_STREAM, 0 );
	if( sock == INVALID_SOCKET )
	{
		return false;
	}

	int noDelay = 1;
	if( setsockopt( sock, IPPROTO_TCP, TCP_NODELAY, (const char* )&noDelay, sizeof( noDelay ) ) != 0 )
	{
        shutdown( sock, SD_BOTH );
		closesocket( sock );
		return false;
	}

	socketInfo.sin_family = AF_INET;
	socketInfo.sin_port = htons( port );
	socketInfo.sin_addr.s_addr = htonl( INADDR_ANY );

	int status = bind( sock, (struct sockaddr* )&socketInfo, sizeof( struct sockaddr_in ) );
	if( status == SOCKET_ERROR )
	{
        shutdown( sock, SD_BOTH );
		closesocket( sock );
		return false;
	}

    m_hSocket = sock;
    m_iSocketStatus = 0;

	listen( m_hSocket, 10 );
	return true;
}

bool TCPSocket::makeClientSocket( string host, int port )
{
    if ( m_hSocket != INVALID_SOCKET )
    {
        return false;
    }

    int sockStatus;
	SOCKET sock = socket( AF_INET, SOCK_STREAM, 0 );
	if( sock == INVALID_SOCKET )
	{
		return false;
	}

	struct sockaddr_in socketInfo;
	socketInfo.sin_family = AF_INET;
	socketInfo.sin_port = htons( port );

    struct hostent* hostinfo = gethostbyname( host.c_str() );

	if ( hostinfo != NULL )
	{
		socketInfo.sin_addr = *(struct in_addr*)hostinfo->h_addr;
	}
	else
	{
        socketInfo.sin_addr.s_addr = inet_addr( host.c_str() );
	}

	if ( SOCKET_ERROR == ( sockStatus = connect( sock, (struct sockaddr*)&socketInfo, sizeof( struct sockaddr_in ) ) ) )
	{
        shutdown( sock, SD_BOTH );
		closesocket( sock );
		return false;
 	}

    m_iSocketStatus = sockStatus;
    m_hSocket = sock;

	m_psRemoteHostIP = string( inet_ntoa( socketInfo.sin_addr ) );

	m_iSocketStatus = 0;
	return true;
}
