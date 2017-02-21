
//  TO DO
// Implement Memory::_memset 
// Implement Memory::_realloc

#include <windows.h>

#include "memory.h"


void __declspec(naked) __stdcall  _copy(void* dst, void* src, size_t sz)
{
#ifdef _WIN32
	__asm
	{
		// ?? pusha - push all register, any advantages?
		push esi     // preserve the register contents
		push edi
		push ecx
        
		//Function paramaters on the (Procedure) Stack are placed in a Last In First Out(LIFO)
	    //Basically they're placed on the stack in the inverse order
		mov esi, DWORD PTR[esp + 0x8 + 0xC]
		mov edi, DWORD PTR[esp + 0x4 + 0xC]
		mov ecx, DWORD PTR[esp + 0xC + 0xC]
		rep movsb

		// ?? same with popa?
		pop ecx     // restore the register contents
		pop edi
		pop esi
		
		ret 0xC
	};
	#else ifdef _WIN64	
		for(reg SIZE_T count = 0; count < sz; count++)
		{
			((unsigned char*)dst)[count] = ((unsigned char*)src)[count];
			if(count == 0)
				count = 0; 
		}
	#endif  
	}


void  _copy2(void *dst, const void *src, size_t sz)
{
		
	__asm
	{		        
		mov esi, DWORD PTR[src]     //Source Index
		mov edi, DWORD PTR[dst]     //Destination Index
		mov ecx, DWORD PTR[sz]      //Counter,size/number of iterations.
		
		rep movsb                   //Repeat (moving)copying Byte++ from ESI++ to EDI++
                                    	//untill ECX-- gets to zero
	};
}


int _size(void* buffer, unsigned char character)
{  
#ifdef _WIN32
	__asm
	{
		mov edi, int PTR[buffer]   // move the pointer the start of the string into destination index register
		sub	ecx, ecx               // ECX = 0
		mov al, character          // The byte which indicates when to stop 	
		not	ecx                    // ECX = -1(for signed), or 4,294,967,295
		cld                        // clear the direction flag, the will increment the value in counter (ecx) register by 1 with each itertion.

		repne	scasb              // compares the byte in AL with the byte at [ES:DI] or [ES:EDI], and sets the flags accordingly. 
		not	ecx                    // ECX =32^2 - ECX
		mov eax, ecx               // the calculated string size in the return variable, EAX register.
	};
#   else ifdef _WIN64	
	
{
//  FIX - Need to set size for the itereation limit
	reg SIZE_T count = 0;
	while(buffer[count] != character)
	    count++;

	return count+=1;
  
}
#   endif
}



int  _compare(void* buffer1, void* buffer2, size_t count)
{   
#ifdef _WIN32
	__asm
	{	 
	        cld	                 // Clear the direction reg
		                     // During iterations, this will increment the value in the ECX regfister
		
		xor eax, eax         // Set EAX to Zero
		                     // Not sure which is faster? :move eax, 0 | sub eax, eax
		
		mov esi, [buffer1]   // Source Index (ESI), used for string manipulation. Points to the source                      
		mov edi, [buffer2]   // Destination Index                     
		mov ecx, [count]     // The size (number) of interations. With each iterations the value in ECX is decreased by a 1:byte?
		                 		
		repe  cmps           // repeat while equall if equall all bytes compared, sets ecx to zero
 		mov eax, ecx         // Mov the result into the Accumulator reg. Which hold results from arithmetic computations,
                                     // return values, among other things.					 
	};
#else ifdef _WIN64
	reg BYTE p1, p2;
	for(reg SIZE_T i = 0; i < size; i++)
	{
		p1 = ((unsigned char*)buffer1)[i];
		p2 = ((unsigned char*)buffer2)[i];

	if(buffer1 != buffer2)
		return (int)(p1 - p2);
	}
	
	return 0;
#endif
}

 

void __declspec(naked) __stdcall _set(void *buffer, char character, SIZE_T size)
{
#ifdef _WIN32
	__asm
	{
		push eax   
		push edi
		push ecx

		mov al,  byte ptr [esp + 0x8 + 0xC]
		mov edi, dword ptr[esp + 0x4 + 0xC]
		mov ecx, dword ptr[esp + 0xC + 0xC]
		
		rep stosb
        
		pop ecx
		pop edi
		pop eax
		ret 0xC
	};
#else
		reg SIZE_T count = size;
		while(count--)
		((char *)buffer)[count] = character;
#endif
}

//TO DO
void  _temp_stack_alloc(size_t sz)
{
	//TO IMPLEMENT
	
}

////////////////////////////////////////////////////////////////////////////////////////////////




void Memory::_memcpy(void* dst, void * src, size_t sz)
{
	#ifdef CUSTOM_LIBC	
		memcpy(dst, src, sz);
	#else
	  	  _copy(dst, src, sz);	
	#endif
		return;	
}	


void* Memory::_alloc (size_t szBuffer)
{
		unsigned char* p = NULL;
	#ifdef CUSTOM_LIBC
		p = (unsigned char*)malloc(szBuffer);	
	#else
		p = (unsigned char*)HeapAlloc((GetProcessHeap()), HEAP_ZERO_MEMORY, szBuffer);
	#endif	
	
		return((p == NULL)? NULL: p);
}


void Memory::_memcpy2(void *dst, const void *src, size_t sz)
{	
	_copy2(dst, src, sz);	
	return;	
}


int Memory::_Length(void* data, uint8_t lastByte)
{  
	int size = 0;	
	size = _size(data,lastByte);	
	return size;
}


bool Memory::_realloc(void *prev, size_t sz)
{    
	reg void* p; //Store in register for quick access

	if(*(uint8_t**)prev == NULL)
		p =  HeapAlloc((GetProcessHeap()), HEAP_ZERO_MEMORY, sz);
	else 
		p =  HeapReAlloc((GetProcessHeap()), HEAP_ZERO_MEMORY, *(uint8_t **)prev, sz);

	if(p == NULL)return false;

	*(uint8_t**)prev =(uint8_t *)p;   

	return true;
}


int Memory::_memcmp(uint8_t* buffer1, uint8_t* buffer2, size_t sz)
{
    if(!sz)
        return(0);

    while(--sz && *buffer1==*buffer2 ) 
	{
        buffer1++;
        buffer2++;
    }
    return(*buffer1 - *buffer2);
} 
	
void Memory::_free(void* p)
{
	if(p)
	{
#ifdef CUSTOM_LIBC	
		free(p);
#else
        HeapFree((GetProcessHeap()), 0, p);	
#endif
	}
	return;	
}	



