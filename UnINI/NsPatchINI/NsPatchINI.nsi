# =============================================================================
#  NsPatchINI script
#  (C) 2007-2008 Roman Switch" Dzieciol
# =============================================================================


!include "Stack.nsh"


# -----------------------------------------------------------------------------
#  compiler
# -----------------------------------------------------------------------------

OutFile "NsPatchINI.exe"
SetCompress off
ShowInstDetails show

Var STACK_FILE_LIST

# -----------------------------------------------------------------------------
#  variables
# -----------------------------------------------------------------------------

Function Test

    
    ${stack::dll_create} $STACK_FILE_LIST
    
    
    
    ${stack::dll_insert} "$STACK_FILE_LIST" "11" "-1" $0
    ${stack::dll_insert} "$STACK_FILE_LIST" "22" "-1" $0
    ${stack::dll_insert} "$STACK_FILE_LIST" "33" "-1" $0
    ${stack::dll_insert} "$STACK_FILE_LIST" "44" "-1" $0
    
    StrCpy $R0 "1"
    ${stack::dll_size} "$STACK_FILE_LIST" $R1
    DetailPrint $R1
    
Loop:    

    ${stack::dll_read} "$STACK_FILE_LIST" $R0 $1 $0
    DetailPrint $1
    
    IntOp $R0 $R0 + 1
    IntCmp $R0 $R1 Loop Loop 0
    
    
    
    ${stack::dll_destroy} "$STACK_FILE_LIST"
    
FunctionEnd

# -----------------------------------------------------------------------------
#  main section
# -----------------------------------------------------------------------------
Section

    # -------------------------------------------------------------------------
    #  write ENV setup
    # -------------------------------------------------------------------------
    
    ClearErrors
    
    Call Test
    
    File "NSPatchINI2.exe"
    
    DetailPrint $EXEDIR
    
    StrCpy $0 "$EXEDIR\..\"
    StrCpy $1 "$0G-Default.ini"
    StrCpy $2 "$0UT2004.ini"
    StrCpy $3 "$0Gunreal2K4.ini"
    NsPatchINI::Merge $1 $2 $3 
    Pop $R0
    StrCmp $R0 "" +3
    DetailPrint $R0
    Goto ReturnError
    
    StrCpy $1 "$0G-DefUser.ini"
    StrCpy $2 "$0User.ini"
    StrCpy $3 "$0GunrealUser2K4.ini"
    NsPatchINI::Merge $1 $2 $3 
    IfErrors ReturnError
    Pop $R0
    StrCmp $R0 "" +3
    DetailPrint $R0
    Goto ReturnError
    

  ;Create uninstaller
  WriteUninstaller "$EXEDIR\Uninstall.exe"
  
    # success
    Goto ReturnSuccess


    # -------------------------------------------------------------------------
    #  Exit points
    # -------------------------------------------------------------------------

ReturnError:
    Call Cleanup
    Abort "Cannot continue"

ReturnSuccess:
    Call Cleanup

SectionEnd


# -----------------------------------------------------------------------------
#  Cleanup
# -----------------------------------------------------------------------------
Function Cleanup

    # close file handles


FunctionEnd

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

DetailPrint $INSTDIR
  Delete "$INSTDIR\Uninstall.exe"


SectionEnd

# =============================================================================
#  EOF
# =============================================================================