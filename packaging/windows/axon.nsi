; Axon NSIS Installer Script
; Basic installation script for Axon

!include "MUI2.nsh"
!include "EnvVarUpdate.nsh"

Name "Axon Encryption Tool"
OutFile "AxonInstaller.exe"
InstallDir "$PROGRAMFILES\Axon"
InstallDirRegKey HKLM "Software\Axon" "Install_Dir"
RequestExecutionLevel admin

; Modern UI
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\..\LICENSE"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Languages
!insertmacro MUI_LANGUAGE "English"

Section "Install"
  SetOutPath "$INSTDIR"
  
  ; Copy the executable and dependencies
  File "..\..\build\axon.exe"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM "Software\Axon" "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Axon" "DisplayName" "Axon Encryption Tool"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Axon" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Axon" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Axon" "NoRepair" 1
  
  ; Add to PATH
  ${EnvVarUpdate} $0 "PATH" "A" "HKLM" "$INSTDIR"
  
  ; Create Start Menu shortcut
  CreateDirectory "$SMPROGRAMS\Axon"
  CreateShortcut "$SMPROGRAMS\Axon\Axon.lnk" "$INSTDIR\axon.exe"
  CreateShortcut "$SMPROGRAMS\Axon\Uninstall.lnk" "$INSTDIR\uninstall.exe"
  
  ; Create uninstaller
  WriteUninstaller "$INSTDIR\uninstall.exe"
SectionEnd

Section "Uninstall"
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Axon"
  DeleteRegKey HKLM "Software\Axon"

  ; Remove from PATH
  ${un.EnvVarUpdate} $0 "PATH" "R" "HKLM" "$INSTDIR"
  
  ; Remove files and uninstaller
  Delete "$INSTDIR\axon.exe"
  Delete "$INSTDIR\uninstall.exe"
  
  ; Remove shortcuts
  Delete "$SMPROGRAMS\Axon\Axon.lnk"
  Delete "$SMPROGRAMS\Axon\Uninstall.lnk"
  RMDir "$SMPROGRAMS\Axon"
  
  ; Remove directories
  RMDir "$INSTDIR"
SectionEnd