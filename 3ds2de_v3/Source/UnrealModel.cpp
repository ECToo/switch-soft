// UnrealModel.cpp
// $Author:   Dave Townsend  $
// $Date:   1 Jan 1997 12:00:00  $
// $Revision:   1.0  $

// May 25, 2001: Modified for the Deus Ex format -- Steve Tack

#include "3ds2de.h"

//===========================================================================
cUnrealPolygon::cUnrealPolygon( int V0, int V1, int V2 ) 
{
    ::memset( this, '\0', sizeof *this );
    mVertex[0] = V0;
    mVertex[1] = V1;
    mVertex[2] = V2;
}

//===========================================================================
cUnrealPolygon::cUnrealPolygon( int V0, int V1, int V2, int Type,
                                unsigned char V0u, unsigned char V0v,
                                unsigned char V1u, unsigned char V1v,
                                unsigned char V2u, unsigned char V2v,
                                int TextureNum ) 
: mType( Type ),
  mColor( 0 ),
  mTextureNum( TextureNum ),
  mFlags( 0 )
{
    mVertex[0] = V0;
    mVertex[1] = V1;
    mVertex[2] = V2;

    mTex[0][0] = V0u;
    mTex[0][1] = V0v;
    mTex[1][0] = V1u;
    mTex[1][1] = V1v;
    mTex[2][0] = V2u;
    mTex[2][1] = V2v;
}

//===========================================================================
cUnrealModel::cUnrealModel() 
{
    for( int i = 0; i < 10; ++i )
        mTextures.push_back( "" );
}

//===========================================================================
cUnrealModel::~cUnrealModel() 
{
}

//===========================================================================
void cUnrealModel::AddPolygon( const cUnrealPolygon& NewPoly )
{
    mPolygons.push_back( NewPoly );
}

//===========================================================================
void cUnrealModel::AddTexture( int TextureNum, const string& TextureName ) 
{
    if( TextureNum < 0 || 10 <= TextureNum )
        throw exception( "UnrealModel::AddTexture: bad TextureNum" );

    mTextures[ TextureNum ] = TextureName;
}

//===========================================================================
void cUnrealModel::AddVertex( float X, float Y, float Z ) 
{

	// Modified to use Deus Ex formatted vertices.  -- Steve Tack

	DeusExVertex	NewVertex;
	NewVertex.x = (int(X * 256.0)) & 0xffff;
	NewVertex.y = (int(Y * 256.0)) & 0xffff;
	NewVertex.z = (int(Z * 256.0)) & 0xffff;
	NewVertex.dummy = 0;

    mVertices.push_back( NewVertex );

}

//===========================================================================
int cUnrealModel::GetNumPolygons() const
{
    return mPolygons.size();
}

//===========================================================================
void cUnrealModel::NewSequence( const string& Name, int Len ) 
{
    Seq   NewSeq;
    NewSeq.Name = Name;
    NewSeq.Length = Len;
    mSequences.push_back( NewSeq );
}

//===========================================================================
void cUnrealModel::Write( const string& ProjDir, const string& BaseName ) 
{
    // Pre-compute some useful values
    int MaxSeqNameLen = 0;
    int NumFrames = 0;
    for( int Seq = 0; Seq < mSequences.size(); ++Seq ) {
        if( MaxSeqNameLen < mSequences[ Seq ].Name.length() )
            MaxSeqNameLen = mSequences[ Seq ].Name.length();
        NumFrames += mSequences[ Seq ].Length;
    }

    // ---- Write _d.3d file ------------------------------------------------
    string DFileName = ProjDir;
    DFileName += "\\Models\\";
    DFileName += BaseName;
    DFileName += "_d.3d";
    FILE* fp = fopen( DFileName.c_str(), "wb" );
    if( fp == 0 )
        throw exception( "can't open _d.3d file" );

    struct DHeader {
        unsigned short  NumPolygons;
        unsigned short  NumVertices;
        unsigned short  BogusRot;
        unsigned short  BogusFrame;
        unsigned long   BogusNormX;
        unsigned long   BogusNormY;
        unsigned long   BogusNormZ;
        unsigned long   FixScale;
        unsigned long   Unused[3];
        unsigned char   Unknown[12];
    } dh;

    memset( &dh, '\0', sizeof(dh) );
    dh.NumPolygons = mPolygons.size();
    dh.NumVertices = mVertices.size() / NumFrames;

    if( fwrite( &dh, sizeof(dh), 1, fp ) != 1 )
        throw exception( "_d.3d: couldn't write header" );

    cPolygonList::iterator i = mPolygons.begin();
    for(  ; i != mPolygons.end(); ++i )
        if( fwrite( i, sizeof(*i), 1, fp ) != 1 )
            throw exception( "_d.3d: couldn't write polygon" );

    fclose( fp );

    // ---- Write _a.3d file ------------------------------------------------
    string AFileName = ProjDir;
    AFileName += "\\Models\\";
    AFileName += BaseName;
    AFileName += "_a.3d";
    fp = fopen( AFileName.c_str(), "wb" );
    if( fp == 0 )
        throw exception( "can't open _a.3d file" );

    short   Data;
    Data = NumFrames;
    if( fwrite( &Data, sizeof(Data), 1, fp ) != 1 )
        throw exception( "can't write _a.3d #frames" );

    Data = mVertices.size() * sizeof(DeusExVertex) / NumFrames;
    if( fwrite( &Data, sizeof(Data), 1, fp ) != 1 )
        throw exception( "can't write _a.3d framesize" );

    cVertexList::iterator v = mVertices.begin();
    for(  ; v != mVertices.end(); ++v ) 
        if( fwrite( v, sizeof(*v), 1, fp ) != 1 )
            throw exception( "_a.3d: couldn't write vertex" );

    fclose( fp );

    //---- Write .uc file ----------------------------------------------------
    string UCFileName = ProjDir;
    UCFileName += "\\Classes\\";
    UCFileName += BaseName;
    UCFileName += ".uc";
    fp = fopen( UCFileName.c_str(), "w" );
    if( fp == 0 )
        throw exception( "can't open .uc file" );
    
    string CommentLine = "//";
    CommentLine += string(77, '=' );
    CommentLine += "\n";

    fputs( CommentLine.c_str(), fp );
    fprintf( fp, "// %s.\n", BaseName.c_str() );
    fputs( CommentLine.c_str(), fp );
    
    fprintf( fp, "class %s extends Actor;\n\n", BaseName.c_str() );
    
    fprintf( fp, "#exec MESH IMPORT MESH=%s "
                 "ANIVFILE=MODELS\\%s_a.3d "
                 "DATAFILE=MODELS\\%s_d.3d X=0 Y=0 Z=0\n",
                 BaseName.c_str(), BaseName.c_str(), BaseName.c_str() );
    fprintf( fp, "#exec MESH ORIGIN MESH=%s X=0 Y=0 Z=0\n\n",
                 BaseName.c_str() );

    string ExecMeshSeq = "#exec MESH SEQUENCE MESH=";
    ExecMeshSeq += BaseName.c_str();
    ExecMeshSeq += " SEQ=";

    fprintf( fp, "%s%*s STARTFRAME=0 NUMFRAMES=%d\n",
                 ExecMeshSeq.c_str(), -MaxSeqNameLen, "All", NumFrames );

    int StartFrame = 0;
    for( /*int*/ Seq = 0; Seq < mSequences.size(); ++Seq ) {
        fprintf( fp, "%s%*s STARTFRAME=%d NUMFRAMES=%d\n",
                 ExecMeshSeq.c_str(),
                 -MaxSeqNameLen, mSequences[ Seq ].Name.c_str(),
                 StartFrame, mSequences[ Seq ].Length );
        StartFrame += mSequences[ Seq ].Length;
    }

    fprintf( fp, "\n" );

    bool AnyTextures = false;
    for( int TexNum = 0; TexNum < mTextures.size(); ++TexNum ) {
        if( !mTextures[ TexNum ].empty() ) {
            fprintf( fp, "#exec TEXTURE IMPORT NAME=J%s%d "
                         "FILE=MODELS\\%s%d.PCX GROUP=Skins ",
                         BaseName.c_str(), TexNum, BaseName.c_str(), TexNum );
            fprintf( fp, "// %s\n", mTextures[ TexNum ].c_str() );
            AnyTextures = true;
        }
    }
    if( AnyTextures )
        fprintf( fp, "\n" );

    fprintf( fp, "#exec MESHMAP NEW   MESHMAP=%s MESH=%s\n",
                 BaseName.c_str(), BaseName.c_str() );
    fprintf( fp, "#exec MESHMAP SCALE MESHMAP=%s X=0.00390625 Y=0.00390625 Z=0.00390625\n\n",
                 BaseName.c_str());

    if( AnyTextures ) {
        for( int i = 0; i < mTextures.size(); ++i ) {
            if( !mTextures[ i ].empty() ) {
                fprintf( fp, "#exec MESHMAP SETTEXTURE MESHMAP=%s "
                             "NUM=%d TEXTURE=J%s%d\n",
                              BaseName.c_str(), i, BaseName.c_str(), i );
            }
        }
    }

    fprintf( fp, "\ndefaultproperties\n{\n" );
    fprintf( fp, "    DrawType=DT_Mesh\n" );
    fprintf( fp, "    Mesh=%s\n", BaseName.c_str() );
    fprintf( fp, "}\n" );

    fclose( fp );
}


