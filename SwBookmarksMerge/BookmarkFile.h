// BookmarkFile.h
#pragma once

class BookmarkFile
{
	wstring					Name;
	ifstream				File;
	streampos				Size;
	vector<char>			Bytes;

public:
	BookmarkFile(wstring FileName);

	vector<char>& GetData()
	{
		return Bytes;
	}

};

// EOF