#pragma once

typedef struct
{	
	char* method;
	char* version;
	char* host;
	char* connection;	
}
HTTPHEADERS, *pHTTPHEADERS;


namespace HTTP
{
    bool  DownloadInMemory(unsigned char** lpBuffer, LPDWORD dwSize);
	void  GETRequestHeaders(pHTTPHEADERS http);
	void  FormatHeaders(char** header, HTTPHEADERS http);
	char* RecvResponse( SOCKET Socket, LPDWORD dwRecved);
	DWORD ParseResponse(char** buffer, DWORD dwLen1);
}

#if(0)
//HTTP Headers
typedef struct
{
	char* Host = "Host: net.tutsplus.com";
	char* User_Agent = "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)";
	char* Accept = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
	char* Accept_Language = "Accept-Language: en-us,en;q=0.5";
	/*Accept-Encoding: gzip,deflate
	Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7
	Keep-Alive: 300
	Connection: keep-alive
	Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120
	Pragma: no-cache
	Cache-Control: no-cache*/	
}
#endif