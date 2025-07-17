@echo off

pushd "%~dp0"

pushd ..\..

Builder\Vendor\premake\premake\premake5.exe gmake

popd
popd

pause