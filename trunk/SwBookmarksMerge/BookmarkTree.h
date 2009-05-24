// BookmarkTree.h
#pragma once


// ============================================================================
//  BookmarkTag
// ============================================================================
struct BookmarkTag
{
	vector<AttribType> Attribs;
	LazyString Name;
	LazyString Text;
	bool bEnd;
};


// ============================================================================
//  BookmarkItem
// ============================================================================
struct BookmarkItem : public vector<BookmarkItem*>
{
public:
	enum EItemType {
		IT_Bookmark,
		IT_Group,
		IT_Separator,
		IT_MAX
	};

public:
	vector<AttribType> Attribs;
	LazyString Name;
	LazyString Description;
	EItemType Type;
	BookmarkItem* Parent;

	BookmarkItem();
	BookmarkItem(BookmarkTag* tag, EItemType type);
	BookmarkItem* AddChild(BookmarkItem* child);

	void PrintItem();
};


// EOF