/**********************************************************************
 *<
	FILE: U3DExport.cpp

	DESCRIPTION:	Appwizard generated plugin

	CREATED BY: 

	HISTORY: 

 *>	Copyright (c) 2000, All Rights Reserved.
 **********************************************************************/

#include "U3DExport.h"
#include "U3DFormat.h"
#include "decomp.h"

#include "IGame.h"
#include "IGameObject.h"
#include "IGameProperty.h"
#include "IGameControl.h"
#include "IGameModifier.h"
#include "IConversionManager.h"
#include "IGameError.h"

#define U3DEXPORT_CLASS_ID	Class_ID(0x5aa85d9b, 0x5b6d0069)











class U3DExport : public SceneExport {
	public:

		IGameScene * pIgame;

		static HWND hParams;
		FILE*		pModel;
		FILE*		pAnim;
		
		
		int curNode;

		int staticFrame;
		int framePerSample;
		BOOL exportGeom;
		BOOL exportNormals;
		BOOL exportVertexColor;
		BOOL exportControllers;
		BOOL exportFaceSmgp;
		BOOL exportTexCoords;
		BOOL exportMappingChannel;
		BOOL exportConstraints;
		BOOL exportMaterials;
		BOOL exportSplines;
		BOOL exportModifiers;
		BOOL exportSkin;
		BOOL exportGenMod;
		BOOL forceSample;
		BOOL splitFile;
		BOOL exportQuaternions;
		BOOL exportObjectSpace;
		BOOL exportRelative;
		BOOL exportNormalsPerFace;
		int cS;
		int exportCoord;
		bool showPrompts;
		bool exportSelected;

		FJSDataHeader DataHeader;
		FJSAnivHeader AnivHeader;

		Tab<FMeshVert> Verts;
		Tab<FJSMeshTri> Tris;

		float igameVersion, exporterVersion;




		int				ExtCount();					// Number of extensions supported
		const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
		const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
		const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
		const TCHAR *	AuthorName();				// ASCII Author name
		const TCHAR *	CopyrightMessage();			// ASCII Copyright message
		const TCHAR *	OtherMessage1();			// Other message #1
		const TCHAR *	OtherMessage2();			// Other message #2
		unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
		void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

		BOOL SupportsOptions(int ext, DWORD options);
		int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);


		
		//Constructor/Destructor

		U3DExport();
		~U3DExport();		


		//

		void ExportChildNodeInfo( IGameNode * child);

		void MakeSplitFilename(IGameNode * node, TSTR & buf);
		void makeValidURIFilename(TSTR&, bool = false);
		BOOL ReadConfig();
		void WriteConfig();
		TSTR GetCfgFilename();

};


class U3DExportClassDesc:public ClassDesc2 {
	public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new U3DExport(); }
	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
	SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID() { return U3DEXPORT_CLASS_ID; }
	const TCHAR* 	Category() { return GetString(IDS_CATEGORY); }

	const TCHAR*	InternalName() { return _T("U3DExport"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }				// returns owning module handle

};



static U3DExportClassDesc U3DExportDesc;
ClassDesc2* GetU3DExportDesc() { return &U3DExportDesc; }


BOOL CALLBACK U3DExportOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static U3DExport *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (U3DExport *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;
	}
	return FALSE;
}


//--- U3DExport -------------------------------------------------------
U3DExport::U3DExport()
: DataHeader(FJSDataHeader())
, AnivHeader(FJSAnivHeader())
, pModel(NULL)
, pAnim(NULL)
{
}

U3DExport::~U3DExport() 
{
	if( pModel != NULL )
		fclose(pModel);

	if( pAnim != NULL )
		fclose(pAnim);

}

int U3DExport::ExtCount()
{
	//TODO: Returns the number of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR *U3DExport::Ext(int n)
{		
	//TODO: Return the 'i-th' file name extension (i.e. "3DS").
	return _T("3d");
}

const TCHAR *U3DExport::LongDesc()
{
	//TODO: Return long ASCII description (i.e. "Targa 2.0 Image File")
	return _T("Unreal 3D Vertex Mesh");
}
	
const TCHAR *U3DExport::ShortDesc() 
{			
	//TODO: Return short ASCII description (i.e. "Targa")
	return _T("Unreal 3D");
}

const TCHAR *U3DExport::AuthorName()
{			
	//TODO: Return ASCII Author name
	return _T("Roman Dzieciol");
}

const TCHAR *U3DExport::CopyrightMessage() 
{	
	// Return ASCII Copyright message
	return _T("Copyright (C) 2007");
}

const TCHAR *U3DExport::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *U3DExport::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int U3DExport::Version()
{				
	//TODO: Return Version number * 100 (i.e. v3.01 = 301)
	return 100;
}

void U3DExport::ShowAbout(HWND hWnd)
{			
	// Optional
}

BOOL U3DExport::SupportsOptions(int ext, DWORD options)
{
	// TODO Decide which options to support.  Simply return
	// true for each option supported by each Extension 
	// the exporter supports.

	return TRUE;
}


// Dummy function for progress bar
DWORD WINAPI fn(LPVOID arg)
{
	return(0);
}



class MyErrorProc : public IGameErrorCallBack
{
public:
	void ErrorProc(IGameError error)
	{
		TCHAR * buf = GetLastIGameErrorText();
		DebugPrint("ErrorCode = %d ErrorText = %s\n", error,buf);
	}
};


#include "utilapi.h"

int	U3DExport::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	//TODO: Implement the actual file Export here and 
	//		return TRUE If the file is exported properly

	/*if(!suppressPrompts)
		DialogBoxParam(hInstance, 
				MAKEINTRESOURCE(IDD_PANEL), 
				GetActiveWindow(), 
				U3DExportOptionsDlgProc, (LPARAM)this);
	return FALSE;

	*/


	Interface * ip = GetCOREInterface();

	MyErrorProc pErrorProc;

	UserCoord Whacky = {
		1,	//Right Handed
		1,	//X axis goes right
		3,	//Y Axis goes down
		4,	//Z Axis goes in.
		1,	//U Tex axis is right
		1,  //V Tex axis is Down
	};	

	SetErrorCallBack(&pErrorProc);

	ReadConfig();
   
	// Set a global prompt display switch
	//showPrompts = suppressPrompts ? FALSE : TRUE;
	//exportSelected = (options & SCENE_EXPORT_SELECTED) ? true : false;

	igameVersion  = GetIGameVersion();

	DebugPrint("3ds max compatible version %.2f%\n",GetSupported3DSVersion());


	/*if(showPrompts) 
	{
		// Prompt the user with our dialogbox, and get all the options.
		if (!DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_PANEL),
			i->GetMAXHWnd(), IGameExporterOptionsDlgProc, (LPARAM)this)) {
			return 1;
		}
	}*/


	

	TSTR filepath;
	TSTR filename;
	TSTR fileext;
	SplitFilename(TSTR(name), &filepath, &filename, &fileext);

	TSTR modelfilename;
	TSTR animfilename;
	modelfilename = filepath + _T("\\") + filename + TSTR(_T("_d")) + fileext;
	animfilename = filepath + _T("\\") + filename + TSTR(_T("_a")) + fileext;

	// Open the stream
	pModel = _tfopen(modelfilename,_T("wb"));
	if (!pModel) {
		TSTR buf;
		buf.printf(_T("Could not open for writing:  %s"), modelfilename);
		MaxMsgBox(0,buf,ShortDesc(),0);
		return 0;
	}

	pAnim = _tfopen(animfilename,_T("wb"));
	if (!pAnim) {
		TSTR buf;
		buf.printf(_T("Could not open for writing:  %s"), animfilename);
		MaxMsgBox(0,buf,ShortDesc(),0);
		return 0;
	}

	curNode = 0;
	ip->ProgressStart(_T("Exporting Using IGame.."), TRUE, fn, NULL);
	
	pIgame = GetIGameInterface();

	IGameConversionManager * cm = GetConversionManager();
	//	cm->SetUserCoordSystem(Whacky);
	cm->SetCoordSystem((IGameConversionManager::CoordSystem)cS);
	//	pIgame->SetPropertyFile(_T("hello world"));
	pIgame->InitialiseIGame(exportSelected);

	staticFrame = 0;
	pIgame->SetStaticFrame(staticFrame);
	




	for(int loop = 0; loop <pIgame->GetTopLevelNodeCount();loop++)
	{
		IGameNode * pGameNode = pIgame->GetTopLevelNode(loop);
		//check for selected state - we deal with targets in the light/camera section
		if(pGameNode->IsTarget())
			continue;

		
		ExportChildNodeInfo(pGameNode);

	}

	pIgame->ReleaseIGame();
	//PrettyPrint(name, pXMLDoc);


	MaxMsgBox(0,_T("Writing files"),ShortDesc(),0);

	if( Tris.Count() > 0 && Verts.Count() > 0 )
	{
		//rewind(pModel);
		DataHeader.NumPolys = Tris.Count();
		DataHeader.NumVertices = Verts.Count();
		fwrite(&DataHeader,sizeof(FJSDataHeader),1,pModel);

		fwrite(Tris.Addr(0),Tris.Count(),sizeof(FJSMeshTri),pModel);



		//rewind(pAnim);
		AnivHeader.FrameSize = Verts.Count() * sizeof(FMeshVert);
		AnivHeader.NumFrames = 1;
		fwrite(&AnivHeader,sizeof(FJSAnivHeader),1,pAnim);
		
		fwrite(Verts.Addr(0),Verts.Count(),sizeof(FMeshVert),pAnim);

	}

	// Close the stream
	fclose(pModel);
	fclose(pAnim);
	pModel = NULL;
	pAnim = NULL;


	
	ip->ProgressEnd();	

	WriteConfig();
	MaxMsgBox(0,_T("Export succesfull!"),ShortDesc(),0);
	return TRUE;

}


void U3DExport::ExportChildNodeInfo(IGameNode * child)
{
	TSTR buf,data;



	buf = TSTR("Processing: ") + TSTR(child->GetName());
	GetCOREInterface()->ProgressUpdate((int)((float)curNode/pIgame->GetTotalNodeCount()*100.0f),FALSE,buf.data()); 


	if(child->IsGroupOwner())
	{
	}
	else
	{
		ULONG  handle = child->GetMaxNode()->GetHandle();

		IGameObject * obj = child->GetIGameObject();

		IGameObject::MaxType T = obj->GetMaxType();


		switch(obj->GetIGameType())
		{
			

			case IGameObject::IGAME_BONE:
			{
				break;
			}

			case IGameObject::IGAME_HELPER:
			{
		
				break;
			}
			case IGameObject::IGAME_LIGHT:
			{
				break;
			}
			case IGameObject::IGAME_CAMERA:
			{
				break;
			}

			case IGameObject::IGAME_MESH:
			{
				IGameMesh * gm = (IGameMesh*)obj;
				TSTR buf;
				buf.printf(_T("Exporting:  %s"), child->GetName());
				MaxMsgBox(0,buf,ShortDesc(),0);
				//gm->SetCreateOptimizedNormalList();
				if(gm->InitializeData())
				{
					int vertcount = gm->GetNumberOfVerts();
					int tricount = gm->GetNumberOfFaces();
					if( vertcount == 0 || tricount == 0 )
						break;

					FMeshVert vert = FMeshVert();
					FJSMeshTri tri = FJSMeshTri();

					int lastvert = Verts.Append(vertcount,&vert,0);
					int lasttri = Tris.Append(tricount,&tri,0);


					for( int i=0; i!=tricount; ++i )
					{
						FaceEx* f = gm->GetFace(i);
						if( f )
						{
							FJSMeshTri t = FJSMeshTri();
							t.iVertex[0] = f->vert[0];
							t.iVertex[1] = f->vert[1];
							t.iVertex[2] = f->vert[2];
							Point2 p;
							if( gm->GetTexVertex(f->texCoord[0],p) ) t.Tex[0] = FMeshUV(p);
							if( gm->GetTexVertex(f->texCoord[1],p) ) t.Tex[1] = FMeshUV(p);
							if( gm->GetTexVertex(f->texCoord[2],p) ) t.Tex[2] = FMeshUV(p);
							t.TextureNum = f->matID;
							
							Tris[lasttri+i] = t;
						}
					}
					

					for( i=0; i!=vertcount; ++i )
					{
						Verts[lastvert+i] = FMeshVert( FVector(gm->GetVertex(i)) );
					}

					//

				}
			}
			break;

			case IGameObject::IGAME_SPLINE:
			{

			}
				break;

			case IGameObject::IGAME_IKCHAIN:
			{

			}
				break;
				
		}
	}	

	for(int i=0;i<child->GetChildCount();i++)
	{
		IGameNode * newchild = child->GetNodeChild(i);

		// we deal with targets in the light/camera section
		if(newchild->IsTarget())
			continue;

		ExportChildNodeInfo(newchild);
	}

	child->ReleaseIGameObject();


}


TSTR U3DExport::GetCfgFilename()
{
	TSTR filename;
	
	filename += GetCOREInterface()->GetDir(APP_PLUGCFG_DIR);
	filename += "\\";
	filename += "U3DExport.cfg";
	return filename;
}

// NOTE: Update anytime the CFG file changes
#define CFG_VERSION 0x01

BOOL U3DExport::ReadConfig()
{
	TSTR filename = GetCfgFilename();
	FILE* cfgStream;

	cfgStream = fopen(filename, "rb");
	if (!cfgStream)
		return FALSE;
	
	/*exportGeom = fgetc(cfgStream);
	exportNormals = fgetc(cfgStream);
	exportControllers = fgetc(cfgStream);
	exportFaceSmgp = fgetc(cfgStream);
	exportVertexColor = fgetc(cfgStream);
	exportTexCoords = fgetc(cfgStream);
	staticFrame = _getw(cfgStream);
	framePerSample = _getw(cfgStream);
	exportMappingChannel = fgetc(cfgStream);
	exportMaterials = fgetc(cfgStream);
	exportSplines = fgetc(cfgStream);
	exportModifiers = fgetc(cfgStream);
	forceSample = fgetc(cfgStream);
	exportConstraints = fgetc(cfgStream);
	exportSkin = fgetc(cfgStream);
	exportGenMod = fgetc(cfgStream);
	cS = fgetc(cfgStream);
	splitFile = fgetc(cfgStream);
	exportQuaternions = fgetc(cfgStream);
	exportObjectSpace = fgetc(cfgStream);
	exportRelative = fgetc(cfgStream);
	exportNormalsPerFace = fgetc(cfgStream);*/
	fclose(cfgStream);
	return TRUE;
}

void U3DExport::WriteConfig()
{
	TSTR filename = GetCfgFilename();
	FILE* cfgStream;

	cfgStream = fopen(filename, "wb");
	if (!cfgStream)
		return;

	
	/*fputc(exportGeom,cfgStream);
	fputc(exportNormals,cfgStream);
	fputc(exportControllers,cfgStream);
	fputc(exportFaceSmgp,cfgStream);
	fputc(exportVertexColor,cfgStream);
	fputc(exportTexCoords,cfgStream);
	_putw(staticFrame,cfgStream);
	_putw(framePerSample,cfgStream);
	fputc(exportMappingChannel,cfgStream);
	fputc(exportMaterials,cfgStream);
	fputc(exportSplines,cfgStream);
	fputc(exportModifiers,cfgStream);
	fputc(forceSample,cfgStream);
	fputc(exportConstraints,cfgStream);
	fputc(exportSkin,cfgStream);
	fputc(exportGenMod,cfgStream);
	fputc(cS,cfgStream);
	fputc(splitFile,cfgStream);
	fputc(exportQuaternions,cfgStream);
	fputc(exportObjectSpace,cfgStream);
	fputc(exportRelative,cfgStream);
	fputc(exportNormalsPerFace, cfgStream);*/

	fclose(cfgStream);
}