#pragma once

//SQLITE definitions
#define SQLITE_OK 0
#define SQLITE_ROW 100
#define API
typedef struct sqlite3 sqlite3;
typedef struct sqlite3_stmt sqlite3_stmt;

 
//SQLITE function pointers
typedef int(API *pOpen)(const char *, sqlite3 **);
typedef int(API *pPrepareV2)(sqlite3 *, const char *, int, sqlite3_stmt **, const char **);
typedef int(API *pStep)(sqlite3_stmt *);
typedef const unsigned char *(API *pColumnText)(sqlite3_stmt*, int);
typedef int(API *pFinalize)(sqlite3_stmt *);
typedef int(API *pClose)(sqlite3 *);



namespace Database
{
	void SQLParsing(char *path, HANDLE module);	
	int RawBinaryParsing( unsigned char* input, int inputSize, unsigned char***cryptedPass, char**& login_info1);
}