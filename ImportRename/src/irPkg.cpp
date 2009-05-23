// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	irPkg.cpp
// ============================================================================
#include "stdafx.h"
#include "irPkg.h"




// ============================================================================
//	irPkg
// ============================================================================
void irPkg::RenameImport( const wxString& oldname, const wxString& newname )
{
	upStrDwordMap imap;
	uppNameItem name;
	dword count = 0;

	name.Name = uppLASCIIZ(newname);
	name.Flags = RF_TagExp | RF_LoadForClient | RF_LoadForServer | RF_LoadForEdit;
	NameTable.push_back(name);
	uppIndex nameidx = uppIndex(NameTable.size()-1);


	// Rename
	for( uppImportTableItr it=ImportTable.begin(); it!=ImportTable.end(); ++it )
	{
		// Map
		const int tidx = it-ImportTable.begin();
		uppImportItem& tti = *it;
		const wxString tstr = GetObjectPath(-tidx-1);

		if( oldname.IsSameAs( tstr, false ) )
		{
			tti.ObjectName = nameidx;
			++count;
			wxLogMessage( wxString::Format( wxT("RENAMED: %s TO: %s"), tstr.c_str(), newname.c_str() ) );
		}
	}

	if( count == 0 )
		throw upexception( wxString::Format( wxT("Name not found: %s"), oldname.c_str() ) );
}

void irPkg::Serialize( upArchive& A )
{
	//UP_TIMER(t);

	if( A.IsLoading() )
	{
		A << Header;
		A << static_cast<uppTableHeader&>(NameTable);
		A << static_cast<uppTableHeader&>(ExportTable);
		A << static_cast<uppTableHeader&>(ImportTable);

		// Notice the order
		A << BI_NameTable << NameTable;
		A << BI_ImportTable << ImportTable;
		A << BI_ExportTable << ExportTable;
	}
	else
	{
		// Get table headers offset
		A.Seek(0);
		A << Header;
		wxFileOffset headers = A.Tell();

		// Append updated tables
		A.Seek(OldSize);
		A << NameTable;
		A << ImportTable;

		// Update table offsets and guid
		wxFileOffset resume = A.Tell();
		A.Seek(headers);
		A << static_cast<uppTableHeader&>(NameTable);
		A << static_cast<uppTableHeader&>(ExportTable);
		A << static_cast<uppTableHeader&>(ImportTable);
		A << Header.GUID;
		A.Seek(resume);
	}
	//UP_TIMER_LOGF(t,0);
}




// ============================================================================
//	The End.
// ============================================================================