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
//	Copyright 1993-1997, Adobe Systems Incorporated.
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
//		Version 1.0.0	1/1/1993	Created for Photoshop 3.0.
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

//-------------------------------------------------------------------------------
//	Includes -- Use precompiled headers if compiling with CodeWarrior.
//-------------------------------------------------------------------------------

#include "PIDefines.h"

#ifdef __PIMWCWMacPPC__
	#include "G16-PPC.ch"
#elif defined(__PIMWCWMac68K__)
	#include "G16-68k.ch"
	// We're using Metrowerks and they have an A4 library, so set that up:
	#include <SetupA4.h>
	#include <A4Stuff.h>
	
	#define UseA4 // We've now defined this for quick checks later.
#else
	#include "G16.h"
#endif

#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 

//-------------------------------------------------------------------------------
//	Prototypes
//-------------------------------------------------------------------------------

static void DoReadPrepare (GPtr globals);
static void DoReadStart (GPtr globals);
static void DoReadContinue (GPtr globals);
static void DoReadFinish (GPtr globals);
static void DoOptionsPrepare (GPtr globals);
static void DoOptionsStart (GPtr globals);
static void DoOptionsContinue (GPtr globals);
static void DoOptionsFinish (GPtr globals);
static void DoEstimatePrepare (GPtr globals);
static void DoEstimateStart (GPtr globals);
static void DoEstimateContinue (GPtr globals);
static void DoEstimateFinish (GPtr globals);
static void DoWritePrepare (GPtr globals);
static void DoWriteStart (GPtr globals);
static void DoWriteContinue (GPtr globals);
static void DoWriteFinish (GPtr globals);
static void DoFilterFile (GPtr globals);

static void AddComment (GPtr globals);
static void GetHistory (GPtr globals, int16 index, Str255 s);

static Boolean	CheckIdentifier (char identifier []);
static void		SetIdentifier (char identifier []);
static Boolean	CheckForServices (GPtr globals);
static int32	RowBytes (GPtr globals);
static Boolean	AllocatePixelBuffer (GPtr globals);
static void		DisposePixelBuffer (GPtr globals);

static void ReadSome (GPtr globals, int32 count, void *buffer);
static void WriteSome (GPtr globals, int32 count, void *buffer);
static void ReadRow (GPtr globals);
static void WriteRow (GPtr globals);
static void DisposeImageResources (GPtr globals);

//-------------------------------------------------------------------------------
//	Globals -- Define global variables for plug-in scope.
//-------------------------------------------------------------------------------

// Windows global for plug-in (considered a Dynamically Linked Library).
// Leave NULL for Macintosh; many cross-platform library utilities require
// something passed in for hDllInstance:
Handle hDllInstance = NULL;


//-------------------------------------------------------------------------------
//
//	ENTRYPOINT / main
//
//-------------------------------------------------------------------------------

MACPASCAL void ENTRYPOINT (const short selector,
						   FormatRecord *formatParamBlock,
						   long *data,
						   short *result)
{
	//---------------------------------------------------------------------------
	//	(1) Enter code resource if Mac 68k.
	//---------------------------------------------------------------------------
	
	#ifdef UseA4				// Are we in 68k Mac MW?
		EnterCodeResource(); 	// A4-globals
	#endif

	LogWrite( "selector: ", its(selector) );
	
	//---------------------------------------------------------------------------
	//	(2) Check for about box request.
	//---------------------------------------------------------------------------

	if (selector == formatSelectorAbout)
	{
		DoAbout((AboutRecordPtr)formatParamBlock);
	}
	else
	{
		//-----------------------------------------------------------------------
		//	(3) Initialize function and global pointers.
		//-----------------------------------------------------------------------
		
	 	static const FProc routineForSelector [] =
		{
			/* formatSelectorAbout  				DoAbout, */
			
			/* formatSelectorReadPrepare */			DoReadPrepare,
			/* formatSelectorReadStart */			DoReadStart,
			/* formatSelectorReadContinue */		DoReadContinue,
			/* formatSelectorReadFinish */			DoReadFinish,
			
			/* formatSelectorOptionsPrepare */		DoOptionsPrepare,
			/* formatSelectorOptionsStart */		DoOptionsStart,
			/* formatSelectorOptionsContinue */		DoOptionsContinue,
			/* formatSelectorOptionsFinish */		DoOptionsFinish,
			
			/* formatSelectorEstimatePrepare */		DoEstimatePrepare,
			/* formatSelectorEstimateStart */		DoEstimateStart,
			/* formatSelectorEstimateContinue */	DoEstimateContinue,
			/* formatSelectorEstimateFinish */		DoEstimateFinish,
			
			/* formatSelectorWritePrepare */		DoWritePrepare,
			/* formatSelectorWriteStart */			DoWriteStart,
			/* formatSelectorWriteContinue */		DoWriteContinue,
			/* formatSelectorWriteFinish */			DoWriteFinish,
			
			/* formatSelectorFilterFile */			DoFilterFile
		};
		
		Ptr globalPtr = NULL;		// Pointer for global structure
		GPtr globals = NULL;		// actual globals

		//-----------------------------------------------------------------------
		//	(4) Allocate and initalize globals.
		//-----------------------------------------------------------------------
		
		globalPtr = AllocateGlobals ((uint32)result,
									 (uint32)formatParamBlock,
									 formatParamBlock->handleProcs,
									 sizeof(Globals),
						 			 data,
						 			 InitGlobals);
		
		if (globalPtr == NULL)
		{ // Something bad happened if we couldn't allocate our pointer.
		  // Fortunately, everything's already been cleaned up,
		  // so all we have to do is report an error.
		  
		  *result = memFullErr;
		  return;
		}
		
		// Get our "globals" variable assigned as a Global Pointer struct with the
		// data we've returned:
		globals = (GPtr)globalPtr;


		//-----------------------------------------------------------------------
		//	(5) Dispatch selector.
		//-----------------------------------------------------------------------

		if (selector > formatSelectorAbout && selector <= formatSelectorFilterFile)
				(routineForSelector[selector-1])(globals); // dispatch using jump table
		else	gResult = formatBadParameters;
		

		//-----------------------------------------------------------------------
		//	(6) Unlock data, and exit resource.
		//-----------------------------------------------------------------------	
		
		// unlock handle pointing to parameter block and data so it can move
		// if memory gets shuffled:
		if ((Handle)*data != NULL)
			PIUnlockHandle((Handle)*data);
	
	} // about selector special		
	
	#ifdef UseA4			// Are we in 68k Mac MW?
		ExitCodeResource(); // A4-globals
	#endif
	
} // end ENTRYPOINT


//-------------------------------------------------------------------------------
//
//	InitGlobals
//	
//	Initalize any global values here.  Called only once when global
//	space is reserved for the first time.
//
//-------------------------------------------------------------------------------

void InitGlobals (Ptr globalPtr)
{	
	// create "globals" as a our struct global pointer so that any macros work:
	GPtr globals = (GPtr)globalPtr;

	// Initialize global variables:
	ValidateParameters (globals);
	
} // end InitGlobals


//-------------------------------------------------------------------------------
//
//	ValidateParameters
//
//	Initialize parameters to default values.
//
//-------------------------------------------------------------------------------

void ValidateParameters (GPtr globals)
{
	int i;

	gPixelBuffer	= NULL;
	gPixelData		= NULL;	
	gRowBytes		= 0;

	for(i=0; i<PI_MAX; i++)
		gIntProps[i] = IntPropDefault[i];

#ifdef USE_DIALOG
	gList			= NULL;
	gDialog			= NULL;
#endif

	PIResetString(gHistory);
}


/*****************************************************************************/

static void DoFilterFile (GPtr globals)
{
	FileHeader header;
	LogWrite( "BEGIN: ", "DoFilterFile" );

	// Exit if we have already encountered an error
	if (gResult != noErr) return;

	// Read the file header.
	if (!TSR (SetFPos (gStuff->dataFork, fsFromStart, 0))) return;

	ReadSome (globals, sizeof (FileHeader), &header);

	if (gResult != noErr) return;

	// Check the identifier.
	if( !CheckIdentifier( header.bfMagic )
		||	header.biBitCount != 16
		||	header.biPlanes != 1
		||	header.biCompression != 0 )
	{
		gResult = formatCannotRead;
		return;
	}
}


/*****************************************************************************/

static void DoReadPrepare (GPtr globals)
{
	LogWrite( "BEGIN: ", "DoReadPrepare" );
	gStuff->maxData = 0;
}


static void DoReadStart (GPtr globals)
{
	FileHeader header;
	LogWrite( "BEGIN: ", "DoReadStart" );

	// override params here
	ReadScriptParamsOnRead (globals); 
    if (gResult != noErr) return;
		
	// Check for the needed services. 
	if (!TSC ((Boolean)(!CheckForServices (globals)))) return;

	// If we have not encountered an error, then we want to read the file header. 
	if (!TSR (SetFPos (gStuff->dataFork, fsFromStart, 0))) return;

	ReadSome (globals, sizeof (header.bfMagic),				&header.bfMagic);
	ReadSome (globals, sizeof (header.bfSize),				&header.bfSize);
	ReadSome (globals, sizeof (header.bfReserved),			&header.bfReserved);
	ReadSome (globals, sizeof (header.bfOffset),			&header.bfOffset);

	ReadSome (globals, sizeof (header.biSize),				&header.biSize);
	ReadSome (globals, sizeof (header.biWidth),				&header.biWidth);
	ReadSome (globals, sizeof (header.biHeight),			&header.biHeight);
	ReadSome (globals, sizeof (header.biPlanes),			&header.biPlanes);
	ReadSome (globals, sizeof (header.biBitCount),			&header.biBitCount);
	ReadSome (globals, sizeof (header.biCompression),		&header.biCompression);
	ReadSome (globals, sizeof (header.biSizeImage),			&header.biSizeImage);
	ReadSome (globals, sizeof (header.biXPelsPerMeter),		&header.biXPelsPerMeter);
	ReadSome (globals, sizeof (header.biYPelsPerMeter),		&header.biYPelsPerMeter);
	ReadSome (globals, sizeof (header.biClrUsed),			&header.biClrUsed);
	ReadSome (globals, sizeof (header.biClrImportant),		&header.biClrImportant);

	if (gResult != noErr) return;

	if( !CheckIdentifier( header.bfMagic )
	||	header.biBitCount != 16
	||	header.biPlanes != 1
	||	header.biCompression != 0 )
	{
		gResult = formatCannotRead;
		return;
	}

	gStuff->imageMode	= plugInModeGray16;
	gStuff->imageSize.v	= (short)	header.biHeight;
	gStuff->imageSize.h	= (short)	header.biWidth;
	gStuff->depth		= (short)	header.biBitCount;
	gStuff->planes		= (short)	header.biPlanes;
}

static void DoReadContinue (GPtr globals)
{
	int32 done;
	int32 total;
	int16 row;

	LogWrite( "BEGIN: ", "DoReadContinue" );

	// Dispose of the image resource data if it exists. 	
	DisposeImageResources (globals);

	// Set up the progress variables. 	
	total	= gStuff->imageSize.v;
	done	= 0;

	// Next, we will allocate the pixel buffer.	
	AllocatePixelBuffer (globals);

	// Set up to start returning chunks of data. 

	gStuff->data				= gPixelData;
	gStuff->theRect.left		= 0;
	gStuff->theRect.right		= gStuff->imageSize.h;
	gStuff->colBytes			= 2;
	gStuff->rowBytes			= gRowBytes;
	gStuff->planeBytes			= 0;
	gStuff->loPlane				= 0;
	gStuff->hiPlane				= 0;

	for (row = 0; gResult == noErr && row < gStuff->imageSize.v; ++row)
	{
		gStuff->theRect.top		= gStuff->imageSize.v - row - 1;
		gStuff->theRect.bottom	= gStuff->imageSize.v - row;

		ReadRow (globals);

		if (gResult == noErr)
			gResult = AdvanceState ();

		UpdateProgress (++done, total);
	}

	gStuff->data = NULL;
	DisposePixelBuffer (globals);
}


static void DoReadFinish (GPtr globals)
{	
	LogWrite( "BEGIN: ", "DoReadFinish" );
	DisposeImageResources (globals);	// Dispose of the image resource data if it exists.
	WriteScriptParamsOnRead (globals);	// should be different for read/write
	AddComment (globals);				// write a history comment
}


/*****************************************************************************/

static void DoOptionsPrepare (GPtr globals)
{
	gStuff->maxData = 0;
}

static void DoOptionsStart (GPtr globals)
{
	// Check for the needed services. */
	if (!TSC ((Boolean)(!CheckForServices (globals)))) return;

	gStuff->data = NULL;

	if( ReadScriptParamsOnWrite (globals) == TRUE )
	{
#ifdef USE_DIALOG
		// Open the dialog.  If it is already up, this does nothing.       
		if (!DialogOpen (globals))
		{
			gResult = nilHandleErr;
			return;
		}

		// Run the parameters dialog.	
		if (!DialogRun (globals))
		{
			DialogClose (globals);
			gResult = userCanceledErr;
			return;
		}
		else
		{
			DialogClose (globals);
		}
#endif
	}
}


static void DoOptionsContinue (GPtr globals)
{
	#ifdef __PIMWCW__
		#pragma unused (globals) // remove this when you write this routine
	#endif
}


static void DoOptionsFinish (GPtr globals)
{
	#ifdef __PIMWCW__
		#pragma unused (globals) // remove this when you write this routine
	#endif
}


/*****************************************************************************/

static void DoEstimatePrepare (GPtr globals)
{
	gStuff->maxData = 0;
}

static void DoEstimateStart (GPtr globals)
{
	int32 dataBytes;
	
	// Check for the needed services.	
	if (!TSC ((Boolean)(!CheckForServices (globals)))) return;

	dataBytes = sizeof (FileHeader) + (gStuff->imageSize.h * gStuff->imageSize.v * 2);
					  		
	gStuff->minDataBytes = dataBytes;
	gStuff->maxDataBytes = dataBytes;	
	gStuff->data = NULL;
}

static void DoEstimateContinue (GPtr globals)
{
	#ifdef __PIMWCW__
		#pragma unused (globals) // remove this when you write this routine
	#endif
}

static void DoEstimateFinish (GPtr globals)
{
	#ifdef __PIMWCW__
		#pragma unused (globals) // remove this when you write this routine
	#endif
}


/*****************************************************************************/

static void DoWritePrepare (GPtr globals)
{
	gStuff->maxData = 0;	
}


static void DoWriteStart (GPtr globals)
{
	FileHeader	header;
	int32		done, total;
	int16		row;

	//MessageBox(NULL, "DoWriteStart", "Func", MB_OK );

    if (gResult != noErr) return;
		
	// Check for the needed services. 	
	if (!TSC ((Boolean)(!CheckForServices (globals)))) return;

	// Write the header. 	
	gResult = SetFPos (gStuff->dataFork, fsFromStart, 0);

	SetIdentifier			( header.bfMagic );
	header.bfSize			= sizeof (FileHeader) + (gStuff->imageSize.h * gStuff->imageSize.v)*2;
	header.bfReserved		= 0;
	header.bfOffset			= sizeof (FileHeader); 

	header.biSize			= 40;//sizeof (FileHeader) - sizeof (header.bfMagic) - sizeof (header.bfSize) - sizeof (header.bfReserved) - sizeof (header.bfOffset);
	header.biWidth			= gStuff->imageSize.h;
	header.biHeight			= gStuff->imageSize.v;
	header.biPlanes			= 1;
	header.biBitCount		= 16;
	header.biCompression	= 0;
	header.biSizeImage		= gStuff->imageSize.h * gStuff->imageSize.v * 2;
	header.biXPelsPerMeter	= 0;
	header.biYPelsPerMeter	= 0;
	header.biClrUsed		= 0;
	header.biClrImportant	= 0;

	WriteSome (globals, sizeof (header.bfMagic),			&header.bfMagic);
	WriteSome (globals, sizeof (header.bfSize),				&header.bfSize);
	WriteSome (globals, sizeof (header.bfReserved),			&header.bfReserved);
	WriteSome (globals, sizeof (header.bfOffset),			&header.bfOffset);

	WriteSome (globals, sizeof (header.biSize),				&header.biSize);
	WriteSome (globals, sizeof (header.biWidth),			&header.biWidth);
	WriteSome (globals, sizeof (header.biHeight),			&header.biHeight);
	WriteSome (globals, sizeof (header.biPlanes),			&header.biPlanes);
	WriteSome (globals, sizeof (header.biBitCount),			&header.biBitCount);
	WriteSome (globals, sizeof (header.biCompression),		&header.biCompression);
	WriteSome (globals, sizeof (header.biSizeImage),		&header.biSizeImage);
	WriteSome (globals, sizeof (header.biXPelsPerMeter),	&header.biXPelsPerMeter);
	WriteSome (globals, sizeof (header.biYPelsPerMeter),	&header.biYPelsPerMeter);
	WriteSome (globals, sizeof (header.biClrUsed),			&header.biClrUsed);
	WriteSome (globals, sizeof (header.biClrImportant),		&header.biClrImportant);

	if (gResult != noErr) return;
		
	// Set up the progress variables.
	total	= gStuff->imageSize.v;
	done	= 0;
	row		= 0;
		
	// Next, we will allocate the pixel buffer.
	AllocatePixelBuffer (globals);
		
	// Set up to start receiving chunks of data.
	gStuff->data			= gPixelData;
	gStuff->theRect.left	= 0;
	gStuff->theRect.right	= gStuff->imageSize.h;
	gStuff->colBytes		= 2;
	gStuff->rowBytes		= gRowBytes;
	gStuff->planeBytes		= 0;
	gStuff->loPlane			= 0;
	gStuff->hiPlane			= 0;

	// save pixels 
	for (row = 0; row<gStuff->imageSize.v && gResult==noErr; ++row)
	{
		gStuff->theRect.top		= gStuff->imageSize.v - row - 1;
		gStuff->theRect.bottom	= gStuff->imageSize.v - row;

		if (gResult == noErr)
			gResult = AdvanceState ();

		WriteRow (globals);
		UpdateProgress (++done, total);
	}


	// cleanup
	gStuff->data = NULL;	
	DisposePixelBuffer (globals);
}


static void DoWriteContinue (GPtr globals)
{
	#ifdef __PIMWCW__
		#pragma unused (globals) // remove this when you write this routine
	#endif
}


static void DoWriteFinish (GPtr globals)
{
	WriteScriptParamsOnWrite (globals); // should be different for read/write
}



/*****************************************************************************/

static void ReadSome (GPtr globals, int32 count, void *buffer)
{
	int32 readCount = count;

	if (gResult != noErr)
		return;

	gResult = FSRead (gStuff->dataFork, &readCount, buffer);
	if (gResult == noErr && readCount != count)
		gResult = eofErr;
}


static void WriteSome (GPtr globals, int32 count, void *buffer)
{
	int32 writeCount = count;

	if (gResult != noErr)
		return;

	gResult = FSWrite (gStuff->dataFork, &writeCount, buffer);
	if (gResult == noErr && writeCount != count)
		gResult = dskFulErr;	
}


/*****************************************************************************/

static void ReadRow (GPtr globals)
{	
	ReadSome (globals, gRowBytes, gPixelData);	
}

static void WriteRow (GPtr globals)
{
	uint16*	pBuffer;
	double	scale;
	int		i;

	scale	= 32768.0/65535.0;					// the scale for PS 0-32768 values
	pBuffer	= malloc(gRowBytes);				// alloc pBuffer
	memcpy(pBuffer,gPixelData,gRowBytes);		// fill pBuffer

	for(i=0;i<gRowBytes/2;++i)
	{ 
		pBuffer[i] = rint((pBuffer[i] / (scale)));	// magic scaling formula, output values exactly like PS .raw 16bpp 
		if( gIntProps[PI_swapBytes].value == 1 )	// swap when needed
			pBuffer[i] = SwapBytes(pBuffer[i]);
	}

	WriteSome (globals, gRowBytes, pBuffer);	// write to file
	free(pBuffer);								// free pBuffer
}


/*****************************************************************************/

static int32 RowBytes (GPtr globals)
{
	return ( (gStuff->imageSize.h * (int32) gStuff->depth) >> 3 );
}


static Boolean AllocatePixelBuffer (GPtr globals)
{
	BufferID buffer;

	if (gResult != noErr) return FALSE;

	// We will want a buffer that is one line wide.
	gPixelBuffer	= 0;
	gRowBytes		= RowBytes (globals);

	if (!TSR (AllocateBuffer (gRowBytes, &buffer))) return FALSE;

	gPixelBuffer	= buffer;
	gPixelData		= LockBuffer (gPixelBuffer, TRUE);

	return TRUE;
}


static void DisposePixelBuffer (GPtr globals)
{
	if (gPixelBuffer != 0)
	{
		FreeBuffer( gPixelBuffer );
		gPixelBuffer	= 0;
		gPixelData		= 0;
	}
}


/*****************************************************************************/

// This routine adds a history entry to the file when incoming.
static void AddComment (GPtr globals)
{
	// We will attempt to add a resource showing that we made this change.
	
	Str255 s, s0;
	Handle h;
	
	if (!ResourceProcsAvailable (NULL))
		return;

	// Pulls a pascal string from the resource tree.
	PIGetString(kHistoryEntry, s);

	NumToString(gStuff->dataFork, s0);

	// Need some unique data for this entry
	PIParamText(s, s0, NULL, NULL);

	h = PIPString2Handle(s);
		
	if (h != NULL)
	{
		PILockHandle(h, FALSE);
		(void) AddPIResource (histResource, h);
		PIUnlockHandle(h);
		PIDisposeHandle(h);
		h = NULL;
	}
	
	if (gHistory[0] > 0)
	{
		// we had an old history.  Write that back out, as well.
		h = PIPString2Handle(gHistory);
		
		if (h != NULL)
		{
			// got our handle. Write it out.
			gHistory[ (gHistory[0]=0)+1 ] = 0; // clear string
			PILockHandle(h, FALSE);
			(void) AddPIResource (histResource, h);
			PIUnlockHandle(h);
			PIDisposeHandle(h);
			h = NULL;
		}
	} 
}


// Returns a history entry in a string
static void GetHistory (GPtr globals, int16 index, Str255 s)
{	
	int16 currentResources = CountPIResources(histResource);
	
	PIResetString(s);
	
	if (currentResources >= index)
	{
		Handle h = GetPIResource (histResource, index);	
		if (h != NULL) PIHandle2PString(h, s);
		// just a read-only reference.  Do NOT dispose.
	}
}


/*****************************************************************************/

static Boolean CheckIdentifier (char identifier [])
{
	return	identifier[0] == 'B' &&
			identifier[1] == 'M';
}

static void SetIdentifier (char identifier [])
{
	identifier[0] = 'B';
	identifier[1] = 'M';
}


/*****************************************************************************/

static Boolean CheckForServices (GPtr globals)
{
	return	WarnBufferProcsAvailable() &&
			WarnAdvanceStateAvailable() &&
			WarnHandleProcsAvailable();
}

/*****************************************************************************/

static void DisposeImageResources (GPtr globals)
{
	if (gStuff->imageRsrcData)
	{
		PIDisposeHandle (gStuff->imageRsrcData);
		gStuff->imageRsrcData = NULL;
		gStuff->imageRsrcSize = 0;
	}
}


/*****************************************************************************/

int32 rint( double dbl )
{       
	return (int32)(dbl+0.5);
}

int16 SwapBytes( int16 s )
{
	uint8 b1, b2;

	b1 = s & 255;
	b2 = (s >> 8) & 255;
	return (b1 << 8) + b2;
}


/*****************************************************************************/

#ifdef USE_LOG

static void LogNuke( void )
{
	remove ( LOG_FILE );
}

static void LogWrite( const char* name, const char* value )
{
	FILE *f;
	f = fopen ( LOG_FILE, "at+" );
	fprintf(f,"%s%s\n",name,value);
	fclose ( f );
}

#endif //ifdef USE_LOG

/*****************************************************************************/
