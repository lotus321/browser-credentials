## A Native Windows Password Recovery Tool for Chrome

### Overview 
The Chrome browser stores login information of websites in a file called Login Data. The CryptUnprotectData  WinAPI is then used to decrypt the password BLOB. Note that this API will only decrypt data stored on a local machine.


 
### Decrypting the Password 
In the first approach, I used the Sqlite3 DLL to simplify the parsing of the login credentials from the Login Data file. This approach is the recommended way to parsing database files. 
The only downside to this approach is that I have to fetch the DLL from a remote host, and dynamically link it at runtime.
The DLL is loaded directly in memory, without having to save it to the Disk. This is necessary because certain security programs, AVs in particular, will block the process of saving an executable file to the disk.
By executing the DLL directly in-memory, we cannot depend on the windows loader (and certainly can't use the LoadLibrary WinAPI.
This means that we have to manually allocate memory (with executable rights), resolve all external dependences, perform base relocation, locate the entry point and finally pass control to the DLL.
In the second approach, I removed dependence on the Sqlite3 (database) DLL, and manually parsed the database file to locate the password BLOB, and copy the corresponding 260 bytes of data But why go through all this trouble? The second approach was more about challenging myself and playing around with Assembler code. 


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
