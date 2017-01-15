#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#include "socket.h"
//
#include "http.h"
#include "memory.h"


void HTTP::GETRequestHeaders(pHTTPHEADERS http)
{
	
	http->method     = "GET /sqlite3.dll";
	http->version    = " HTTP/1.1\r\n";
	http->host       = "Host:localhost\r\n";
	http->connection = "Connection: Close\r\n\r\n\r\n";
		
}


void HTTP::FormatHeaders(char** header, HTTPHEADERS http)
{
	char* httpQuery = (char*)Memory::_alloc(2048);

	strcpy(httpQuery, http.method);
	strcat(httpQuery, http.version);
	strcat(httpQuery, http.host);
	strcat(httpQuery, http.connection);
	
	*header = httpQuery;
	
	printf("HTTP Headers: %s\n",httpQuery);

}

DWORD ConnectThread(void* data)
{
	ConnectionData* connData = (ConnectionData*)data;

	bool isConnected = Socket::_connect(connData);
	if(isConnected)	
		ExitThread(0);
	else 
		ExitThread(-1);

	return 0;
}



SOCKET Connect(URL* url, DWORD maxAttempts)
{	
	ConnectionData connData;	
	Socket::_initialize(&connData, url);

	for(int i=0; i<maxAttempts; i++) 
	{
		HANDLE hThread = (HANDLE)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ConnectThread, &connData, NULL, 0);
		if((long)WaitForSingleObject(hThread, 10000) == WAIT_TIMEOUT)
		{
			shutdown(connData.Socket, 2);
			TerminateThread(hThread, 1);
		}
		DWORD exitStatus;
		BOOL res = (BOOL)GetExitCodeThread(hThread, &exitStatus);

		if(res && exitStatus == 0)
		return connData.Socket;
	}
	
	return -1;
}



char* HTTP::RecvResponse( SOCKET Socket, LPDWORD dwRecved )
{
	DWORD dwLen = 0;
	char *buffer = NULL;
	int szRecv = 0;

	char *p = (char*)Memory::_alloc( 1024 );
	if (p == NULL)
		return NULL;
	
	//To Do: Execute _recv in a seperate thread
	//The function blocks until all data is recieved
	while ((szRecv = (int)Socket::_recv( Socket, p)) != 0)
	{
		if (szRecv == -1)
			break;		

		if (buffer == NULL)
		{
			buffer = (char*)Memory::_alloc( szRecv + 1 );
			if ( buffer == NULL )
				return NULL;
		}
        //When receiving on a socket, the exact size of	data cannot not be predicated
        //In this, realloc can be used to increase the size of the buffer 		
		else
		{
			char *p = (char*)realloc( buffer, dwLen + szRecv + 1 );						
			if (p == NULL)
			   return NULL;	
			buffer = p;
		}
		Memory::_memcpy( buffer + dwLen, p, szRecv );
		dwLen += szRecv;
	}
	free(p);

	if(dwLen > 0)
	{
		*dwRecved = dwLen;
		return buffer;
	}
	free(buffer);	
	
	return NULL;
}

	
	
DWORD HTTP::ParseResponse(char** buffer, DWORD dwLen1)
{
	DWORD offset = 0;
	DWORD dwLen = dwLen1;

	for ( DWORD i = 0; i < dwLen; i++ )
	{
		if((*buffer)[i] == '\r' && (*buffer)[i + 1] == '\n' &&  (*buffer)[i + 2] == '\r' &&  (*buffer)[i + 3] == '\n'    )
		{
			offset = i + 4;
			break;
		}
	}
	if ( offset > 0 )
	{
		*buffer += offset;
		dwLen  -= offset;		     
	}

	return dwLen;
}


bool HTTP::DownloadInMemory( unsigned char** pBuffer, LPDWORD szBuffer )
{
	URL url;
	DWORD dwUserSize = 1024, dwLen = 0;;
	HTTPHEADERS httpGET;	
	char* header = NULL;	
	
    if (!Socket::_init())
		return false;		
    
	url.host = "localhost";
	url.port = 80;
	SOCKET Socket = Connect(&url,3);
    if (Socket == -1)
		return false;	
		
	GETRequestHeaders(&httpGET);	
	FormatHeaders(&header, httpGET);
	
	bool ok = Socket::_send( Socket, header, strlen(header));
	char *buffer = RecvResponse( Socket, &dwLen );	
	closesocket(Socket);	
	printf("Received Bytes: %u\n",dwLen);
	
	if (dwLen > 0)	
	{
      DWORD sz  = ParseResponse(&buffer, dwLen);
   	  *pBuffer  = (unsigned char*)buffer;
	  *szBuffer = sz;	  
	  return true;
    } 
	
	return  false;		
}

