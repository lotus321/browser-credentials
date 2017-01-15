#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

struct ConnectionData
{
	SOCKET Socket;
	sockaddr_in SockAddr;
};

//URL
typedef struct
{
	char* host;      
	char* resource; 
	int   port;		
}
URL, *pURL;

namespace Socket
{
	bool _init(void);

	void _uninit(void);

	bool _initialize(ConnectionData* connData, pURL url);

	bool _connect(ConnectionData* conndata);

	bool _send( SOCKET Socket, const char *Data, int DataLen );

	int _recv(SOCKET Socket, char* TempBuffer);
}

