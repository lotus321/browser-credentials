## A Native Windows Password Recovery Tool for Chrome

### Introduction

Most modern Web Browsers give users the option of saving Login details when they visit website that require a username and password to log in. 
The Chrome browser stores login information of websites in a file called Login Data. This file, which is located in a user's AppData folder, stores the login crendentils in a database formart. The user's email/username is stored in plain text,and the password is encrypted as a 230 byte binary BLOB 
The project will present two approach to parsing database file, where Chrome stores the credentials, for the purpose of decrypting the login passwords.

 
### Parsing with Database Library
Using the Sqlite3 DLL is the recommenden approach as it simplifies the process of parsing the login credentials and runs on both 32 & 64 bits Windows.
The library fetched from a remote server and loaded directly into a process's memory space. This is necessary becasue  certain security programs, AVs in particular, will prevent a program from saving an executable file.
By executing the DLL in-memory, the program cannot depend on the windows loader (and certainly can't use the `LoadLibrary` WinAPI) in loading the executable file.
To achieve the above, the program will haave to manually allocate memory (with executable rights), resolve all external dependences, perform base relocation, locate the entry point and finally pass control to the DLL.


### Raw Binary Parsing
To avoid statically linking a large library file, the second approach parses the login credentials manually.
As stated earlier, the username\email is stored in plain. 
Using a Hex Editor, the Login Data file is analysed
In the second approach, the dependence on the Sqlite3 (database) DLL, and manually parsed the database file to locate the password BLOB, and copy the corresponding 260 bytes of data But why go through all this trouble? The second approach was more about challenging myself and playing around with Assembler code.



### Why Implement Custom Memory Functions
With the second approach, I implemented custom LIBC functions. The logic behind the implementation of custom functions (in the module.c file), which are, independent of LIBC is that C runtime heap functions ( malloc, realloc, etc ) are module dependant, which means that if you call for example malloc() in code from one module (i.e. a DLL), then you should call free() within code of the same module or you could suffer some pretty bad heap corruption  
Using Some windows APIs for example HeapAlloc() with GetProcessHeap() instead of malloc(), including overloading new and delete operators to make use of such, allow you to pass dynamically allocated objects between modules and not have to worry about memory corruption if memory is allocated in code of one module and freed in code of another module once the pointer to a block of memory has been passed across to an external module.	


### Current Issues 
When Chrome is running, I noticed that to blocks access to some sensitive folders, in this case the User Data folder which is located in the AppData folder. This means we can't access the file which stores the login credentials, if chrome is running. The temporary solutions I have used in this project is to terminate the running chrome process. This brute force approach is not ideal because it can cause a browser to lose important data if it is abruptly terminated. A much better approach would is code injection. With this technique, we can inject our executable code in a running chrome instance and access the browser's folders without restriction.	

### Compiling The Source Code
The source code is compiled with Developer Command Prompt for Visual Studio 2015
using the command `cl /EHsc ....cpp ....cpp`,  where `....cpp` are the input C/C++ source files.

###### Snapshot On Windows 7 (64-bit)
![compile](https://cloud.githubusercontent.com/assets/22178295/22188498/e12a4d5a-e169-11e6-8cfe-5366c320c29b.PNG).

### Testing
Tested on Windows 7 and 10

-------------------------------------------------------------------------------------------------------------------------------
