@echo off
rem Откройте cmd.exe в режиме администратора, если нужно
pushd "C:\Qt\6.9.2\mingw_64"
call "bin\qtenv2.bat"
windeployqt.exe "C:\projects\qt\QmlAppTest\build\QmlApp.exe"  --qmldir "C:\projects\qt\QmlAppTest\qml"  --debug



popd