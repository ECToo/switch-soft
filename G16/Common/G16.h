//-------------------------------------------------------------------------------
//
//	G16 BMP File Format plugin for Photoshop 5.0+
//	Copyright © 2004 neai.o2.pl
//
//-------------------------------------------------------------------------------
//
//	File:
//		G16.h
//
//	Copyright 1994-1997, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Description:
//		This file contains the header prototypes and macros for the
//		File Format module G16.
//
//	Use:
//		Format modules are called from the Save, Save as,
//		and Save a copy dialogs.
//
//	Version history:
//		Version 1.0.0	1/1/1994	Created for Photoshop 3.0.
//			Written by Mark Hamburg
//
//		Version 2.0.0	5/27/1996	Updated for Photoshop 4.0.
//			Scripting added.
//
//		Version 2.1.0	6/28/1997	Updated for Photoshop 4.0.1.
//			Updated for new version of Photoshop and projects
//			moved to CodeWarrior Pro.
//
//-------------------------------------------------------------------------------

#ifndef __G16_H__	// Has this not been defined yet?
#define __G16_H__	// Only include this once by predefining it

#include "PIFormat.h"					// Format Photoshop header file.
#include "FileUtilities.h"				// File Utility library.
#include "G16Terminology.h"				// Terminology for plug-in.

//#define	USE_DIALOG
//#define	USE_LOG
#define	LOG_FILE	"g16log.txt"

//-------------------------------------------------------------------------------
//	Structure -- FileHeader
//-------------------------------------------------------------------------------

#pragma pack(push,2) 
typedef struct FileHeader
{
	char	bfMagic [2];
	uint32	bfSize;
	uint32	bfReserved;
	uint32	bfOffset;

	uint32	biSize;
	int32	biWidth;
	int32	biHeight;
	uint16	biPlanes;
	uint16	biBitCount;
	uint32	biCompression;
	uint32	biSizeImage;
	int32	biXPelsPerMeter;
	int32	biYPelsPerMeter;
	uint32	biClrUsed;
	uint32	biClrImportant;

} FileHeader;
#pragma pack(pop) 

//-------------------------------------------------------------------------------
//	Globals -- structures
//-------------------------------------------------------------------------------

// Windows handle for our plug-in (seen as a dynamically linked library):
extern Handle hDllInstance;


#define PROPINT(NAME,VAL)	{ #NAME, VAL } 
#define PROPIDX(NAME)		PI_ ## NAME

typedef enum 
{
	PROPIDX(swapBytes),
	PROPIDX(MAX)
}IntPropsIdx;

typedef struct 
{ 
	char	*name; 
	int32	value; 
} IntProp, IntPropt[PI_MAX]; 

static IntPropt	IntPropDefault = 
{ 
	PROPINT(swapBytes,0), 
};


// This is our structure that we use to pass globals between routines:
typedef struct
{ 
	short				*result;			// Must always be first in Globals.
	FormatRecord		*formatParamBlock;	// Must always be second in Globals.

	BufferID			pixelBuffer;
	Ptr					pixelData;
	int32				rowBytes;
	Str255				history;
	IntPropt			IntProps;

#ifdef USE_DIALOG
	#ifdef __PIWin__
		HWND			dialog;
		HWND			list;
	#elif defined(__PIMac__)
		DialogPtr		dialog;
		DialogTHndl		dialogHdl;
	#endif
#endif

} Globals, *GPtr, **GHdl;				// *GPtr = global pointer; **GHdl = global handle


//-------------------------------------------------------------------------------
//	Globals -- definitions and macros
//-------------------------------------------------------------------------------

#define gResult				(*(globals->result))
#define gStuff				(globals->formatParamBlock)

#define gPixelBuffer		(globals->pixelBuffer)
#define gPixelData			(globals->pixelData)
#define gRowBytes			(globals->rowBytes)
#define gHistory			(globals->history)
#define gIntProps			(globals->IntProps)

#ifdef USE_DIALOG
	#define gDialog			(globals->dialog)
	#define gDialogHdl		(globals->dialogHdl)
	#define gList			(globals->list)
#endif


//-------------------------------------------------------------------------------
//	Prototypes
//-------------------------------------------------------------------------------

// Everything comes in and out of ENTRYPOINT. It must be first routine in source:
MACPASCAL void ENTRYPOINT	( const short selector,
							FormatRecord *formatParamBlock,
							long *data,
							short *result);

void	InitGlobals			( Ptr globalPtr );			// Initialize globals.
void	ValidateParameters	( GPtr globals );			// Validates (inits) parameters.

Boolean ReadScriptParamsOnRead		( GPtr globals );	// Read any scripting params.
Boolean ReadScriptParamsOnWrite		( GPtr globals );	// Read any scripting params.
OSErr	WriteScriptParamsOnRead		( GPtr globals );	// Write any scripting params.
OSErr	WriteScriptParamsOnWrite	( GPtr globals );	// Write any scripting params.

void	DoAbout		( AboutRecordPtr about );	// Pop about box.

char*	its			( int32 i );		// int to string
int32	sti			( char* s );		// string to int
int32	rint		( double dbl );		// float to int, don't touch
int16	SwapBytes	( int16 s );		// endianism

#ifdef USE_DIALOG
	Boolean DialogOpen	( GPtr globals );
	Boolean DialogRun	( GPtr globals );
	void	DialogClose	( GPtr globals );

	BOOL WINAPI	UIProc	( HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam );
	BOOL WINAPI	LVProc	( HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam );

	WNDPROC	LVProcPrev;
#endif


#ifdef USE_LOG
	void	LogNuke		( void );
	void	LogWrite	( const char* Name, const char* Value );
#else
	#define	LogNuke()
	#define	LogWrite(Name,Value)
#endif

//-------------------------------------------------------------------------------

#endif // __G16_H__
