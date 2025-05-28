@echo off
setlocal enabledelayedexpansion

:: === Compiler dan flags ===
set CXX=g++
set CXXFLAGS=-std=c++17 -Wall -Iinclude -Iinclude/SDL3 -Iinclude/zketch -DZKETCH_EXPORTS

:: === Direktori ===
set SRC_DIR=src
set MODULES_DIR=%SRC_DIR%\modules
set OBJ_DIR=build\src
set BIN_DIR=bin
set OUT_DLL=%BIN_DIR%\zketch.dll
set OUT_IMPLIB=%BIN_DIR%\libzketch.dll.a

:: === Library ===
set LIB_DIR=lib
set LIBS=-L%LIB_DIR% -lSDL3 -lSDL3_image -lSDL3_ttf

:: === Cari semua source files ===
set SOURCES=

for /R %SRC_DIR% %%f in (*.cpp) do (
    set SOURCES=!SOURCES! "%%f"
)

echo [INFO] Source files found:
echo %SOURCES%

:: === Buat folder output kalau belum ada ===
if not exist %BIN_DIR% mkdir %BIN_DIR%
if not exist %OBJ_DIR% mkdir %OBJ_DIR%

echo [INFO] Building DLL...

:: === Compile dan link jadi DLL sekaligus ===
%CXX% -shared %CXXFLAGS% %SOURCES% %LIBS% -Wl,--out-implib,%OUT_IMPLIB% -o %OUT_DLL%

if %ERRORLEVEL% == 0 (
    echo [SUCCESS] DLL built: %OUT_DLL%
) else (
    echo [ERROR] Build failed!
)

pause
