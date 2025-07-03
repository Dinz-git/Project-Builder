@echo off
setlocal

echo === Clean operation started ===

set "PROJECT_DIR=Builder"
set "VENDOR_DIR=Vendor"
set "PREMAKE_DIR=premake"

pushd "..\..\%PROJECT_DIR%\%VENDOR_DIR%"
pushd "%PREMAKE_DIR%"

if exist "%PREMAKE_DIR%" (
    rmdir /S /Q "%PREMAKE_DIR%"
    echo Removed %PREMAKE_DIR%
)

popd
popd

echo === Clean operation completed ===

endlocal
pause