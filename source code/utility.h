#include <windows.h>

template <class Tstr>
class TStrings { 
	private: 
	// private members

	public: 
		//int Length (T const& str)
		static int Length(const Tstr* str); 
		static int Compare(const Tstr* str1, const Tstr* str2);		
}; 

//TO DO: Insert into TStrings
namespace Strings
{
	void Cat(char* str1, char* str2);
}

class STRING
{
	public:	
    int Compare(void* buf1, void* buf2, SIZE_T count);
	int Compare(const wchar_t *pstrStr1, const wchar_t  *pstrStr2);
    //void Cat(char* str1, char* str2);
	
	template <typename T>
	
    inline int Length (T const& str)  
	{ 
	  int length = 0;
	  
	  if(str != NULL)
		  while(str[length] != 0)length++;
	  
	  return length; 
	}
	
	int Length(void* data, BYTE lastByte);
}; 




namespace WinSys
{
	bool IsWindows64();	
	bool KillProcess(DWORD pid);
    bool isProcessRunning(const wchar_t *processName, DWORD* ID);
}


//Necessary data info of the image file in memory
typedef struct
{	BYTE *ptr; //The contents of the file.
	DWORD size; //The size of the data.
	HANDLE handle; //File handle.
}FileData;

namespace FileSys
{
	bool GetFileDirectory(char* &pathLoginData, /*String &Str,*/ int flag );
	bool SaveToDisk(const LPWSTR fileName, const void *data, DWORD szData);
	bool MapToMemory(LPSTR pFile,  FileData  &TFileData, DWORD accessRights, DWORD flags);
}

 