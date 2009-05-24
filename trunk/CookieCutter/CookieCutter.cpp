//	:: UnrealScript TextBuffer Exporter
//	:: Copyright(C) 2004 Switch` switch@thieveryut.com

#include "CookieCutter.h"

int main(int argc, char* argv[])
{
	if( argc != 2 )
	{
		cout << endl << "INFO    :: UnrealScript TextBuffer Exporter";
		cout << endl << "        :: Copyright(C) 2004 Switch` switch@thieveryut.com";
		cout << endl;
		cout << endl << "USAGE   :: CookieCutter.exe UnrealPackage";
		cout << endl << "EXAMPLE :: CookieCutter.exe Core.u";
		cout << endl;
		return 0;
	}

	ProcessFile(argv[1]);
	return 0;
}

void ProcessFile( string FName )
{
	fstream File(FName.c_str(),  ios::in | ios::binary);

	if( File.good() == false )
	{
		cout << endl << "[ERROR] Unable to open file "<< FName;
		cout << endl;
		return;
	}


	// - make directory -------------------------------------------------------

	string mdcmd = "mkdir ";
	string pkgdir = ".\\";
	pkgdir.append(FName, 0, FName.find('.'));
	pkgdir.append("\\Classes\\");
	mdcmd.append(pkgdir);

	system(mdcmd.c_str());


	// - header ---------------------------------------------------------------
	PKG_Header PKGHeader;
	File.read((char*)(&PKGHeader), sizeof(PKGHeader));

	// info
//	cout << endl << "Package Version " << dec << LOWORD(PKGHeader.PackageVersion);
//	cout << endl << "Licencee Mode   " << dec << HIWORD(PKGHeader.PackageVersion);


	// - get scripttext index -------------------------------------------------
	
	DWORD STIndex = -1;
	NameItem* NameList = ReadNameTable( File, PKGHeader.NameOffset, PKGHeader.NameCount, STIndex );
	if( STIndex == -1 )
	{
		cout << endl << "[ERROR] ScriptText not found in the NameTable";
		cout << endl;
		return;
	}
	

	// - find scripttexts in export table -------------------------------------

	ExportItem* ExportList = ReadExportTable( File, PKGHeader.ExportOffset, PKGHeader.ExportCount );


	DWORD i;
	for(i=0; i<PKGHeader.ExportCount; i++)
	{
		if( ExportList[i].ObjectName == STIndex )
		{
			//cout << endl << "[" << i << "] " << dec << NameList[ExportList[ExportList[i].Package-1].ObjectName].Name;
			char *buffer = ReadTextBuffer( File, ExportList[i].SerialOffset );
			string fname = pkgdir + "\\" + NameList[ExportList[ExportList[i].Package-1].ObjectName].Name + ".uc";
			fstream SrcFile(fname.c_str(), ios_base::out | ios_base::trunc | ios_base::binary);
			SrcFile << buffer;
			SrcFile.close();

			//cout << endl << buffer;
			delete[] buffer;
		}
	}

	
    File.close();
	return;
}

NameItem* ReadNameTable( fstream &File, DWORD &NameOffset, DWORD &NameCount, DWORD &TextBufferIndex )
{
	File.seekg( NameOffset, ios_base::beg );

	BYTE NameLength;
	DWORD NameFlags;
	DWORD i;

	NameItem *NameList;
	NameList = (NameItem *) malloc(NameCount * sizeof(NameItem));
	
	for(i=0; i<NameCount; i++)
	{
		NameLength = File.get();
	
		char *buffer = new char [NameLength];
		File.read( buffer, NameLength);
		File.read((char*)(&(NameFlags)), sizeof(NameFlags));

		NameList[i].Name = buffer;
		NameList[i].Flags = NameFlags;

		if( strcmp(buffer, "ScriptText") == 0 )
			TextBufferIndex = i;

		
	//	cout <<endl<< NameList[i].Name;
	}
	return NameList;
}

ExportItem* ReadExportTable( fstream &File, DWORD &ExportOffset, DWORD &ExportCount )
{
	File.seekg( ExportOffset, ios_base::beg );

	DWORD i;

	ExportItem *ExportList;
	ExportList = (ExportItem *) malloc(ExportCount * sizeof(ExportItem));

	for(i=0; i<ExportCount; i++)
	{
		ExportList[i].Class = ReadIndex( File );
		ExportList[i].Super = ReadIndex( File );
		File.read((char*)(&ExportList[i].Package), sizeof(ExportList[i].Package));
		ExportList[i].ObjectName = ReadIndex( File );
		File.read((char*)(&ExportList[i].ObjectFlags), sizeof(ExportList[i].ObjectFlags));
		ExportList[i].SerialSize = ReadIndex( File );
		if( ExportList[i].SerialSize > 0 )
			ExportList[i].SerialOffset = ReadIndex( File );
		
	//	cout <<endl<< ExportList[i].ObjectName;
	}
	return ExportList;
}

string itos(int num)
{
	ostringstream myStream;
	myStream << num << flush;
	return(myStream.str());
}

int ReadIndex( fstream &File )
{
	BYTE b0, b1, b2, b3, b4;
	int result = 0;

	b0 = File.get();
	if (b0 & 0x40)
	{
		b1 = File.get();
		if (b1 & 0x80)
		{
			b2 = File.get();
			if (b2 & 0x80)
			{
				b3 = File.get();
				if (b3 & 0x80)
				{
					b4 = File.get();
					result = b4;
				}
				result = (result << 7) + (b3 & 0x7F);
			}
			result = (result << 7) + (b2 & 0x7F);
		}
		result = (result << 7) + (b1 & 0x7F);
	}
	result = (result << 6) + (b0 & 0x3F);

	if (b0 & 0x80) 
		result = -result;

	return result;
}

char* ReadTextBuffer( fstream &File, DWORD Offset )
{
	DWORD Pos;
	DWORD Top;
	WORD wtf;
	DWORD TextSize;
	DWORD OldOffset;
	
	OldOffset = File.tellg();
	File.seekg( Offset, ios_base::beg );

	File.read((char*)(&Pos), sizeof(Pos));
	File.read((char*)(&Top), sizeof(Top));
	File.read((char*)(&wtf), sizeof(wtf));
	TextSize = ReadIndex( File );

	//cout << endl << dec << "Pos: " << Pos << ", Top: " << Top << ", wtf: " << wtf << ", TextSize: " << TextSize;

	if( TextSize > 0 )
	{
		char *buffer = new char [TextSize];
		File.read( buffer, TextSize);
		File.seekg( OldOffset, ios_base::beg );
		return buffer;
	}
	return "empty textbuffer?";
}

