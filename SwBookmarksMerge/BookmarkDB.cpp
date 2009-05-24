#include "Precompile.h"
#include "BookmarkTree.h"
#include "BookmarkParser.h"
#include "BookmarkDB.h"

BookmarkDB::BookmarkDB(void) : Duplicates(0)
{
}

BookmarkDB::~BookmarkDB(void)
{
}

bool BookmarkDB::AddTree( BookmarkItem* item )
{
	if( item->empty() )
	{
		for( vector<AttribType>::iterator it=item->Attribs.begin(); it!=item->Attribs.end(); ++it )
		{
			if( strncmp("HREF", (*it).first.ptr(), 4) == 0 )
			{
				string name = (*it).second.str();
				transform(name.begin(), name.end(), name.begin(), tolower);

				pair<LinkMapTypeIt, bool> result = LinkMap.insert(LinkMapPair(name,item));
				if( !result.second )
				{
					//cout << ++dup << " " << name << endl;
				}

				return result.second;
			}
		}
	}
	else
	{
		for( vector<BookmarkItem*>::iterator it=item->begin(); it!=item->end(); ++it )
		{
			if( !AddTree(*it) )
			{
				delete *it;
				*it = NULL;
				++Duplicates;
			}
		}
	}
	return true;
}

BookmarkItem* BookmarkDB::MergeTree( BookmarkItem* base, BookmarkItem* item )
{
	AddTree(base);
	cout << " " << Duplicates << " base duplicates" << endl;

	AddTree(item);
	cout << " " << Duplicates << " total duplicates" << endl;

	for( vector<BookmarkItem*>::iterator it=item->begin(); it!=item->end(); ++it )
	{
		base->push_back(*it);
	}

	item->clear();

	//cout << " " << Duplicates << " duplicates" << endl;
	return base;
}