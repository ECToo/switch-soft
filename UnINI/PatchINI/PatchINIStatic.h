// ============================================================================
//  PatchINIStatic.h
// ============================================================================
#pragma once		

#ifdef __cplusplus
extern "C" {
#endif

extern int PatchINIMerge( const char* mergeini
						 , const char* baseini
						 , const char* resultini
						 , char* errorbuffer
						 , int errorbufferlen );

#ifdef __cplusplus
}
#endif


// ============================================================================
//  EOF
// ============================================================================