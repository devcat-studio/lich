@ECHO off

ECHO "Debug|x64 빌드 중"
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\common7\ide\devenv" lich-test.sln /nologo /build "Debug|x64"
IF %ERRORLEVEL% NEQ 0 GOTO error

ECHO "x64\Debug\lich-test.exe 실행 중"
x64\Debug\lich-test.exe
IF %ERRORLEVEL% NEQ 0 GOTO error


ECHO "Release|x64 빌드 중"
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\common7\ide\devenv" lich-test.sln /nologo /build "Release|x64"
IF %ERRORLEVEL% NEQ 0 GOTO error

ECHO "x64\Release\lich-test.exe 실행 중"
x64\Release\lich-test.exe
IF %ERRORLEVEL% NEQ 0 GOTO error


ECHO "Debug|x86 빌드 중"
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\common7\ide\devenv" lich-test.sln /nologo /build "Debug|x86"
IF %ERRORLEVEL% NEQ 0 GOTO error

ECHO "Debug\lich-test.exe 실행 중"
Debug\lich-test.exe
IF %ERRORLEVEL% NEQ 0 GOTO error


ECHO "Release|x86 빌드 중"
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\common7\ide\devenv" lich-test.sln /nologo /build "Release|x86"
IF %ERRORLEVEL% NEQ 0 GOTO error

ECHO "Release\lich-test.exe 실행 중"
Release\lich-test.exe
IF %ERRORLEVEL% NEQ 0 GOTO error



ECHO "완료"
GOTO end

:error
ECHO "실패"

:end