#include <windows.h>
#include "NsPatchINI.h"
#include "PatchINI/PatchINIStatic.h"

HINSTANCE g_hInstance;

HWND g_hwndParent;

void __declspec(dllexport) Merge(HWND hwndParent, int string_size, 
                                      char *variables, stack_t **stacktop,
                                      extra_parameters *extra)
{
	g_hwndParent=hwndParent;

	NSPATCHINI_INIT();

	{
		char mergeini[1024];
		char baseini[1024];
		char resultini[1024];
		int result = 1;
		char errorstr[1024];

		if(	popstring(mergeini) == 0
		&&	popstring(baseini) == 0
		&&	popstring(resultini) == 0 )
		{
			result = PatchINIMerge(mergeini,baseini,resultini,errorstr,sizeof(errorstr));
			pushstring(errorstr);
		}
		else
			pushstring("PatchINIMerge:: insufficient params");
	}
}



BOOL WINAPI DllMain(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
	g_hInstance=hInst;
	return TRUE;
}
