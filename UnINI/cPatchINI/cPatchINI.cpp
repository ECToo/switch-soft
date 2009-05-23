// ============================================================================
//  cPatcIINI.cpp : Defines the entry point for the console application.
// ============================================================================

#include "stdafx.h"
#include "PatchINI\PatchINIStatic.h"


// ============================================================================
//  main
// ============================================================================
int main(int argc, const char* argv[])
{
	if( argc != 4 )
	{
		cout << "usage: <merge file> <base file> <result file>" << endl;
		return 1;
	}

	int result = 1;
	char errorstr[1024];

	result = PatchINIMerge(argv[1],argv[2],argv[3],errorstr,sizeof(errorstr));
	if( result != 0 )
		cout << errorstr << endl;

	return result;
}


// ============================================================================
//  EOF
// ============================================================================