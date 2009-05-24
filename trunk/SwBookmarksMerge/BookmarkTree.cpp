// BookmarkTree.cpp

#include "Precompile.h"
#include "BookmarkTree.h"



// ============================================================================
//  BookmarkItem
// ============================================================================
BookmarkItem* BookmarkItem::AddChild( BookmarkItem* child )
{
	if( child )
	{
		child->Parent = this;
		push_back(child);
	}
	return child;
}

BookmarkItem::BookmarkItem() : Parent(NULL)
{

}

BookmarkItem::BookmarkItem( BookmarkTag* tag, EItemType type ) : Attribs(tag->Attribs), Name(tag->Text), Parent(NULL), Type(type)
{

}

void BookmarkItem::PrintItem()
{
	//cout << size() << "\t";
	BookmarkItem* p = Parent;
	while( p )
	{
		cout << "\t";
		p = p->Parent;
	}

	switch(Type)
	{
	case IT_Group:
		cout << "> " << Name.str(); break;
	case IT_Separator:
		cout << "--------------------------------------------------------------------------" << Name.str(); break;
	case IT_Bookmark:
		cout << "  " << Name.str(); break;
	default:
		cout << "? " << Name.str(); break;
	}

	//cout << " (" << Description << ")";
	cout << endl;

	for( vector<BookmarkItem*>::iterator it=begin(); it!=end(); ++it)
	{
		(*it)->PrintItem();
	}
}

// EOF