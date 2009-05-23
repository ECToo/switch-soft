//-------------------------------------------------------------------------------
//
//	G16 BMP File Format plugin for Photoshop 5.0+
//	Copyright 2004 neai.o2.pl
//
//-------------------------------------------------------------------------------
//
//	File:
//		G16.r
//
//	Copyright 1993-1997, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Description:
//		This file contains the resource definitions for the
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
//	Definitions -- Required by include files.
//-------------------------------------------------------------------------------

// The About box and resources are created in DialogUtilities.r.
// You can easily override them, if you like.

#define plugInName			"G16"
#define plugInCopyrightYear	"1993"
#define plugInDescription \
	"G16 BMP file format plugin\nCopyright © 2004 neai.o2.pl"

//-------------------------------------------------------------------------------
//	Definitions -- Required by other resources in this rez file.
//-------------------------------------------------------------------------------

// Dictionary (aete) resources:

#define vendorName			"AdobeSDK"
#define plugInAETEComment 	"G16 BMP file format plugin"

#define plugInSuiteID		'sdK4'
#define plugInClassID		'simP'
#define plugInEventID		typeNull // must be this

#define plugInPriority		-1

//-------------------------------------------------------------------------------
//	Set up included files for Macintosh and Windows.
//-------------------------------------------------------------------------------

#include "PIDefines.h"

#ifdef __PIMac__
	#include "Types.r"
	#include "SysTypes.r"
	#include "PIGeneral.r"
	#include "PIUtilities.r"
	#include "DialogUtilities.r"
#elif defined(__PIWin__)
	#include "PIGeneral.h"
	#include "PIUtilities.r"
	#include "WinDialogUtils.r"
#endif

#include "PIActions.h"

#include "G16Terminology.h"	// Terminology for plug-in.

//-------------------------------------------------------------------------------
//	PiPL resource
//-------------------------------------------------------------------------------

resource 'PiPL' (ResourceID, plugInName " PiPL", purgeable)
{
    {
		Kind { ImageFormat },
		Name { plugInName },
		Version { (latestFormatVersion << 16) | latestFormatSubVersion },

		#if Macintosh
		Code68K { ImageFormat, $$ID },
		CodePowerPC { 0, 0, "" },
		#endif

		#if MSWindows
		CodeWin32X86 { "ENTRYPOINT" },
		#endif
	
		// ClassID, eventID, aete ID, uniqueString:
		HasTerminology { plugInClassID, plugInEventID, ResourceID, vendorName " " plugInName },
		
		Priority { 65535 },
		
		SupportedModes
		{
			noBitmap, doesSupportGrayScale,
			noIndexedColor, noRGBColor,
			noCMYKColor, noHSLColor,
			noHSBColor, noMultichannel,
			noDuotone, noLABColor
		},	
			
		EnableInfo { "PSHOP_ImageMode == Gray16Mode" },
	
		FmtFileType { '8B1F', '8BIM' },
		ReadTypes { { '8B1F', '    ' } },
		FilteredTypes { { '8B1F', '    ' } },
		ReadExtensions { { 'BMP ', 'G16 ' } },
		WriteExtensions { { 'BMP ', 'G16 ' } },
		FilteredExtensions { { 'BMP ', 'G16 ' } },
		FormatFlags { fmtDoesNotSaveImageResources, fmtCanRead, fmtCanWrite, fmtCanWriteIfRead },
		/* canRead, canWrite, canWriteIfRead, savesResources */
		FormatMaxSize { { 32767, 32767 } },
		FormatMaxChannels { {   0, 1, 0, 0, 0, 0, 
							   0, 0, 0, 0, 0, 0 } }
		}
	};


//-------------------------------------------------------------------------------
//	PiMI resource (kept for backward compatibility)
//-------------------------------------------------------------------------------

resource 'PiMI' (ResourceID, plugInName " PiMI", purgeable)
{
	latestFormatVersion, 	/* Version, subVersion, and priority of the interface */
	latestFormatSubVersion,
	0,
	supportsGrayScale,			/* Supported Image Modes */
	'    ',						/* Required host */
	
	{
		canRead,
		cannotReadAll,
		canWrite,
		canWriteIfRead,
		savesResources,
		{  0, 1, 0, 0,		/* Maximum # of channels for each plug-in mode */
		  0, 0, 0, 0,
		  0, 0,  0,  0,
		   0,  0,  0,  0 },
		32767,				/* Maximum rows allowed in document */
		32767,				/* Maximum columns allowed in document */
		'8B1F',				/* The file type if we create a file. */
		'8BIM',				/* The creator type if we create a file. */
		{					/* The type-creator pairs supported. */
			'8B1F', '    '
		},
		{					/* The extensions supported. */
		}
	},
	
};

//-------------------------------------------------------------------------------
//	Dictionary (scripting) resource
//-------------------------------------------------------------------------------

resource 'aete' (ResourceID, plugInName " dictionary", purgeable)
{
	1, 0, english, roman,									/* aete version and language specifiers */
	{
		vendorName,											/* vendor suite name */
		"G16 BMP File Format",								/* optional description */
		plugInSuiteID,										/* suite ID */
		1,													/* suite code, must be 1 */
		1,													/* suite level, must be 1 */
		{},													/* structure for filters */
		{													/* non-filter plug-in class here */
			vendorName " G16",								/* unique class name */
			plugInClassID,									/* class ID, must be unique or Suite ID */
			plugInAETEComment,								/* optional description */
			{												/* define inheritance */
				"<Inheritance>",							/* must be exactly this */
				keyInherits,								/* must be keyInherits */
				classFormat,								/* parent: Format, Import, Export */
				"parent class format",						/* optional description */
				flagsSingleProperty,						/* if properties, list below */
							
				"G16 SwapBytes",
				keySwapBytes,
				typeInteger,
				"Swap Bytes",
				flagsSingleProperty
				
				/* no properties */
			},
			{}, /* elements (not supported) */
			/* class descriptions */
		},
		{}, /* comparison ops (not supported) */
		{}	/* any enumerations */
	}
};


//-------------------------------------------------------------------------------
//	History resource
//-------------------------------------------------------------------------------

resource StringResource (kHistoryEntry, "History", purgeable)
{
	plugInName ": ref num=^0."
};


//-------------------------------------------------------------------------------
//	Dialog resource
//-------------------------------------------------------------------------------

resource 'DLOG' (ResourceID+1, plugInName " UI", purgeable)
{
	{0, 0, 206, 255},
	movableDBoxProc,
	visible,
	noGoAway,
	0x0,
	ResourceID+1,
	plugInName,
	centerParentWindowScreen
};

resource 'DITL' (ResourceID+1, plugInName " UI", purgeable)
{
	{
	}
};

//-------------------------------------------------------------------------------

// end G16.r
