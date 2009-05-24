#include "Precompile.h"
#include "BookmarkParser.h"
#include "BookmarkDB.h"
#include "BookmarkWriter.h"

BookmarkWriter::BookmarkWriter(wstring Name, BookmarkItem* RootItem) 
: Name(Name), RootItem(RootItem), BookmarkCount(0), GroupCount(0)
{
	// Open file
	File.open(Name.c_str(), ios_base::out | ios_base::binary | ios_base::trunc);
	if( !File )
		throw swException(wstring_make() << "Cannot write file: " << Name);
}

BookmarkWriter::~BookmarkWriter(void)
{
}

void BookmarkWriter::WriteFile()
{
	File << "<!DOCTYPE NETSCAPE-Bookmark-file-1>" << endl;
	File << "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=UTF-8\">" << endl;
	File << "<TITLE>Bookmarks</TITLE>" << endl;

	WriteItem(RootItem);

	cout << " " << BookmarkCount << " bookmarks" << endl;
	cout << " " << GroupCount << " folders" << endl;

	if( File.fail() )
		throw swException(wstring_make() << "Failed writing file: " << Name);
}

void BookmarkWriter::WriteItem( BookmarkItem* item )
{
	switch (item->Type)
	{
	case BookmarkItem::IT_Bookmark:
		++BookmarkCount;
		File << Indent << "<DT><A";
		WriteAttributes(item);
		File << ">" << item->Name.str() << "</A>" << endl;

		if( !item->Description.empty() )
		{
			string comment = trim(item->Description.str(), string("\r\n\t "));
			if( !comment.empty() )
				File << "<DD>" << comment << endl;
		}
		break;

	case BookmarkItem::IT_Group:
	{
		++GroupCount;
		if( item->Parent )
		{
			File << Indent << "<DT><H3";
			WriteAttributes(item);
			File << ">" << item->Name.str() << "</H3>" << endl;
		}
		else
		{
			File << Indent << "<H1>" << item->Name.str() << "</H1>" << endl;
		}

		if( !item->Description.empty() )
		{
			string comment = trim(item->Description.str(), string("\r\n\t "));
			if( !comment.empty() )
				File << "<DD>" << comment << endl;
		}

		File << Indent << "<DL><p>" << endl;
		Indent.append(4,' ');
		for( vector<BookmarkItem*>::iterator it=item->begin(); it!=item->end(); ++it )
		{
			if( *it )
			{
				WriteItem(*it);
			}
		}
		Indent.resize(Indent.size()-4);
		File << Indent << "</DL><p>" << endl;
		break;

	}

	case BookmarkItem::IT_Separator:
		File << Indent << "<HR>" << endl;
		break;
	}
}

void BookmarkWriter::WriteAttributes( BookmarkItem* item )
{
	for( vector<AttribType>::iterator it=item->Attribs.begin(); it!=item->Attribs.end(); ++it )
	{
		File << " " << (*it).first.str() << "=\"" << (*it).second.str() << "\""; 
	}
}