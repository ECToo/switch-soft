#pragma once

class BookmarkDB
{
	typedef hash_map<string, BookmarkItem*> LinkMapType;
	typedef pair<string, BookmarkItem*> LinkMapPair;
	typedef LinkMapType::iterator LinkMapTypeIt;
	LinkMapType LinkMap;
	int Duplicates;

public:
	BookmarkDB(void);
	~BookmarkDB(void);

public:
	bool AddTree(BookmarkItem* item);
	BookmarkItem* MergeTree(BookmarkItem* base, BookmarkItem* item);
};
