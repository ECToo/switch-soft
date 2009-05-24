//	:: UnrealScript TextBuffer Exporter
//	:: Copyright(C) 2004 Switch` switch@thieveryut.com

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;


typedef unsigned long	DWORD;
typedef unsigned short	WORD;
typedef unsigned char	BYTE;

#define LOWORD(x)	((WORD) (x)) 
#define HIWORD(x)	((WORD) (((DWORD) (x) >> 16) & 0xFFFF)) 


#define PKG_Signature		0x9E2A83C1

typedef struct
{
	DWORD	Signature;
	DWORD	PackageVersion;
	DWORD	PackageFlags;
	DWORD	NameCount;
	DWORD	NameOffset;
	DWORD	ExportCount;
	DWORD	ExportOffset;
	DWORD	ImportCount;
	DWORD	ImportOffset;
}PKG_Header;

typedef struct
{
	DWORD	Class;
	DWORD	Super;
	DWORD	Package;
	DWORD	ObjectName;
	DWORD	ObjectFlags;
	DWORD	SerialSize;
	DWORD	SerialOffset;
}ExportItem;

typedef struct
{
	char*	Name;
	DWORD	Flags;
}NameItem;



string  itos(int num);
int ReadIndex( fstream &File );
NameItem* ReadNameTable( fstream &File, DWORD &NameOffset, DWORD &NameCount, DWORD &TextBufferIndex );
ExportItem* ReadExportTable( fstream &File, DWORD &ExportOffset, DWORD &ExportCount );
char* ReadTextBuffer( fstream &File, DWORD Offset );
void ProcessFile( string FName );



