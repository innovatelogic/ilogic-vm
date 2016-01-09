@echo off

call %~dp0/config.cmd
 
set PYTHON="%python%"
set SCRIPT=""-u %~dp0build.py -h""

python %SCRIPT% %*

ECHO ERRORLEVEL=%ERRORLEVEL%