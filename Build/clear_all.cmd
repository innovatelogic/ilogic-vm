@echo off
call %~dp0\config.cmd

if exist %~dp0..\..\%OUT_FOLDER_32%\ ( rmdir /Q /S %~dp0..\..\%OUT_FOLDER_32% && echo clear32 ok) ELSE (echo clear32 not exist)
if exist %~dp0..\..\%OUT_FOLDER_64%\ ( rmdir /Q /S %~dp0..\..\%OUT_FOLDER_64% && echo clear64 ok) ELSE (echo clear64 not exist)