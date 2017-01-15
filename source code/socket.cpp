#include "socket.h"

bool Socket::_init(void)
{
  WSADATA WSAData;  
  return WSAStartup(MAKEWORD(2, 2), &WSAData) == 0 ? true : false;
}


void Socket::_uninit(void)
{
    WSACleanup();
}


bool Socket::_initialize(ConnectionData* connData, pURL url)
{
    LPHOSTENT lpHost = (LPHOSTENT)gethostbyname( (const char*)url->host );
	if(lpHost == NULL)
	return false;
	
	sockaddr_in sockaddr;
	
	sockaddr.sin_family		 = AF_INET;
	sockaddr.sin_addr.s_addr = **(unsigned long**)lpHost->h_addr_list;
	sockaddr.sin_port		 = (USHORT)htons( (unsigned short)url->port );
	
	SOCKET Socket = (SOCKET)socket( AF_INET, SOCK_STREAM, 0 );
	
	connData->SockAddr = sockaddr;
	connData->Socket = Socket;
	
	return true;
}


bool Socket::_send( SOCKET Socket, const char *Data, int DataLen )
{
	return  send(Socket, Data, DataLen, 0) == SOCKET_ERROR ? false : true;
}


int Socket::_recv(SOCKET Socket, char* TempBuffer)
{
  int szSocketBuf = 1024;
  int bytes = 0;
  
  bytes = recv(Socket, TempBuffer, szSocketBuf, 0); 
  
  return bytes;
}


bool Socket::_connect(ConnectionData* conndata)
{
	int result;
	bool ret = true;;
	if (result = connect( conndata->Socket, (const struct sockaddr*)&conndata->SockAddr, sizeof( conndata->SockAddr ) ) == SOCKET_ERROR )
	{
		closesocket(conndata->Socket);
		ret = false;
	}
	return ret;
}



