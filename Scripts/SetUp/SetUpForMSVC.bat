@echo off

pushd "%~dp0"

pushd ..\..

Builder\Vendor\premake\premake\premake5.exe vs2022

popd
popd

pause