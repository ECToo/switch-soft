// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	irPkg.h
// ============================================================================
#ifndef IRPKG_H
#define IRPKG_H

#include "uppPkg.h"



// ============================================================================
// irPkg
// ============================================================================
class irPkg : public uppPkg
{
public: 
	wxFileOffset OldSize;

public: 
	irPkg( wxFileOffset oldsize ) : OldSize(oldsize)
	{
	}

public: 
	virtual void Serialize( upArchive& A );

	void RenameImport( const wxString& oldname, const wxString& newname );

};



#endif IRPKG_H
// ============================================================================
//	The End.
// ============================================================================