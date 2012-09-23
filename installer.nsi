; The name of the installer
Name "Uistelupäiväkirja"

; The file to write
OutFile "uistelu-win32.exe"

; The default installation directory
InstallDir $PROGRAMFILES\Uistelupaivakirja

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\Uistelu" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "Uistelupäiväkirja (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "release\Uistelupaivakirja.exe"
  File "$%lib%\libgcc_s_dw2-1.dll"
  File "$%lib%\libstdc++-6.dll"
  File "$%lib%\mingwm10.dll"
  File "$%lib%\phonon4.dll"
  File "$%lib%\QtCore4.dll"
  File "$%lib%\QtGui4.dll"
  File "$%lib%\QtNetwork4.dll"
  File "$%lib%\QtOpenGl4.dll"
  File "$%lib%\QtWebKit4.dll"
  File "$%lib%\QtXml4.dll"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\Uistelu "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Uistelu" "DisplayName" "Uistelupaivakirja"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Uistelu" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Uistelu" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Uistelu" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Uistelupaivakirja"
  CreateShortCut "$SMPROGRAMS\Uistelupaivakirja\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Uistelupaivakirja\Uistelupaivakirja.lnk" "$INSTDIR\Uistelupaivakirja.exe" "" "$INSTDIR\Uistelupaivakirja.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Uistelu"
  DeleteRegKey HKLM SOFTWARE\Uistelu
  DeleteRegKey HKCU "Software\Samuli Penttila\Uistelupaivakirja"

  ; Remove files and uninstaller
  Delete $INSTDIR\Uistelupaivakirja.nsi
  Delete $INSTDIR\uninstall.exe
  Delete $INSTDIR\Uistelupaivakirja.exe
  Delete $INSTDIR\libgcc_s_dw2-1.dll
  Delete $INSTDIR\libstdc++-6.dll
  Delete $INSTDIR\mingwm10.dll
  Delete $INSTDIR\phonon4.dll
  Delete $INSTDIR\QtCore4.dll
  Delete $INSTDIR\QtGui4.dll
  Delete $INSTDIR\QtNetwork4.dll
  Delete $INSTDIR\QtOpenGl4.dll
  Delete $INSTDIR\QtWebKit4.dll
  Delete $INSTDIR\QtXml4.dll

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Uistelupaivakirja\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\Uistelupaivakirja"
  RMDir "$INSTDIR"

SectionEnd
