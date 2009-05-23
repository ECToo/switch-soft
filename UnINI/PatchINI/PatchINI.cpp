// ============================================================================
//  PatchINI.cpp : Defines the entry point for the console application.
// ============================================================================

#include "stdafx.h"
#include "PatchINI.h"
#include "PatchINIStatic.h"

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
	string prop, val, cmd;
	size_t pos = text.find("=");
	if( pos != string::npos )
	{
		prop = trim(text.substr(0,pos), " \t");
		val = trim(text.substr(pos+1), " \t");

		if( bChangeFile && prop.substr(0,1).find_first_of("+-!.") != string::npos )
		{
			cmd = prop.substr(0,1);
			prop = prop.substr(1);
		}
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
	Sections.push_back(new UnINISection(text,bChangeFile));
	return Sections.back();
}


// ============================================================================
//  C Interface
// ============================================================================

int PatchINIMerge( const char* mergeini
				  , const char* baseini
				  , const char* resultini
				  , char* errorbuffer
				  , int errorbufferlen )
{
	try
	{
		UnINIFile changefile(true);
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


// ============================================================================
//  EOF
// ============================================================================