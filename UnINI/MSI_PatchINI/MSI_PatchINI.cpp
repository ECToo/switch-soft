// MSI_PatchINI.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "MSI_PatchINI.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif


#define MB_RETURN_ERROR(text){ \
	::MessageBox(NULL, text, TEXT("Error"), MB_OK); \
	return ERROR_INSTALL_FAILURE; }


wstring MsiGetPropertyStl( MSIHANDLE hInstall, LPCWSTR szName )
{
	TCHAR* szValueBuf = NULL;
	DWORD cchValueBuf = 0;
	UINT uiStat =  MsiGetProperty(hInstall, szName, TEXT(""), &cchValueBuf);
	if (ERROR_MORE_DATA == uiStat)
	{
		++cchValueBuf; // on output does not include terminating null, so add 1
		szValueBuf = new TCHAR[cchValueBuf];
		if (szValueBuf)
		{
			uiStat = MsiGetProperty(hInstall, szName, szValueBuf, &cchValueBuf);
		}
	}

	if (ERROR_SUCCESS != uiStat)
	{
		if (szValueBuf != NULL) 
			delete [] szValueBuf;
		return wstring(TEXT(""));
	}

	wstring result = szValueBuf;
	delete [] szValueBuf;
	return result;
}

BOOL PatchINI_Existing( wstring mergePath, wstring targetPath, int flags=1 )
{
	// Default INI must exist
	if( ::PathFileExists(mergePath.c_str()) == FALSE )
		return FALSE;

	// Target INI may not exist, and it won't be created
	if( ::PathFileExists(targetPath.c_str()) == FALSE )
		return TRUE;

	// Patch target INI
	int result = PatchINIMergeW(mergePath.c_str(), targetPath.c_str(), targetPath.c_str(), flags, NULL, 0);
	return result == 0 ? TRUE : FALSE;
}

UINT __stdcall PatchINI_2K4Mod(MSIHANDLE hInstaller)
{
	// Get properties
	wstring productStr = MsiGetPropertyStl(hInstaller, TEXT("ProductName"));
	wstring installPath = MsiGetPropertyStl(hInstaller, TEXT("INSTALLDIR"));

	// Get paths
	wstring modSystemPath = installPath + wstring(TEXT("./")) + productStr + wstring(TEXT("/System/"));

	if( PatchINI_Existing(modSystemPath + wstring(TEXT("Default.ini")), modSystemPath + productStr + wstring(TEXT(".ini"))) == FALSE )
		MB_RETURN_ERROR(TEXT("PatchExistingINI2K4(Default.ini) failed!"));

	if( PatchINI_Existing(modSystemPath + wstring(TEXT("DefUser.ini")), modSystemPath + productStr + wstring(TEXT("User.ini"))) == FALSE )
		MB_RETURN_ERROR(TEXT("PatchExistingINI2K4(DefUser.ini) failed!"));

	return ERROR_SUCCESS;
}