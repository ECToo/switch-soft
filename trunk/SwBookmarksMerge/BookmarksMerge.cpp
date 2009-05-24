// BookmarksMerge.cpp


#include "Precompile.h"
#include "BookmarksMerge.h"
#include "BookmarkFile.h"
#include "BookmarkParser.h"
#include "BookmarkDB.h"
#include "BookmarkWriter.h"


int wmain(int argc, wchar_t* argv[])
{
	try
	{
		if( argc != 4 )
		{
			throw swException(wstring_make() << "usage: <merge file> <base file> <result file>" );
		}

		wstring MergeFileName = argv[1];
		wstring BaseFileName = argv[2];
		wstring ResultFileName = argv[3];

		cout << endl << "Loading: " << toNarrowString(MergeFileName.c_str()).c_str() << endl;
		BookmarkFile MergeFile(MergeFileName);

		cout << endl << "Loading: " << toNarrowString(BaseFileName.c_str()) << endl;
		BookmarkFile BaseFile(BaseFileName);

		cout << endl << "Parsing: " << toNarrowString(MergeFileName.c_str()) << endl;
		BookmarkParser MergeParsed(MergeFile.GetData());

		cout << endl << "Parsing: " << toNarrowString(BaseFileName.c_str()) << endl;
		BookmarkParser BaseParsed(BaseFile.GetData());

		cout << endl << "Merging..." << endl;
		BookmarkDB DB;
		//DB.AddTree(MergeParsed.RootItem);
		//DB.AddTree(BaseParsed.RootItem);
		DB.MergeTree(MergeParsed.RootItem, BaseParsed.RootItem);

		cout << endl << "Writing: " << toNarrowString(ResultFileName.c_str()) << endl;
		BookmarkWriter writer(ResultFileName, MergeParsed.RootItem);
		writer.WriteFile();


		//wcout << L"merge file: " << MergeFileName << endl;
		//wcout << "base file: " << (char*)BaseFileName.c_str() << endl;
		//wcout << "result file: " << (char*)ResultFileName.c_str() << endl;
		//cout << endl;

		return EXIT_SUCCESS;
	}
	catch(swException& e)
	{
		cout << toNarrowString(e.What()) << endl;
	}
	catch(exception& e)
	{
		cout << e.what() << endl;
	}
	catch(...)
	{
		cout << "unknown exception" << endl;
	}

	return EXIT_FAILURE;
}

// EOF