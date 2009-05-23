// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	irMain.cpp
// ============================================================================
#include "stdafx.h"
#include "irMain.h"
#include "irPkg.h"


// ============================================================================
//	irMain
// ============================================================================
int irMain::OnRun()
{
	try 
	{
		static const wxCmdLineEntryDesc cmdLineDesc[] =
		{
			{ wxCMD_LINE_PARAM,  NULL, NULL, _T("InputFile"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY },
			{ wxCMD_LINE_PARAM,  NULL, NULL, _T("OldName"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
			{ wxCMD_LINE_PARAM,  NULL, NULL, _T("NewName"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
			{ wxCMD_LINE_NONE, NULL, NULL, NULL, (wxCmdLineParamType)0, 0 }
		};

		wxCmdLineParser parser(cmdLineDesc, argc, argv);
		
		switch (parser.Parse())
		{
			case -1:
				return 0;

			case 0:
				ParseParams(parser);
				return 0;
		}

		return 1;
	} 
	catch( upexception& e ) { wxLogError(wxT("%s"), e.wwhat()); return 1; }		  
	catch( exception& e ) { wxLogError(wxT("%hs"), e.what()); return 1; }
}


void irMain::ParseParams( const wxCmdLineParser& cmdline )
{
	if( cmdline.GetParamCount() == 1 )
	{
		RenameImport( wxT("SwampTextures"),		wxT("JRTX_SwampJZ"),	cmdline.GetParam(0) );
		RenameImport( wxT("SwampNoTexture"),	wxT("JRSM_SwampJZ"),	cmdline.GetParam(0) );
		RenameImport( wxT("JurassicRage"),		wxT("JRSM_Misc"),		cmdline.GetParam(0) );
		RenameImport( wxT("JRMeshes"),			wxT("JRTX_Misc"),		cmdline.GetParam(0) );
	}
	else if( cmdline.GetParamCount() == 3 )
	{
		RenameImport( cmdline.GetParam(1), cmdline.GetParam(2), cmdline.GetParam(0) );
	}

	wxShell(wxT("PAUSE"));
}

void irMain::RenameImport( const wxString& oldname, const wxString& newname, const wxString& filename )
{
	guard;

	//wxLogMessage( wxT("") );
	//wxLogMessage( wxT("RenameImport() %s"), filename.c_str() );

	// Rename imports
	{
		// Load
		upFileReader freader( filename );
		irPkg pkg( freader.Length() );
		pkg.Serialize(freader);

		// Rename
		pkg.RenameImport( oldname, newname );

		// Write
		upFileWriter fwriter( filename + wxT(".ir.temp") );

		fwriter.Serialize( freader.GetDataPtr(), freader.Length() );

		pkg.Serialize(fwriter);
	}

	wxRemoveFile( filename );
	wxRenameFile( filename + wxT(".ir.temp"), filename );

	// Rename files

	unguard;
}




// ============================================================================
//	The End.
// ============================================================================