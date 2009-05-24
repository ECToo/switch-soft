// BookmarkParser.cpp

#include "Precompile.h"
#include "BookmarkTree.h"
#include "BookmarkParser.h"





// ============================================================================
//  BookmarkParser
// ============================================================================
BookmarkParser::BookmarkParser(BytesType& Bytes) 
: Bytes(Bytes), RootItem(NULL), BookmarkCount(0), GroupCount(0)
{
	BytesTypeIt it = Bytes.begin();

	BookmarkTag tag;
	BookmarkItem* lastItem = NULL;
	BookmarkTag* PendingTag = NULL;

	while(it != Bytes.end())
	{
		switch(*it)
		{
		case '<':
			{
				// Interpret last tag
				// Here because we need to parse the text following it first
				if( PendingTag )
				{
					if( !PendingTag->bEnd )
					{
						BookmarkItem* item = NULL;

						if( strncmp("H1", PendingTag->Name.ptr(), 2) == 0 )
						{
							item = new BookmarkItem(PendingTag, BookmarkItem::IT_Group);
							RootItem = item;
							FolderStack.push_back(item);
						}
						else if( strncmp("H3", PendingTag->Name.ptr(), 2) == 0 )
						{
							item = new BookmarkItem(PendingTag, BookmarkItem::IT_Group);
							FolderStack.back()->AddChild(item);
							FolderStack.push_back(item);
							++GroupCount;
						}
						else if( strncmp("A", PendingTag->Name.ptr(), 1) == 0 )
						{
							item = new BookmarkItem(PendingTag, BookmarkItem::IT_Bookmark);
							FolderStack.back()->AddChild(item);
							++BookmarkCount;
						}
						else if( strncmp("HR", PendingTag->Name.ptr(), 2) == 0 )
						{
							item = new BookmarkItem(PendingTag, BookmarkItem::IT_Separator);
							FolderStack.back()->AddChild(item);
						}
						else if( strncmp("DD", PendingTag->Name.ptr(), 2) == 0 )
						{
							if( lastItem )
								lastItem->Description = PendingTag->Text;
						}

						if( item )
						{
							lastItem = item;
						}
					}
					else
					{
						if( strncmp("DL", PendingTag->Name.ptr(), 2) == 0 )
						{
							FolderStack.pop_back();
						}
					}

					PendingTag = NULL;
				}

				// Parse possible tag
				tag = BookmarkTag();
				if( ParseTag(++it, tag) )
				{
					TagList.push_back(new BookmarkTag(tag));
					PendingTag = TagList.back();
				}
				break;
			}

		default:
			{
				// Parse text
				BytesTypeIt text = it;
				ParseText(text, it);
				if( PendingTag )
				{
					/*string textstr = string(text,it);
					textstr = trim(textstr,string("\r\n\t "));
					if( !textstr.empty() )
					{
						//cout << "" << textstr << "" << endl; 
					}*/
					PendingTag->Text = LazyString(text,it);
				}
				break;
			}
		}
	}

	cout << " " << BookmarkCount << " bookmarks" << endl;
	cout << " " << GroupCount << " folders" << endl;

	/*cout << endl;
	cout << "TREE:" << endl;
	if( RootItem )
		RootItem->PrintItem();*/
}

BookmarkParser::~BookmarkParser()
{
	//for_each(Items.begin(), Items.end(), DeleteObject());
}

bool BookmarkParser::ParseTag(BytesTypeIt& it, BookmarkTag& tag)
{
	bool bFinished = false;

	// Ending tag
	tag.bEnd = *it == '/';

	// Handle comment
	if( *it == '!' )
	{
		BytesTypeIt comment = it;
		bFinished = ParseTagComment(comment, it);
		//cout << "<" << string(comment, it) << ">" << endl; 
		if( bFinished )
			return false;
	}

	// Handle name
	{
		BytesTypeIt name = it;
		bFinished = ParseTagName(name, it);
		tag.Name = LazyString(name, it);
		//cout << "<" << tag.Name << ">" << endl; 
		if( bFinished )
			return true;
	}

	// Handle attributes
	while(++it != Bytes.end())
	{
		BytesTypeIt name = it;
		bFinished = ParseTagAttribName(name, it);
		LazyString namestr = LazyString(name, it);
		if( bFinished )
			return true;

		BytesTypeIt value = ++it;
		bFinished = ParseTagAttribValue(value, it);
		LazyString valuestr = LazyString(value, it);
		tag.Attribs.push_back(AttribType(namestr, valuestr));
		//cout << "\t" << namestr << "=" << valuestr << "" << endl; 
		if( bFinished )
			return true;
	}

	// End of file while in tag
	return false;
}

bool BookmarkParser::ParseTagComment(BytesTypeIt& begin, BytesTypeIt& it)
{
	for(it; it!=Bytes.end(); ++it)
	{
		if( *it == '>' )
		{
			// End of tag
			return true;
		}
	}

	// End of file
	return true;
}

bool BookmarkParser::ParseTagName(BytesTypeIt& begin, BytesTypeIt& it)
{
	for(it; it!=Bytes.end(); ++it)
	{		
		if( *it == '>' )
		{
			// End of tag
			return true;
		}
		else if( begin == it )
		{
			// Skip leading chars
			if( *it == '/' )
			{
				++begin;
			}
		}
		else if( *it == ' ' )
		{
			// End of name
			return false;
		}
	}

	// End of file
	return true;
}

bool BookmarkParser::ParseTagAttribName(BytesTypeIt& begin, BytesTypeIt& it)
{
	for(it; it!=Bytes.end(); ++it)
	{		
		if( *it == '>' )
		{
			// End of tag
			return true;
		}
		else if( begin == it )
		{
			// Skip leading chars
			if( *it == ' ' )
			{
				++begin;
			}
		}
		else if( *it == '=' )
		{
			// End of name
			return false;
		}
	}

	// End of file
	return true;
}

bool BookmarkParser::ParseTagAttribValue(BytesTypeIt& begin, BytesTypeIt& it)
{
	for(it; it!=Bytes.end(); ++it)
	{		
		if( *it == '>' )
		{
			// End of tag
			return true;
		}
		else if( begin == it )
		{
			// Skip leading chars
			if( *it == '=' || *it == '\"' )
			{
				++begin;
			}
		}
		else if( *it == '\"' )
		{
			// End of value
			return false;
		}
	}

	// End of file
	return true;
}

void BookmarkParser::ParseText(BytesTypeIt& begin, BytesTypeIt& it)
{
	for(it; it!=Bytes.end(); ++it)
	{
		if( *it == '<' )
		{
			break;
		}
		else if( begin == it )
		{
			// Skip leading chars
			if( *it == '>' )
			{
				++begin;
			}
		}
	}
}

