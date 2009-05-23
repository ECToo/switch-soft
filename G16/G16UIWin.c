//-------------------------------------------------------------------------------
//
//	G16 BMP File Format plugin for Photoshop 5.0+
//	Copyright © 2004 neai.o2.pl
//
//-------------------------------------------------------------------------------
//
//	File: G16UIWin.c
//
//	Copyright © 1993-1997, Adobe Systems Incorporated.
//	All rights reserved.
//
//	C source file for MS-Windows specific code for G16 Plug-In
//
//-------------------------------------------------------------------------------

#include "G16.h"
#include "WinDialogUtils.h"


/*****************************************************************************/

char* its( int32 i )
{
	static char s[33];
	ltoa(i, s, 10);
	return s;
}

int32 sti( char* s )
{
	return atoi(s);
}


/*****************************************************************************/

void DoAbout (AboutRecordPtr about)
{
	ShowAbout(about, hDllInstance, AboutID);
}


/*****************************************************************************/

/* Initialization and termination code for window's dlls. */

#ifdef WIN32

BOOL APIENTRY DLLInit(HANDLE hInstance, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
		hDllInstance = hInstance;
	return TRUE;	// Indicate that the DLL was initialized successfully.  
}

#else	
// Code from Borland's window's dll example code.
#if defined(__BORLANDC__)
// Turn off warning: Parameter '' is never used; effects next function only
#pragma argsused 
#endif

// Every DLL has an entry point LibMain and an exit point WEP.
int FAR PASCAL LibMain( HANDLE hInstance, WORD wDataSegment,
								   WORD wHeapSize, LPSTR lpszCmdLine )
{
	// Required when using Zortech; causes blink to include startup code
	extern __acrtused_dll;

	// The startup code for the DLL initializes the local heap (if there is one)
	// with a call to LocalInit which locks the data segment.
	if ( wHeapSize != 0 )
		UnlockData( 0 );

	hDllInstance = hInstance;
	return 1;   // Indicate that the DLL was initialized successfully.
}

int FAR PASCAL WEP(int nParam)
{
	switch  (nParam) {
	  case  WEP_SYSTEM_EXIT: // System shutdown in progress
	  case  WEP_FREE_DLL   : // DLL use count is 0
	  default :              // Undefined;  ignore
			return  1;
	}
}

#endif //ifdef WIN32


/*****************************************************************************/

#ifdef USE_DIALOG

#include "commctrl.h"

#define STATE_IDLE		0
#define STATE_AGAIN		1
#define STATE_CANCEL	-1

short	state = STATE_IDLE;


/*****************************************************************************/

void SetItemIdx ( LV_ITEM Item, int ItemIdx, int SubItemIdx )
{
	Item.iItem		= ItemIdx;		//
	Item.iSubItem	= SubItemIdx;	//
}

#define ListView_InsertItemText(hwnd, pitem, ItemIdx, SubItemIdx, ItemTxt)   \
	(pitem).iItem		= (ItemIdx);\
	(pitem).iSubItem	= (SubItemIdx);\
	(pitem).pszText		= (ItemTxt);\
	ListView_InsertItem((hwnd),&(pitem))

#define ListView_SetPair(hwnd, pitem, ItemIdx, ItemTxt, SubItemTxt)   \
	(pitem).iItem		= (ItemIdx);\
	(pitem).iSubItem	= 0;\
	(pitem).pszText		= (ItemTxt);\
	ListView_InsertItem((hwnd),&(pitem));\
	ListView_SetItemText((hwnd), (ItemIdx),1,(SubItemTxt))

int32 ListView_ReadInt( HWND hwnd, int iItem, int iSubItem )
{
	Str255 buf;
	ListView_GetItemText(hwnd,iItem,iSubItem,buf,255);
	return sti(buf);
}


/*****************************************************************************/

BOOL WINAPI LVProc(HWND hList, UINT wMsg, WPARAM wParam, LPARAM lParam) 
{

	switch (wMsg)
	{
	case WM_CHAR:
		{
			// pass on keyboard input to the parent
			SendMessage(GetParent(hList), wMsg, wParam, lParam);
			break;
		}


	case WM_LBUTTONDOWN:
		{
			// single-click editing
			LV_HITTESTINFO hti;
			memset(&hti, 0, sizeof(hti));
			hti.pt.x = LOWORD(lParam);
			hti.pt.y = HIWORD(lParam);

			if( ListView_HitTest(hList,&hti) != -1 )
			{					
				SetFocus(hList);
				ListView_EditLabel(hList,hti.iItem);
			}
			return 0; 
		}

	case WM_LBUTTONDBLCLK:
		{
			// treat the double-click as another single click
			SendMessage(hList, WM_LBUTTONDOWN, wParam, lParam);
			return 0;
		}
	}

	// pass the message on to the original window procedure
	return CallWindowProc(LVProcPrev, hList, wMsg, wParam, lParam);
}


/*****************************************************************************/

BOOL WINAPI UIProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam) 
{
	static GPtr globals = NULL;		  // need to be static	

	switch  (wMsg)
	{

	case WM_INITDIALOG:
		{
			LV_ITEM		LvItem;
			LV_COLUMN	LstColVal, LstColProp;
			int			i;

			CenterDialog(hDlg);

			// set up globals	
			globals		= (GPtr) lParam;
			gList		= GetDlgItem(hDlg,1011); // get the ID of the ListView	
			LVProcPrev	= (WNDPROC)SetWindowLong(gList, GWL_WNDPROC, (DWORD)LVProc);    

			// Here we put the info on the Column headers
			// this is not data, only name of each header we like
			memset(&LstColVal,0,sizeof(LstColVal)); // Reset Column

			LstColVal.mask		= LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM|LVCF_ORDER;	// Type of mask
			LstColVal.cx		= 0x64;											// width between each coloum
			LstColVal.pszText	= "Value";										// 
			LstColVal.iOrder	= 1;											// put editable field on right side

			LstColProp			= LstColVal;
			LstColProp.pszText	= "Property";
			LstColProp.iOrder	= 0;

			// Inserting Columns as much as we want
			ListView_InsertColumn(gList, 0, &LstColVal);
			ListView_InsertColumn(gList, 1, &LstColProp);

			//  Setting common properties Of Items:
			memset(&LvItem,0,sizeof(LvItem)); // Reset Item Struct
			LvItem.mask			= LVIF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;	// Text Style
			LvItem.cchTextMax	= 256;									// Max size of test

			// allocate mem for items
			ListView_SetItemCount(gList, PI_MAX);

			// insert items
			for(i=0; i<PI_MAX; i++)
			{
				ListView_SetPair(gList, LvItem, i,its(gIntProps[i].value), gIntProps[i].name );
			}

			ShowWindow(hDlg,SW_NORMAL); 
			UpdateWindow(hDlg); 

			return FALSE;
		}

	case WM_NOTIFY:
	{
		switch( ((LPNMHDR)lParam)->code  )
		{
			case LVN_BEGINLABELEDIT: 
			{
				break;
			}

			case LVN_ENDLABELEDIT: 
			{
				int iIndex;
				char tempstr[255]="";
				HWND hEdit;

				iIndex = ListView_GetNextItem(gList,-1,LVNI_FOCUSED);
				if( iIndex != - 1 )
				{
					//save to list
					hEdit = ListView_GetEditControl(gList);
					GetWindowText(hEdit, tempstr, sizeof(tempstr));
					ListView_SetItemText(gList, iIndex,0,tempstr);

					//save to globals
					gIntProps[iIndex].value = ListView_ReadInt(gList, iIndex, 0);
				}
				break;
			}
		}
		break;
	}

	case WM_COMMAND:
	{
		int item;
		int cmd;

		item = COMMANDID (wParam);              // WIN32 Change
		cmd  = HIWORD(wParam);

		switch (item)
		{
			case ok:
				if (cmd == BN_CLICKED)
					state = STATE_AGAIN;
				break;

			case cancel:
				if (cmd == BN_CLICKED)
					state = STATE_CANCEL;
				break;
		} 
		return FALSE;
	}
	} 
	return FALSE;
}

/*****************************************************************************/

/* Setup the parameters dialog.  Returns TRUE if it succeeds. */

Boolean DialogOpen( GPtr globals )
{
	PlatformData *platform;

	state = STATE_IDLE;

	if (gDialog != NULL)
	{
		SetActiveWindow (gDialog);	// set the dialog window the active window
		return TRUE;
	}
	else
	{
		// dialog not created yet.  Do it.
		platform = ((FormatRecordPtr)gStuff)->platformData;

		gDialog = CreateDialogParam(hDllInstance,
			(LPSTR)"TESTDIALOG",
			(HWND)platform->hwnd,
			(DLGPROC)UIProc, // FARPROC
			(LPARAM)globals);

		if (gDialog)
		{
			// got it.  Show it.
			ShowWindow(gDialog, SW_NORMAL);
			UpdateWindow(gDialog);
			return TRUE;
		}
		else // lost it.
			return FALSE;		// Couldnt put up the dialog box		
	}
}

/*****************************************************************************/

// Run the parameters dialog.  Returns TRUE if it succeeds.				

Boolean DialogRun (GPtr globals)
{
	MSG msg;

	// We are basically waiting for user to press "OK" or "Cancel" from the dialog

	while (GetMessage((LPMSG)&msg, NULL, 0, 0))
	{
		if (!IsDialogMessage((HWND)gDialog, &msg))
		{ 
			// wasn't for our dialog.  Send it on its way.
			TranslateMessage((LPMSG)&msg);
			DispatchMessage ((LPMSG)&msg);
		}

		// state is set in UIProc based on the user response
		// The only way this function would return is by the user responding with
		// either pressing the "OK" or the "Cancel" button
		if (state == STATE_AGAIN)
		{ 
			return TRUE;
		}
		else if (state == STATE_CANCEL)
		{
			return FALSE;
		}
	}
	return FALSE;
}


/*****************************************************************************/

/* Close the parameters dialog. */

void DialogClose (GPtr globals)
{
	if (gDialog != NULL)
	{
		DestroyWindow(gDialog);
		gDialog = NULL;
	}
}

#endif //ifdef USE_DIALOG


/*****************************************************************************/
