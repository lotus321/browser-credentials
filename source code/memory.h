#include "preprocessor.h"

/// <Summary>
/// ASM_INTERNAL:: For functions declared with the naked attribute, 
/// the compiler generates code without prolog and epilog code.
/// This enables more flexibility when manipulating data on the stack
/// </summary>
void  __stdcall _copy(void *dst, void *src, size_t size);

/// <Summary>
/// </summary>
void  _copy2(void *dst, const void *csrc, size_t size);


/// <Summary>
/// </summary>
int   _size(void* src, uint8_t lastByte);


/// <Summary>
/// </summary>
int   _compare(void* buf1, void* buf2, size_t count);


///<Summary>
///</summary> 
void  _allocate_on_stack(size_t size);


namespace Memory
{
	void Init(void);//TO DO
	
	///<Summary>
    ///</summary> 
	void _memcpy(void* dst, void * src, size_t size);
	
	///<Summary>
    ///</summary> 
	bool _realloc(void *old, size_t size);
	
	///<Summary>
    ///</summary> 
	void* _alloc(size_t size);
	
	///<Summary>
    ///</summary> 
	void _memcpy2(void *dst, const void *src, size_t size);
	
	///<Summary>
    ///</summary> 
	int _memcmp(uint8_t* ptr1, uint8_t* ptr2, size_t sz);
	
	///<Summary>
    ///</summary> 
	int _Length(void* data, uint8_t lastByte);
	
	void _free(void* p);

}