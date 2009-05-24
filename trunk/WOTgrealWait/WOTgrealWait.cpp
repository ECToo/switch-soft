// WOTgrealWait.cpp : Defines the entry point for the application.
//


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <tlhelp32.h>

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	// TODO: Place code here.

	LPSTR str = lpCmdLine;

	// We need to know how long is the name of the 
	// executable because in the running process list
	// only 15 characters are reported, every character 
	// over the 15th is truncated:
	// i.e. 'MyApplication.exe'  is reported as 'MyApplication.e' and so on.

/*	int len = strlen(str);

	// Let's make some logic and if the length of the name is over 15, 
	// let's put NULLs on the extra characters.
	// This is needed because once we obtain the running process list, 
	// we will iterate on it and find the process to kill on a 
	// strcmp name basis.

	if(len > 15)
	{
		for(int i = 15; i < len; i++)
			if(*(str + i)!=NULL)
				*(str + i) = NULL;
	}*/

	// Wait for process start
	DWORD processID = 0;
	while(true)
	{
		// Let's take a snapshot of the running process list
		HANDLE hSnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

		// Prepare the struct to contains every process information

		PROCESSENTRY32* processInfo=new PROCESSENTRY32;
		processInfo->dwSize=sizeof(PROCESSENTRY32);

		BOOL bFound = false;


		// Iterate through processes to find ours...

		while(Process32Next(hSnapShot,processInfo)!=FALSE)
		{
			// store the Process ID
			processID = processInfo->th32ProcessID;

			// Is this our running process to kill?
			if(strcmp(processInfo->szExeFile,str) == 0)
			{
				//MessageBox(0,"found","WOTgrealWait",0);
				bFound = TRUE;
				break;
			}
		}

		CloseHandle(hSnapShot);

		if( bFound )
			break;
		else
			Sleep(200);
	}

	// Obtain the Process handle....
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,TRUE,processID);
	if(hProcess != NULL)
	{
		//wait for end
		//MessageBox(0,"wait","WOTgrealWait",0);
		WaitForSingleObject( hProcess, INFINITE );
		return 0;
	}

	return 1;
}



