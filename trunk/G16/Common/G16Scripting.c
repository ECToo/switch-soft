//-------------------------------------------------------------------------------
//
//	G16 BMP File Format plugin for Photoshop 5.0+
//	Copyright © 2004 neai.o2.pl
//
//-------------------------------------------------------------------------------
//
//	File:
//		G16Scripting.c
//
//	Copyright 1996-1997, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Description:
//		Scripting support
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
#else
	#include "G16.h"
#endif


//-------------------------------------------------------------------------------
//
//	ReadScriptParamsOnRead
//
//	Checks the parameters against scripting-returned parameters, if any, and
//	updates the globals to match ones provided by the scripting system.
//
//	Inputs:
//		GPtr globals		Pointer to global structure.
//
//	Outputs:
//		returns TRUE		If you should pop your dialog.
//		returns FALSE		If you should not pop your dialog.
//
//		gResult				Will return any fatal error.
//
//-------------------------------------------------------------------------------


Boolean ReadScriptParamsOnRead (GPtr globals)
{
	PIReadDescriptor			token = NULL;
	DescriptorKeyID				key = 0;
	DescriptorTypeID			type = 0;
	OSType						shape = 0, create = 0;
	DescriptorKeyIDArray		array = { NULLID };
	int32						flags = 0;
	OSErr						gotErr = noErr, stickyError = noErr;
	Boolean						returnValue = true;
	
	if (DescriptorAvailable())
	{ /* playing back.  Do our thing. */
		token = OpenReader(array);
		if (token)
		{
			while (PIGetKey(token, &key, &type, &flags))
			{
				
				switch (key)
				{
					case keySwapBytes:
						PIGetInt(token, &gIntProps[PI_swapBytes].value);
						break;					// check keys here
				}
			}

			stickyError = CloseReader(&token); // closes & disposes.
				
			if (stickyError)
			{
				if (stickyError == errMissingParameter) // missedParamErr == -1715
					;
					/* (descriptorKeyIDArray != NULL)
					   missing parameter somewhere.  Walk IDarray to find which one. */
				else
					gResult = stickyError;
			}
		}
		
		returnValue = PlayDialog();
		/* return TRUE if want to show our Dialog */		
	}
	
	return returnValue;
}
		
//-------------------------------------------------------------------------------
//
//	ReadScriptParamsOnWrite
//
//	Checks the parameters against scripting-returned parameters, if any, and
//	updates the globals to match ones provided by the scripting system.
//
//	Inputs:
//		GPtr globals		Pointer to global structure.
//
//	Outputs:
//		returns TRUE		If you should pop your dialog.
//		returns FALSE		If you should not pop your dialog.
//
//		gResult				Will return any fatal error.
//
//-------------------------------------------------------------------------------


Boolean ReadScriptParamsOnWrite (GPtr globals)
{
	PIReadDescriptor			token = NULL;
	DescriptorKeyID				key = 0;
	DescriptorTypeID			type = 0;
	OSType						shape = 0, create = 0;
	DescriptorKeyIDArray		array = { NULLID };
	int32						flags = 0;
	OSErr						gotErr = noErr, stickyError = noErr;
	Boolean						returnValue = true;


	if (DescriptorAvailable())
	{ /* playing back.  Do our thing. */
		token = OpenReader(array);

		//MessageBox(NULL, "ReadScriptParamsOnWrite", "", MB_OK );

		if (token)
		{
			while (PIGetKey(token, &key, &type, &flags))
			{
				switch (key)
				{
					case keySwapBytes:
						PIGetInt(token, &gIntProps[PI_swapBytes].value);
						break;
					// check other keys here
				}
			}


			stickyError = CloseReader(&token); // closes & disposes.
				
			if (stickyError)
			{
				if (stickyError == errMissingParameter) // missedParamErr == -1715
					;
					/* (descriptorKeyIDArray != NULL)
					   missing parameter somewhere.  Walk IDarray to find which one. */
				else
					gResult = stickyError;
			}
		}
		
		returnValue = PlayDialog();
		/* return TRUE if want to show our Dialog */		
	}
	
	return returnValue;
}

//-------------------------------------------------------------------------------
//
//	WriteScriptParamsOnRead
//
//	Takes our parameters from our global variables and writes them out to the
//	scripting system, which hands it all back to the host.
//
//	Inputs:
//		GPtr globals			Pointer to global structure.
//
//		Boolean gFooRead		Boolean to write to scripting system.
//
//	Outputs:
//		returns an OSErr		If there was a serious error.
//		returns noErr			If everything worked fine.
//
//-------------------------------------------------------------------------------

OSErr WriteScriptParamsOnRead (GPtr globals)
{
	PIWriteDescriptor			token = nil;
	OSErr						gotErr = noErr;
			
	if (DescriptorAvailable())
	{ /* recording.  Do our thing. */
		token = OpenWriter();
		if (token)
		{
			// write keys here
			PIPutInt(token, keySwapBytes, gIntProps[PI_swapBytes].value);
			gotErr = CloseWriter(&token); /* closes and sets dialog optional */
			/* done.  Now pass handle on to Photoshop */
		}
	}
	return gotErr;
}

		
//-------------------------------------------------------------------------------
//
//	WriteScriptParamsOnWrite
//
//	Takes our parameters from our global variables and writes them out to the
//	scripting system, which hands it all back to the host.
//
//	Inputs:
//		GPtr globals			Pointer to global structure.
//
//		Boolean gBarWrite		Boolean to write to scripting system.
//
//	Outputs:
//		returns an OSErr		If there was a serious error.
//		returns noErr			If everything worked fine.
//
//-------------------------------------------------------------------------------

OSErr WriteScriptParamsOnWrite (GPtr globals)
{
	PIWriteDescriptor			token = nil;
	OSErr						gotErr = noErr;
			
	if (DescriptorAvailable())
	{
		// recording.  Do our thing.
		token = OpenWriter();
		if (token)
		{
			// write keys here
			PIPutInt(token, keySwapBytes, gIntProps[PI_swapBytes].value);
			gotErr = CloseWriter(&token); // closes and sets dialog optional 
			// done.  Now pass handle on to Photoshop
		}
	}
	return gotErr;
}

//-------------------------------------------------------------------------------

// end G16Scripting.c
