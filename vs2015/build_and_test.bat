@ECHO off

ECHO "Debug|x64 ���� ��"
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\common7\ide\devenv" lich-test.sln /nologo /build "Debug|x64"
IF %ERRORLEVEL% NEQ 0 GOTO error

ECHO "x64\Debug\lich-test.exe ���� ��"
x64\Debug\lich-test.exe
IF %ERRORLEVEL% NEQ 0 GOTO error


ECHO "Release|x64 ���� ��"
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\common7\ide\devenv" lich-test.sln /nologo /build "Release|x64"
IF %ERRORLEVEL% NEQ 0 GOTO error

ECHO "x64\Release\lich-test.exe ���� ��"
x64\Release\lich-test.exe
IF %ERRORLEVEL% NEQ 0 GOTO error


ECHO "Debug|x86 ���� ��"
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\common7\ide\devenv" lich-test.sln /nologo /build "Debug|x86"
IF %ERRORLEVEL% NEQ 0 GOTO error

ECHO "Debug\lich-test.exe ���� ��"
Debug\lich-test.exe
IF %ERRORLEVEL% NEQ 0 GOTO error


ECHO "Release|x86 ���� ��"
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\common7\ide\devenv" lich-test.sln /nologo /build "Release|x86"
IF %ERRORLEVEL% NEQ 0 GOTO error

ECHO "Release\lich-test.exe ���� ��"
Release\lich-test.exe
IF %ERRORLEVEL% NEQ 0 GOTO error



ECHO "�Ϸ�"
GOTO end

:error
ECHO "����"

:end