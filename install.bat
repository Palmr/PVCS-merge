@ECHO OFF

:build
IF EXIST ".\pvcsmerge.exe" GOTO :installEXE
CALL build.bat
IF NOT EXIST ".\pvcsmerge.exe" GOTO :buildFail
GOTO :installEXE

:installEXE
IF NOT EXIST "%PROGRAMFILES%\PVCS\vm\win32\bin\pvcsmerge.exe" GOTO :installFail
ECHO Installing PVCS-merge...
ECHO Backing up old pvcsmerge.exe to old-pvcsmerge.exe
MOVE "%PROGRAMFILES%\PVCS\vm\win32\bin\pvcsmerge.exe" "%PROGRAMFILES%\PVCS\vm\win32\bin\old-pvcsmerge.exe"

ECHO Copying new pvcsmerge.exe
COPY ".\pvcsmerge.exe" "%PROGRAMFILES%\PVCS\vm\win32\bin\pvcsmerge.exe"


:generateConfig
IF EXIST ".\pvcsmerge.cfg" GOTO :installCONFIG
IF EXIST "%PROGRAMFILES%\WinMerge\WinMergeU.exe" GOTO :winmergePF
IF EXIST "%PROGRAMFILES(x86)%\WinMerge\WinMergeU.exe" GOTO :winmergePFx86
GOTO :customDiffTool


:winmergePF
ECHO C:\PROGRA~1\WinMerge\WinMergeU.exe /e /u /maximize /wl /wr /dl "(leftDesc)" /dr "(rightDesc)" "(leftPath)" "(rightPath)" >> ".\pvcsmerge.cfg"
GOTO :installCONFIG


:winmergePFx86
ECHO C:\PROGRA~2\WinMerge\WinMergeU.exe /e /u /maximize /wl /wr /dl "(leftDesc)" /dr "(rightDesc)" "(leftPath)" "(rightPath)" >> ".\pvcsmerge.cfg"
GOTO :installCONFIG


:customDiffTool
ECHO C:\path\to\diff\tool.exe /e /u /maximize /wl /wr /dl "(leftDesc)" /dr "(rightDesc)" "(leftPath)" "(rightPath)" >> ".\pvcsmerge.cfg"
START /WAIT notepad.exe ".\pvcsmerge.cfg"
GOTO :installCONFIG


:installCONFIG
ECHO Installing config...
COPY ".\pvcsmerge.cfg" "%PROGRAMFILES%\PVCS\vm\win32\bin\pvcsmerge.cfg"
GOTO :end


:buildFail
ECHO Build didn't seem to work :(
GOTO :end


:installFail
ECHO No PVCS found to install into :(
GOTO :end


:end
ECHO Done!
PAUSE
