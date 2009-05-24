// BookmarkParser.h
#pragma once

#include "BookmarkTree.h"



// ============================================================================
//  BookmarkParser
// ============================================================================
class BookmarkParser
{
public:
	BytesType&	Bytes;
	vector<BookmarkTag*> TagList;
	vector<BookmarkItem*> FolderStack;
	BookmarkItem* RootItem;
	int BookmarkCount;
	int GroupCount;

public:
	BookmarkParser(BytesType& Bytes);
	~BookmarkParser();

public:
	bool ParseTag(BytesTypeIt& it, BookmarkTag& tag);
	void ParseText(BytesTypeIt& begin, BytesTypeIt& it);

public:
	bool ParseTagComment(BytesTypeIt& begin, BytesTypeIt& it);
	bool ParseTagName(BytesTypeIt& begin, BytesTypeIt& it);
	bool ParseTagAttribName(BytesTypeIt& begin, BytesTypeIt& it);
	bool ParseTagAttribValue(BytesTypeIt& begin, BytesTypeIt& it);


};

// EOF