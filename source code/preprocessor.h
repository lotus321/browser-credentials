/*Preprocessor*/

//        Proces name for the chrome browser
#define   CHROME_EXE L"chrome.exe"



//        Path to the 'login Data'
//        This file holds all the necessary data for the required browser credentials
#define   CHROME_LOGIN_DATA  "\\Google\\Chrome\\User Data\\Default\\Login Data"



//        x86 Inline Assemler, if not x64 relay on C (pointers)
#define   _WIN32  1



//        __stdcall - The __stdcall calling convention is used to call Win32 API functions. 
//        The callee cleans the stack, so the compiler makes vararg functions __cdecl. 
//        Functions that use this calling convention require a function prototype.
//        Source ~ MSDN  

//        -- naked --
//        For functions declared with the naked attribute, the compiler generates code without prolog and epilog code.
//        This feature is useful when the need arise to write your own prolog/epilog code sequences using inline assembler code. 
//        Naked functions are particularly useful in writing virtual device drivers. 
//        Note that the naked attribute is only valid on x86 and ARM, and is not available on x64.      
#ifdef    _WIN64
#   	  define   __stdcall 
#   	  define   __declspec(naked)   
#else
#  		  define   __stdcall  __stdcall
#  		  define   __declspec(naked) __declspec(naked)
#endif



//        register - this stores variables in the register
//        as opposed to storing in the ram
//        compilier dependent, only implementend on x32 ??
#define   reg  register  


//        Data types - When you are in Rome, do what the Romans do!
//        The ideal is to use Microsoft defined data types while compiling for windows
//        And try to be as compatible as much when compiling for cross platform software
#ifndef   uint8_t
typedef   unsigned __int8  uint8_t; 
#endif		  

