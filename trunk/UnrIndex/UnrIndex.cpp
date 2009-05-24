// UnrIndex.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



	// first byte of data s same as first serialized byte

   	//int32 Original = D.Value;
	//dword V = labs(D.Value);
   	//byte B0 = ((D.Value>=0) ? 0 : 0x80) + ((V < 0x40) ? V : ((V & 0x3f)+0x40));
    //D.Value = 0;
	
	// 1111 1111 1111 1111 1111 1111 1111 1111
	// 0000 0000 0000 0000 0000 0000 0000 0000
	// 0100 0000 0000 0000 0000 0000 0000 0000
	// 32									 0


inline wxString PrintIndex( wxInt32 value )
{
	wxString text;
	byte data[5];
	memset(&data[0],0,sizeof(data));
   	wxInt32 Original = value;
	wxUint32 V = labs(value);
   	data[0] = ((value>=0) ? 0 : 0x80) + ((V < 0x40) ? V : ((V & 0x3f)+0x40));
	text += wxString::Format(wxT("%.2x"),data[0]);
    if( data[0] & 0x40 )
    {
        V >>= 6;
        data[1] = (V < 0x80) ? V : ((V & 0x7f)+0x80);
		text += wxString::Format(wxT("%.2x"),data[1]);
        if( data[1] & 0x80 )
        {
            V >>= 7;
            data[2] = (V < 0x80) ? V : ((V & 0x7f)+0x80);
			text += wxString::Format(wxT("%.2x"),data[2]);
            if( data[2] & 0x80 )
            {
                V >>= 7;
                data[3] = (V < 0x80) ? V : ((V & 0x7f)+0x80);
				text += wxString::Format(wxT("%.2x"),data[3]);
                if( data[3] & 0x80 )
                {
                    V >>= 7;
                    data[4] = V;
					text += wxString::Format(wxT("%.2x"),data[4]);
                }
            }
        }
    }
	
	return text;
}

inline bool ScanIndex( wxString text, unsigned long& value )
{
	wxUint8 data[5];
	memset(&data[0],0,sizeof(data));

	if( text.IsEmpty() )
	{
		value = 0;
		return true;
	}

	if( text.Length() % 2 != 0 )
		return false;

	unsigned long byteval;
	for( wxUint32 i=0; i<5; ++i)
	{
		if( i*2 < text.Length() )
		{
			if( text.Mid(i*2,2).ToULong(&byteval,16) )
				data[i] = byteval;
			else
				return false;
		}
		else
			break;
	}

	value = 0;

    if( data[0] & 0x40 ) {
        if( data[1] & 0x80 ) {
            if( data[2] & 0x80 ) {
                if( data[3] & 0x80 ) {
                    value = data[4];
                } value = (value << 7) + (data[3] & 0x7f);
            } value = (value << 7) + (data[2] & 0x7f);
        } value = (value << 7) + (data[1] & 0x7f);
    } value = (value << 6) + (data[0] & 0x3f);

    if( data[0] & 0x80 )
        value = -static_cast<wxInt32>(value);

	wxString test = PrintIndex(value);
	if( !text.IsSameAs(test) )
		return false;

	return true;
}



inline wxString PrintBinary( wxUint32 value )
{
	if( value == 0 )
		return wxT("0");

	wxString text;
	text.Append(wxT('0'),32);

	for( int i=0; i!=32; ++i )
	{
		if( (value & 1 << i) == 1 << i )
			text[31-i] = wxT('1');
	}
	// 10000000000000000000000000000000


	return text;
}

inline bool ScanBinary( wxString text, unsigned long& value )
{
	if( text.Length() != 32 )
		return false;

	value = 0;
	for( int i=0; i!=32; ++i )
	{
		if( text[31-i] == wxT('1') )
		{
			value |= 1 << i;
		}
		else if( text[31-i] != wxT('0') )
			return false;
	}
	return true;
}



// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
	MyFrame
	(	wxWindow*		parent
	,	wxWindowID		id
	,	const wxString&	title
	,	const wxPoint&	pos = wxDefaultPosition
	,	const wxSize&	size = wxDefaultSize
	,	long			style = wxDEFAULT_FRAME_STYLE
	,	const wxString&	name = wxT("UnrIndex")
	);

    void OnText(wxCommandEvent& event);
	void OnMaxLen(wxCommandEvent& event);

private:
	wxTextCtrl* DecBox;
	wxTextCtrl* HexBox;
	wxTextCtrl* IntBox;
	wxTextCtrl* IdxBox;
	wxTextCtrl* BitBox;
	wxObject* Updating;


    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    Box_Dec
,	Box_Hex
,	Box_Int
,	Box_Idx
,	Box_Bit
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------
 // the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_TEXT(wxID_ANY, MyFrame::OnText)
	EVT_TEXT_MAXLEN(wxID_ANY, MyFrame::OnMaxLen)
END_EVENT_TABLE()


// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if ( !wxApp::OnInit() )
        return false;

    // create the main application window
    MyFrame *frame = new MyFrame( NULL, wxID_ANY, wxT("UnrIndex"),wxDefaultPosition,wxDefaultSize,wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxSTAY_ON_TOP | wxMINIMIZE_BOX );

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame
(	wxWindow*		parent
,	wxWindowID		id
,	const wxString&	title
,	const wxPoint&	pos
,	const wxSize&	size
,	long			style
,	const wxString&	name
)	
:	wxFrame( parent, id, title, pos, size, style, name )
,	Updating(NULL)
{

	wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	DecBox = new wxTextCtrl(panel,Box_Dec,wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	HexBox = new wxTextCtrl(panel,Box_Hex,wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	IntBox = new wxTextCtrl(panel,Box_Int,wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	IdxBox = new wxTextCtrl(panel,Box_Idx,wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	BitBox = new wxTextCtrl(panel,Box_Bit,wxEmptyString, wxDefaultPosition, wxSize(280,20), 0 );

	wxFont boxfont = wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT );
	DecBox->SetFont(boxfont);
	HexBox->SetFont(boxfont);
	IntBox->SetFont(boxfont);
	IdxBox->SetFont(boxfont);
	BitBox->SetFont(boxfont);

	wxStaticBoxSizer* decframe = new wxStaticBoxSizer( wxHORIZONTAL, panel, wxT("Base-10") );
	wxStaticBoxSizer* hexframe = new wxStaticBoxSizer( wxHORIZONTAL, panel, wxT("Base-16") );
	wxStaticBoxSizer* intframe = new wxStaticBoxSizer( wxHORIZONTAL, panel, wxT("Serialized") );
	wxStaticBoxSizer* idxframe = new wxStaticBoxSizer( wxHORIZONTAL, panel, wxT("Index Serialized") );
	wxStaticBoxSizer* bitframe = new wxStaticBoxSizer( wxHORIZONTAL, panel, wxT("Binary") );

	wxSizerFlags boxflags = wxSizerFlags().Expand().Border(2);
	decframe->Add(DecBox,boxflags);
	hexframe->Add(HexBox,boxflags);
	intframe->Add(IntBox,boxflags);
	idxframe->Add(IdxBox,boxflags);
	
	wxSizerFlags boxframeflags = wxSizerFlags().Expand().Border(0);
	wxBoxSizer* boxsizer = new wxBoxSizer( wxHORIZONTAL );
	boxsizer->Add(decframe,boxframeflags);
	boxsizer->Add(hexframe,boxframeflags);
	boxsizer->Add(intframe,boxframeflags);
	boxsizer->Add(idxframe,boxframeflags);

	wxSizerFlags bitframeflags = wxSizerFlags(1).Expand().Border(0).Center();
	bitframe->Add(BitBox,bitframeflags);

	wxBoxSizer* topsizer = new wxBoxSizer( wxVERTICAL );
	topsizer->Add(boxsizer,boxframeflags);
	topsizer->Add(bitframe,boxframeflags);
	

	panel->SetSizer( topsizer );
	topsizer->SetSizeHints( this );


}


void MyFrame::OnMaxLen(wxCommandEvent& event)
{
}

void MyFrame::OnText(wxCommandEvent& event)
{
	wxTextCtrl* ctrl = static_cast<wxTextCtrl*>(event.GetEventObject());
	if( !ctrl 
	|| Updating != NULL 
	|| (ctrl != DecBox && ctrl != HexBox && ctrl != IntBox && ctrl != IdxBox && ctrl != BitBox) )
	{
		event.Skip();
		return;
	}

	unsigned long value = 0;
	wxString newstring = event.GetString();
	if( newstring.IsEmpty() )
		newstring = wxT("0");
	else
		newstring = newstring.Trim(false).Trim(true);

	// prevent recursive OnText events
	Updating = ctrl;
	
	DecBox->SetOwnForegroundColour( wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT) );
	HexBox->SetOwnForegroundColour( wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT) );
	IntBox->SetOwnForegroundColour( wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT) );
	IdxBox->SetOwnForegroundColour( wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT) );
	BitBox->SetOwnForegroundColour( wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT) );

	ctrl->SetOwnForegroundColour(*wxBLUE);

	if( ctrl == DecBox )
	{
		if( newstring.ToULong(&value,10) )
		{
			HexBox->SetValue( wxString::Format( wxT("%x"), value ) );
			IdxBox->SetValue( PrintIndex(value) );
			BitBox->SetValue( PrintBinary(value) );
			value = wxINT32_SWAP_ALWAYS(value);
			IntBox->SetValue( wxString::Format( wxT("%x"), value ) );
		}
		else
		{
			DecBox->SetOwnForegroundColour(*wxRED);
		}
	}
	else if( ctrl == HexBox )
	{
		if( newstring.Length() < 8 )
		{
			newstring = newstring.Pad(8-newstring.Length(),wxT('0'),false);
		}
		else if( newstring.Length() > 8 )
		{
			newstring = newstring.Truncate(8);
			ctrl->SetValue(newstring);
		}

		if( newstring.ToULong(&value,16) )
		{
			DecBox->SetValue( wxString::Format( wxT("%d"), value ) );
			IdxBox->SetValue( PrintIndex(value) );
			BitBox->SetValue( PrintBinary(value) );
			value = wxINT32_SWAP_ALWAYS(value);
			IntBox->SetValue( wxString::Format( wxT("%x"), value ) );
		}
		else
		{
			HexBox->SetOwnForegroundColour(*wxRED);
		}
	}
	else if( ctrl == IntBox )
	{
		if( newstring.Length() < 8 )
		{
			newstring = newstring.Pad(8-newstring.Length(),wxT('0'),true);
		}
		else if( newstring.Length() > 8 )
		{
			newstring = newstring.Truncate(8);
			ctrl->SetValue(newstring);
		}

		if( newstring.ToULong(&value,16) )
		{
			value = wxINT32_SWAP_ALWAYS(value);
			DecBox->SetValue( wxString::Format( wxT("%d"), value ) );
			HexBox->SetValue( wxString::Format( wxT("%x"), value ) );
			IdxBox->SetValue( PrintIndex(value) );
			BitBox->SetValue( PrintBinary(value) );
		}
		else
		{
			IntBox->SetOwnForegroundColour(*wxRED);
		}
	}
	else if( ctrl == IdxBox )
	{
		if( newstring.Length() > 10 )
		{
			newstring = newstring.Truncate(10);
			ctrl->SetValue(newstring);
		}

		if( ScanIndex(newstring,value) )
		{
			DecBox->SetValue( wxString::Format( wxT("%d"), value ) );
			HexBox->SetValue( wxString::Format( wxT("%x"), value ) );
			BitBox->SetValue( PrintBinary(value) );
			value = wxINT32_SWAP_ALWAYS(value);
			IntBox->SetValue( wxString::Format( wxT("%x"), value ) );
		}
		else
		{
			IdxBox->SetOwnForegroundColour(*wxRED);
		}
	}
	else if( ctrl == BitBox )
	{
		if( newstring.Length() < 32 )
		{
			newstring = newstring.Pad(32-newstring.Length(),wxT('0'),false);
		}
		else if( newstring.Length() > 32 )
		{
			newstring = newstring.Truncate(32);
			ctrl->SetValue(newstring);
		}


		if( ScanBinary(newstring,value) )
		{
			DecBox->SetValue( wxString::Format( wxT("%d"), value ) );
			HexBox->SetValue( wxString::Format( wxT("%x"), value ) );
			IdxBox->SetValue( PrintIndex(value) );
			IntBox->SetValue( wxString::Format( wxT("%x"), value ) );
		}
		else
		{
			BitBox->SetOwnForegroundColour(*wxRED);
		}
	}

	DecBox->Refresh();
	HexBox->Refresh();
	IntBox->Refresh();
	IdxBox->Refresh();
	BitBox->Refresh();

	if( Updating == ctrl )
		Updating = NULL;
}




// event handlers
