#pragma once

class BookmarkWriter
{
	wstring		Name;
	ofstream	File;
	BookmarkItem* RootItem;
	string Indent;
	int BookmarkCount;
	int GroupCount;

public:
	BookmarkWriter(wstring Name, BookmarkItem* RootItem);
	~BookmarkWriter(void);
public:
	void WriteFile();
	void WriteItem(BookmarkItem* item);

	void WriteAttributes( BookmarkItem* item );
};
