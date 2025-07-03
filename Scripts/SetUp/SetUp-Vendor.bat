@echo off
setlocal

echo === Download operation started ===

set "PROJECT_DIR=Builder"
set "VENDOR_DIR=Vendor"
set "PREMAKE_DIR=premake"
set "PREMAKE_TAG=5.0.0-beta6"

curl --version >nul 2>&1 || (
    echo Error: curl is not installed.
    exit /b 1
)

tar --version >nul 2>&1 || (
    echo Error: tar is not installed.
    exit /b 1
)

pushd "..\..\%PROJECT_DIR%\%VENDOR_DIR%"
pushd "%PREMAKE_DIR%"

if exist "%PREMAKE_DIR%" (
    rmdir /S /Q "%PREMAKE_DIR%"
)

mkdir "%PREMAKE_DIR%"
pushd "%PREMAKE_DIR%"

echo Downloading premake, version %PREMAKE_TAG%...
curl -L -o premake-windows.zip "https://github.com/premake/premake-core/releases/download/v%PREMAKE_TAG%/premake-%PREMAKE_TAG%-windows.zip"
tar -xf premake-windows.zip
del premake-windows.zip

popd
popd
popd

echo === Download operation completed ===

endlocal
pause