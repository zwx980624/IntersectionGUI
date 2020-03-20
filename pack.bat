@echo off
echo Setting up environment for Qt usage...
set PATH=C:\Qt\Qt5.9.0\5.9\msvc2017_64\bin;%PATH%
echo Remember to call vcvarsall.bat to complete environment setup!
rmdir /s/q pack
mkdir pack
copy x64\Release\IntersectionGUI.exe pack\
copy IntersectionGUI\calcInterface.dll pack\
copy IntersectionGUI\calcInterface.lib pack\
cd pack
windeployqt IntersectionGUI.exe