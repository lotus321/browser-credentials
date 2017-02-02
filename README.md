## A Native Windows Password Recovery Tool for Chrome

### Introduction

Modern Web Browsers give users the option of saving Login credentials when they visit websites that require a username and password combination to log in. 

The Chrome browser stores these credentials in a file called Login Data. This file, which on the Windows OS is located in the user's AppData folder, stores the data in a database format. The Login ID (username or email) is stored in plain text and the password is encrypted as a 230 byte binary BLOB. 

To extract and decrypt this password BLOB, two approaches will be employed; using a database libray and raw binary parsing.


 
### Parsing with Database Library
Using the Sqlite3 DLL is the recommended approach as it simplifies the process of parsing the login credentials and runs on both 32 & 64 bits Windows.

To reduce the size of the executable file, the dll library is not staticaly linked at compilation. Instaed, the library is downloaded from a remote server and dynamically loaded into the process's memory space without saving the file to the disk. This is necessary becasue certain security programs, AVs in particular, will prevent a program from saving an executable file.
By executing the DLL in-memory, the program cannot depend on the windows loader (and certainly can't use the `LoadLibrary` WinAPI).

To function as intended, the program will have to manually allocate memory (with executable rights), resolve all external dependences, perform base relocation, locate the entry point and finally pass control to the DLL.


### Raw Binary Parsing
The approach has the advantage of removing the dependence on the database library file by manually parsing the login credentials.

As stated earlier only the passwords are encrypted, the login IDs are stored in plain text.  
The file itself is very large, only a smaller section corresponding to one credential entry is shown. 

After analysing the binaries with an the hex editor, it can be observed that  each entry of  credentials for a particulr website is serapated by rows of null bytes ```0x00```, and the password BLOB starts with a unique sequence of bytes.


![hex](https://cloud.githubusercontent.com/assets/22178295/22538428/61ef35b4-e966-11e6-9590-3c8f07224fa4.PNG)

To help with the visual analysis of the hex data, the data corresponding to the website name is in green, data for the email/username in blue, and that of the encrypted password blob in red.

Ok, now looking at ascii characters, the website name and login name can clearly be seen as http//:www.linkedin.com and login ID:m.mike@yahoo.com, respectively. This indicates that chrome does not encrypt these credentials.
Again, refering to the ascii characters, shortly after the website name, are encrypted or put it simply "unredable" characters. Looking at offset ```00005510```, these characters are represented by ```OÂ—ë....tý.¿ÒÖÞI```, that is the start of the encrypted passsord.

Through visual inspection with a Hex Editor, it can be observed found the sequence of bytes ```08 0D 0D 0D 08 08``` which are located at offset offset ```000054A0``` indicates the start to each unique credential entry. 

The index to the password blob can is at the start of the "unreadable" ascii characters, shown at offset ```0005510``` . The corresponding  sequence of 16  bytes, ```[01 00 00 00 D0 8C 9D DF 01 15 D1 11 8C 7A 00 C0 ]``` indicates the start index to the password BLOB.
 After the index to this sequence is located, the next 260 bytes of the data are then decrypted using the```CryptUnprotectData```  WinAPI.


### Custom Memory Functions
The logic behind the implementation of custom functions (in the module.c file), which are, independent of LIBC is that C runtime heap functions ( malloc, realloc, etc ) are module dependant, which means that if you call for example malloc() in code from one module (i.e. a DLL), then you should call free() within code of the same module or you could suffer some pretty bad heap corruption.

It is necessary to avoid the use of C runtime heap functions because the process will be using functions from modules which are maunualy loaded by the process. 

Using Some windows APIs for example HeapAlloc() with GetProcessHeap() instead of malloc(), including overloading new and delete operators to make use of such, allow you to pass dynamically allocated objects between modules and not have to worry about memory corruption if memory is allocated in code of one module and freed in code of another module once the pointer to a block of memory has been passed across to an external module.	


### Current Issues 
When Chrome is running it blocks access to some sensitive folders, in this case the User Data folder which is located in the AppData folder. 

A basic solution that can be used to bypass the browser's protection mechanism is the termination of the running Chrome process. This brute force approach is not ideal because it can cause a browser to lose usaved data.

A better solution, and the future implemetation in the works, is code injection. With this technique, executable code can be injected in a running chrome instance. This injected code will have the privelages to access the browser's folders and internal data, without restrictions.	

### Compiling The Source Code
The source code is compiled with Developer Command Prompt for Visual Studio 2015
using the command `cl /EHsc ....cpp ....cpp`,  where `....cpp` are the input C/C++ source files.

###### Snapshot On Windows 7 (64-bit)
![compile](https://cloud.githubusercontent.com/assets/22178295/22188498/e12a4d5a-e169-11e6-8cfe-5366c320c29b.PNG).

### Testing
Tested on Windows 7 and 10

-------------------------------------------------------------------------------------------------------------------------------
