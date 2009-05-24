// BookmarkFile.cpp


#include "Precompile.h"
#include "BookmarkFile.h"
#include "BookmarkParser.h"

BookmarkFile::BookmarkFile(wstring FileName) : Name(FileName)
{
	// Open file
	File.open(Name.c_str(), ios_base::in | ios_base::binary);
	if( !File )
		throw swException(wstring_make() << "Cannot open file: " << Name);

	// Get file size
	File.seekg(0, ios::end);
	Size = File.tellg();
	File.seekg(0, ios::beg);

	// Read file
	Bytes.reserve(Size);
	Bytes.resize(Size);
	File.read(&Bytes[0], Size);
	if( File.fail() )
		throw swException(wstring_make() << "Failed reading file: " << Name);
	
	// Verify contents
	if( Bytes.size() == 0 )
		throw swException(wstring_make() << "File must not be empty: " << Name);

	cout << " " << Bytes.size() << " bytes" << endl;
}

// EOF