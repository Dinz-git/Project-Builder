@echo off
setlocal enabledelayedexpansion

echo === Clean operation started ===

set "MAIN_DIR=Project-Builder"
set "PROJECT_DIR=Builder"
set "EXECUTABLE_DIR=prj"
set "VENDOR_DIR=Vendor"
set "BIN_DIR=bin"
set "BININT_DIR=bin-int"
set "VS_DIR=.vs"

pushd ..\..

if exist "%VS_DIR%" (
    rmdir /S /Q "%VS_DIR%"
    echo Removed %VS_DIR%
)

echo Cleaning %MAIN_DIR%...
del /F /Q *.sln Makefile 2>nul

pushd "%PROJECT_DIR%"

for %%D in ("%BIN_DIR%" "%BININT_DIR%") do (
    if exist "%%~D" (
        rmdir /S /Q "%%~D"
        echo Removed %%~D
    )
)

pushd "%EXECUTABLE_DIR%"

echo Cleaning %EXECUTABLE_DIR%...
del /F /Q *.vcxproj *.vcxproj.filters *.user Makefile 2>nul

popd
popd
popd

echo === Clean operation completed ===

endlocal
pause