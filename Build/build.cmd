@echo off

call config.cmd

if "%AMD64%" == "true"( set ARCH=x64 
set OUT=%OUT_FOLDER_64%
) else ( set ARCH=""	
set OUT=%OUT_FOLDER_32%
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
 
set PYTHON="%python%"
set SCRIPT=""-u %~dp0build.py --goal tools_build --out %OUT%""

python %SCRIPT% %*
::

::python -u %~dp0build.py --goal tools_build --out '%OUT%'
ECHO ERRORLEVEL=%ERRORLEVEL%