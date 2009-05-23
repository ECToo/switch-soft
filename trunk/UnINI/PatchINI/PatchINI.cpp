// ============================================================================
//  PatchINI.cpp : Defines the entry point for the console application.
// ============================================================================

#include "stdafx.h"
#include "PatchINI.h"
#include "PatchINIStatic.h"


// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>



// ============================================================================
//  UnINIProperty
// ============================================================================

void UnINIProperty::DumpDebug( ostream* stream )
{
	*stream << Text << endl;
}

void UnINIProperty::Merge( UnINISection* to )
{
	if(!IsValid() )
		return;

	//cout << "Merge " << Text << " " << Cmd  << endl;

	if( Cmd == "" )
	{
		UnINIProperty* prop = to->Find(Property);
		if( prop )
		{
			prop->SetValue(Value);
		}
		else
		{
			to->Append(Property,Value);
		}
	}
	else if( Cmd == "+" )
	{
		if( !to->Find(Property,Value) )
		{
			to->Append(Property,Value);
		}
	}
	else if( Cmd == "-" )
	{
		to->Remove(Property,Value);
	}
	else if( Cmd == "!" )
	{
		to->Remove(Property);
	}
	else if( Cmd == "." )
	{
		to->Append(Property,Value);
	}
}

void UnINIProperty::Write( ofstream* stream )
{
	*stream << Text << endl;
}


// ============================================================================
//  UnINISection
// ============================================================================

void UnINISection::DumpDebug( ostream* stream )
{
	for_each( LineData.begin(), LineData.end(), bind2nd(mem_fun1_ref(&UnINIProperty::DumpDebug),stream) );
	*stream << endl;
}

UnINIProperty& UnINISection::PushLine( const UnINIProperty& prop )
{
	LineData.push_back(prop);
	return LineData.back();
}

UnINIProperty* UnINISection::Find( const string& text )
{
	UnINIPropertyList::iterator it = find_if( LineData.begin(), LineData.end(), UnINIProperty::find_prop(text) );
	if( it != LineData.end() )
		return &*it;
	return NULL;
}

UnINIProperty* UnINISection::Find( const string& prop, const string& val )
{
	UnINIPropertyList::iterator it = find_if( LineData.begin(), LineData.end(), UnINIProperty::find_propval(prop,val) );
	if( it != LineData.end() )
		return &*it;
	return NULL;
}

void UnINISection::Remove( const string& prop )
{
	//cout << "Remove " << prop  << endl;
	LineData.remove_if(UnINIProperty::find_prop(prop));    
}

void UnINISection::Remove( const string& prop, const string& val )
{
	//cout << "Remove " << prop << " " << val << endl;            
	LineData.remove_if(UnINIProperty::find_propval(prop,val));                       
}

void UnINISection::Append( const string& prop, const string& val )
{
	//cout << "Append " << prop << " " << val << endl;

	// find last existing property with the same name
	UnINIPropertyList::reverse_iterator it = find_if( LineData.rbegin(), LineData.rend(), UnINIProperty::find_prop(prop) );
	if( it != LineData.rend() )
		LineData.insert(it.base(),UnINIProperty(prop, val));
	else
		PushLine(UnINIProperty(prop, val));
}

void UnINISection::ReadLine( const string& text )
{
	if( bSkipSection )
		return;

	string prop, val, cmd;
	size_t pos = text.find("=");
	if( pos != string::npos )
	{
		prop = trim(text.substr(0,pos), " \t");
		val = trim(text.substr(pos+1), " \t");

		if( File->bChangeFile && prop.substr(0,1).find_first_of("+-!.") != string::npos )
		{
			cmd = prop.substr(0,1);
			prop = prop.substr(1);
		}
	}
	else if( !text.empty() && File->bDefaultOnly )
	{
		size_t pos = text.find("PatchINI::DefaultOnly");
		if( pos != string::npos )
			bSkipSection = true;
	}


	PushLine(UnINIProperty(text, prop, val, cmd));
}

void UnINISection::Write( ofstream* stream )
{
	for_each( LineData.begin(), LineData.end(), bind2nd(mem_fun1_ref(&UnINIProperty::Write),stream) );

	// add empty line after each non-empty section
	if( !Text.empty() && !LineData.empty() )
		*stream << endl;
}

void UnINISection::MergeInto( UnINIFile* basefile )
{
	//cout << "MergeInto " << endl;

	UnINISection* basesection = basefile->FindSection(Text);
	if(!basesection )
		basesection = basefile->AddSection(Text);

	for_each( LineData.begin(), LineData.end(), bind2nd(mem_fun1_ref(&UnINIProperty::Merge),basesection) );
}


// ============================================================================
//  UnINIFile
// ============================================================================

void UnINIFile::DumpDebug( ostream* stream )
{
	for_each( Sections.begin(), Sections.end(), bind2nd(mem_fun1(&UnINISection::DumpDebug),stream) );
}

void UnINIFile::Read( const char* path )
{
	//cout << "Read " << path << endl;

	ifstream file(path);
	if( !file )
		throw logic_error(string_make() << "Cannot open file: " << path);

	string text;
	UnINISection* CurrentSection = AddSection("");
	while( getline(file, text) )
	{
		if( !text.empty() )
		{
			//cout << text << endl;

			// if section
			if( text.at(0) == '[' && text.at(text.length()-1 ) == ']' )
			{
				string sectionname = text.substr(1,text.length()-2);
				CurrentSection = FindSection(sectionname);
				if(!CurrentSection )
					CurrentSection = AddSection(sectionname);
			}
			else
			{
				CurrentSection->ReadLine(text);
			}
		}
	}
}

void UnINIFile::Write( const char* path )
{
	//cout << "Write " << path << endl;
	ofstream file(path);
	if( !file )
		throw logic_error(string_make() << "Cannot open file: " << path);

	for_each( Sections.begin(), Sections.end(), bind2nd(mem_fun1(&UnINISection::Write), &file) );
}

void UnINIFile::MergeInto( UnINIFile& basefile )
{
	//cout << "MergeInto " << endl;
	for_each( Sections.begin(), Sections.end(), bind2nd(mem_fun1(&UnINISection::MergeInto), &basefile) );
}


UnINISection* UnINIFile::FindSection( const string& text )
{
	UnINISectionData::reverse_iterator it = find_if( Sections.rbegin(), Sections.rend(), UnINISection::compare_no_case(text) );
	if( it != Sections.rend() )
		return *it;
	return NULL;
}

UnINISection* UnINIFile::AddSection( const string& text )
{
	//cout << "AddSection " << text << endl;
	Sections.push_back(new UnINISection(text,this));
	return Sections.back();
}


// ============================================================================
//  C Interface
// ============================================================================

int PatchINIMerge( const char* mergeini
				  , const char* baseini
				  , const char* resultini
				  , int flags
				  , char* errorbuffer
				  , int errorbufferlen )
{
	try
	{
		UnINIFile changefile(true, (flags & PATCHINI_DEFAULTONLY) == PATCHINI_DEFAULTONLY);
		UnINIFile basefile;

		changefile.Read(mergeini);
		basefile.Read(baseini);

		changefile.MergeInto(basefile);
		basefile.Write(resultini);

		if( errorbuffer )
			strcpy_s(errorbuffer, errorbufferlen, "");
		return 0;
	}
	catch (exception& e)
	{
		if( errorbuffer )
			strcpy_s(errorbuffer, errorbufferlen, e.what());
	}
	catch (...)
	{
		if( errorbuffer )
			strcpy_s(errorbuffer, errorbufferlen, "unknown PatchINIMerge error");
	}

	return 1;
}

string toNarrowString( const wchar_t* pStr )
{
	// figure out how many narrow characters we are going to get 

	int len = static_cast<int>(wcslen(pStr));
	int nChars = WideCharToMultiByte( CP_UTF8 , 0 , pStr , len , NULL , 0 , NULL , NULL ) ; 

	if ( len == -1 )
		-- nChars ; 
	if ( nChars == 0 )
		return "" ;

	// convert the wide string to a narrow string

	// nb: slightly naughty to write directly into the string like this

	string buf ;
	buf.resize( nChars ) ;
	WideCharToMultiByte( CP_UTF8 , 0 , pStr , len , const_cast<char*>(buf.c_str()) , nChars , NULL , NULL ) ; 

	return buf ; 
}

wstring toWideString( const char* pStr )
{
	// figure out how many wide characters we are going to get 

	int len = static_cast<int>(strlen(pStr));
	int nChars = MultiByteToWideChar( CP_ACP , 0 , pStr , len , NULL , 0 ) ; 
	if ( len == -1 )
		-- nChars ; 
	if ( nChars == 0 )
		return L"" ;

	// convert the narrow string to a wide string 

	// nb: slightly naughty to write directly into the string like this

	wstring buf ;
	buf.resize( nChars ) ; 
	MultiByteToWideChar( CP_ACP , 0 , pStr , len , const_cast<wchar_t*>(buf.c_str()) , nChars ) ; 

	return buf ;
}


int PatchINIMergeW( const wchar_t* mergeini
				  , const wchar_t* baseini
				  , const wchar_t* resultini
				  , int flags
				  , wchar_t* errorbuffer
				  , int errorbufferlen )
{

	string mergeini_mb = toNarrowString(mergeini);
	string baseini_mb = toNarrowString(baseini);
	string resultini_mb = toNarrowString(resultini);

	/*::MessageBoxA(NULL, mergeini_mb.c_str(), "PatchINI: mergeini", MB_OK);
	::MessageBoxA(NULL, baseini_mb.c_str(), "PatchINI: baseini", MB_OK);
	::MessageBoxA(NULL, resultini_mb.c_str(), "PatchINI: resultini", MB_OK);*/

	static const int errorbufferlen_mb = 1024;
	char errorbuffer_mb[errorbufferlen_mb] = {0};

	int result = PatchINIMerge(mergeini_mb.c_str(), baseini_mb.c_str(), resultini_mb.c_str(), flags, &errorbuffer_mb[0], errorbufferlen_mb);
	//::MessageBoxA(NULL, errorbuffer_mb, "PatchINI: error", MB_OK);

	return result;
}

// ============================================================================
//  EOF
// ============================================================================