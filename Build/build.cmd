@echo off

set AMD64=false
set GENERATOR="Visual Studio 11 2012"

if "%AMD64%" == "true"( set ARCH=x64 
set OUT=out64
) else ( set ARCH=""	
set OUT=out32
)

set ROOT_DIR=%~dp0..
set INSTALL_DIR=%ROOT_DIR%\..\%OUT%
set WORKING_DIR=%INSTALL_DIR%\build_%TARGET%_vc110
IF NOT EXIST %INSTALL_DIR% mkdir %INSTALL_DIR%

IF "%MSVC_REDIST_LIBS%" == ""( setLocal EnableDelayedExpansion
  set MSVC_REDIST_LIBS=%VS110COMNTOOLS%..\..\VC\redist\x64\Microsoft.vC11.CRT
  if !VS110COMNTOOLS!=="" (echo "Warning: failed to get redistributable lib path")
  setLocal DisableDelayedExpansion
 )

python -u %~dp0build.py --goal tools_build
ECHO ERRORLEVEL=%ERRORLEVEL%