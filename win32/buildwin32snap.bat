@echo off
IF EXIST win32\GamesServ-Setup*.exe del win32\GamesServ-Setup*.exe
"C:\Program Files\Pantaray\QSetup\Composer.exe" win32\GamesServ.qsp /Compile /Exit
move win32\GamesServ-Setup.exe win32\GamesServ-Setup-3-0-0-%1.exe
