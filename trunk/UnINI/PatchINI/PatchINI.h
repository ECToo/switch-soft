// ============================================================================
//  PatchINI.h
// ============================================================================
#pragma once		


// ============================================================================
//  forward declarations
// ============================================================================

class UnINISection;
class UnINIFile;


// ============================================================================
//  DeleteObject
// ============================================================================
struct DeleteObject
{
	template<typename T> void operator()(const T* ptr) const
	{
		delete ptr;
	}
};


// ============================================================================
//  string_make
// ============================================================================
class string_make : public ostringstream 
{
public:
	operator string() const { return str(); };
	template<typename AA>
	string_make &operator << (const AA &str) 
	{
		*static_cast<ostringstream *>(this) << str;
		return *this;
	}
};

// ============================================================================
//  cstring_make
// ============================================================================
class cstring_make : public string_make 
{
public:
	operator const char *() const { return str().c_str(); }
private:
	operator string() const { return str(); };
};


// ============================================================================
//  std::string trim
// ============================================================================
inline string trim(const string& s, const string& chars) 
{
	if( s.length() != 0 )
	{
		size_t beg = s.find_first_not_of(chars);
		size_t end = s.find_last_not_of(chars);
		if(beg != string::npos)
			return string(s, beg, end - beg + 1);
	}
	return "";
}


// ============================================================================
//  UnINIProperty
// ============================================================================
class UnINIProperty
{
public:
	struct find_prop
	{
		const string& Property;
		find_prop( const string& prop ) : Property(prop) {}
		bool operator()( UnINIProperty& s ) const { return s.IsValid() && _stricmp( Property.c_str(), s.Property.c_str() ) == 0; }
	};

	struct find_propval
	{
		const string& Property;
		const string& Value;
		find_propval( const string& prop, const string& val ) : Property(prop), Value(val) {}
		bool operator()( UnINIProperty& s ) const { return s.IsValid() && _stricmp( Property.c_str(), s.Property.c_str() ) == 0 && _stricmp( Value.c_str(), s.Value.c_str() ) == 0; }
	};

public:
	UnINIProperty() : bIsValid(false)
	{
	}

	UnINIProperty(const string& text) : Text(text), bIsValid(false)
	{
	}

	UnINIProperty(const string& prop, const string& val) : Property(prop), Value(val), bIsValid(true)
	{
		UpdateText();
	}

	UnINIProperty(const string& text, const string& prop, const string& val, const string& cmd) 
		: Text(text), Property(prop), Value(val), Cmd(cmd), bIsValid(!text.empty() && !prop.empty())
	{
	}

public:
	bool IsValid()
	{
		return bIsValid;
	}

	void SetValue(const string& value)
	{
		bIsValid = true;
		Value = value;
		UpdateText();
	}

	void UpdateText()
	{
		Text = string_make() << Cmd << Property << "=" << Value;
	}

	void DumpDebug( ostream* stream );
	void Merge( UnINISection* to );
	void Write( ofstream* stream );

private:
	string Text;
	string Property;
	string Value;
	string Cmd;
	bool bIsValid;
};

typedef list<UnINIProperty> UnINIPropertyList;


// ============================================================================
//  UnINISection
// ============================================================================
class UnINISection
{
public:
	struct compare_no_case
	{
		const string& Text;
		compare_no_case( const string& text ) : Text( text ) {}
		bool operator()( UnINISection* s ) const { return _stricmp( Text.c_str(), s->Text.c_str() ) == 0; }
	};

public:
	UnINISection()
	{
	}

	UnINISection(const string& text, bool bchangefile=false) : Text(text), bChangeFile(bchangefile)
	{	
		if( !Text.empty() )
			PushLine(UnINIProperty(string_make() << "[" << Text << "]"));
	}

	~UnINISection()
	{
	}

public:
	void DumpDebug( ostream* stream );

	UnINIProperty& PushLine( const UnINIProperty& prop );

	UnINIProperty* Find( const string& text );
	UnINIProperty* Find( const string& prop, const string& val );

	void Remove( const string& prop );
	void Remove( const string& prop, const string& val );
	void Append( const string& prop, const string& val );

	void ReadLine( const string& text );
	void Write( ofstream* stream );
	void MergeInto( UnINIFile* basefile );



private:
	string Text;
	UnINIPropertyList LineData;
	bool bChangeFile;
};

typedef list<UnINISection*> UnINISectionData;


// ============================================================================
//  UnINIFile
// ============================================================================
class UnINIFile
{
public:
	UnINIFile( bool changefile=false ) : bChangeFile(changefile)
	{
	}

	~UnINIFile()
	{
		for_each(Sections.begin(), Sections.end(), DeleteObject());
	}

public:
	void DumpDebug( ostream* stream );

	void Read( const char* path );
	void Write( const char* path );
	void MergeInto( UnINIFile& basefile );

	UnINISection* FindSection( const string& text );
	UnINISection* AddSection( const string& text );

private:
	UnINISectionData Sections;
	bool bChangeFile;

};



// ============================================================================
//  EOF
// ============================================================================