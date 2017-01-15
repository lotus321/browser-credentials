

 ---- Why Implement Custom Memory Functions ----
The logic behind the implemetation of custom functions, which are, independent of LIBC is that 
C runtime heap functions ( malloc, realloc, etc ) are module dependant, which means that if
you callfor example malloc() in code from one module (i.e. a DLL), 
then you should call free() within code of the same module or you could suffer 
some pretty bad heap corruption  
Using Some windows APIs for example HeapAlloc() with GetProcessHeap() instead of 
malloc(), including overloading new and delete operators to make use of such, 
allow you to pass dynamically allocated objects between modules and not have to worry about 
memory corruption if memory is allocated in code of one module and freed in code of another module 
once the pointer to a block of memory has been passed across to an external module.	
