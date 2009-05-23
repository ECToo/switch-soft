// ============================================================================
//  PatchINIStatic.h
// ============================================================================
#pragma once	


// flags
#define PATCHINI_NOFLAGS 0x0000
#define PATCHINI_DEFAULTONLY 0x0001


#ifdef __cplusplus
extern "C" {
#endif

extern int PatchINIMerge( const char* mergeini
						 , const char* baseini
						 , const char* resultini
						 , int flags
						 , char* errorbuffer
						 , int errorbufferlen );

extern int PatchINIMergeW( const wchar_t* mergeini
						 , const wchar_t* baseini
						 , const wchar_t* resultini
						 , int flags
						 , wchar_t* errorbuffer
						 , int errorbufferlen );

#ifdef __cplusplus
}
#endif


// ============================================================================
//  EOF
// ============================================================================