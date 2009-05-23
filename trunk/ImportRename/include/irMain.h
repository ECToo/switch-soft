// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	irMain.h
// ============================================================================
#ifndef IRMAIN_H
#define IRMAIN_H

#include "upFile.h"
#include "uppPkg.h"


// ============================================================================
// irMain
// ============================================================================
class irMain : public wxAppConsole
{
public:
    virtual bool OnInit() { return true; }
    virtual int OnRun();

private:
    void ParseParams( const wxCmdLineParser& cmdline );
	void RenameImport( const wxString& oldname, const wxString& newname, const wxString& filename );
    
};

IMPLEMENT_APP_CONSOLE(irMain);



#endif IRMAIN_H
// ============================================================================
//	The End.
// ============================================================================