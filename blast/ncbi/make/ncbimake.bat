@echo off
rem  $Revision: 6.12 $
rem  +++++ Denis Vakatov, NCBI // vakatov@peony.nlm.nih.gov +++++

echo  *** Running:  %0  ***

if "%__PATH_MS%"==""  set __PATH_MS=C:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin;C:\Program Files\Microsoft Visual Studio\Vc98
rem if "%__PATH_MS%"==""  set __PATH_MS=C:\Program Files\DevStudio\SharedIDE\bin;C:\Program Files\DevStudio\VC
rem if "%__PATH_MS%"==""  set __PATH_MS=c:\msdev

if "%__PATH_BOR%"==""  set __PATH_BOR=c:\bc5
rem if "%__PATH_BOR%"==""  set __PATH_BOR=c:\bc45

set OPENGL=1
rem set LIBPNG_DIR=..\..\libpng
rem set ZLIB_DIR=..\..\zlib


echo  ***** Copy/Build/User flag  *****

if "%4"=="copy"  goto l_COPY
if "%4"==""      goto l_BUILD
if "%4"=="noexe" goto l_BUILD
if "%4"=="user"  goto l_USER
goto l_USAGE
:l_COPY
:l_BUILD
set __MAKEPATH=.
set __NCBIHOME=NNNNNNN
goto end_C_T
:l_USER
if "%6"==""  goto l_USAGE
if "%5"==""  goto l_USAGE
set __MAKEPATH=%6\make
set __NCBIHOME=NCBIHOME
:end_C_T


echo  ***** Debug/NoDebug  *****

if "%3"=="D"  goto l_DEB
if "%3"=="O"  goto l_OPT
goto l_USAGE
:l_DEB
set __DBUG=DBUG
goto end_D_O
:l_OPT
set __DBUG=DDDD
:end_D_O


echo  ***** Window/Console  *****

if "%2"=="W"  goto l_WIN
if "%2"=="C"  goto l_CON
goto l_USAGE
:l_WIN
set __CONSOLE=CCCCCCC
goto end_W_C
:l_CON
set __CONSOLE=CONSOLE
:end_W_C


echo  ***** MSVC; DLL(msvc); Borland  *****

if "%1"=="M"  goto l_MS
if "%1"=="D"  goto l_DLL
if "%1"=="B"  goto l_BOR
goto l_USAGE

:l_MS
set __COMP=MS
set BASE_PATH=%__PATH_MS%
set __MAKE=nmake
goto end_COMP
:l_DLL
set __COMP=DLL
set BASE_PATH=%__PATH_MS%
set __MAKE=nmake
goto end_COMP
:l_BOR
set __COMP=BOR
set BASE_PATH=%__PATH_BOR%
set __MAKE=make -N
set OPENGL=
goto end_COMP

:end_COMP


echo  ***** Set Environment  *****

set __PATH=%PATH%
set __LIB=%LIB%
set __BIN=%BIN%
set __INCLUDE=%INCLUDE%

set LIB=%BASE_PATH%\lib
set BIN=%BASE_PATH%\bin
set INCLUDE=%BASE_PATH%\include
set PATH=%BIN%;%PATH%


echo  ***** Run Makefile  *****

echo %__MAKE% -f %__MAKEPATH%\makefile.dos %__COMP%=1 %__CONSOLE%=1 %__DBUG%=1 MAKEUSER=%5.mak %__NCBIHOME%=%6 %4
%__MAKE% -f %__MAKEPATH%\makefile.dos %__COMP%=1 %__CONSOLE%=1 %__DBUG%=1 MAKEUSER=%5.mak %__NCBIHOME%=%6 %4


echo  *****  Reset Environment  *****

set LIB=%__LIB%
set BIN=%__BIN%
set INCLUDE=%__INCLUDE%
set PATH=%__PATH%

set __LIB=
set __BIN=
set __INCLUDE=
set __PATH=

set __DBUG=
set __COMP=
set __CONSOLE=
set __MAKE=
set __NCBIHOME=
set __MAKEPATH=

goto l_EXIT


echo  ***** Usage  *****

:l_USAGE
echo "Usage:"
echo " ncbimake {M|B|D} {W|C} {O|D} [noexe | copy | user <makefile> <toolkit_path>]"
echo " (M)icrosoft / (B)orland compiler / (D)LL"
echo " (W)indows/(C)onsole application"
echo " (D)ebug/(O)ptimized version"
echo " "
echo " Argument #4:"
echo "  (copy)  -- deploy all sources(must be run from MAKE directory)"
echo "  ()      -- build in the current directory(created by 'copy')"
echo "  (noexe) -- same as () but build libraries only(no executables)"
echo "  (user)  -- must be followed by(both):"
echo "     <makefile>      -- user's makefile basename (will be completed by .MAK)"
echo "     <toolkit_path>  -- path to the NCBI toolkit"
echo " "
echo "For more details read MAKE/README.DOS"


echo  ***** Exit  *****

:l_EXIT
set __PATH_16=
set __PATH_MS=
set __PATH_BOR=

echo  *** Exiting:  %0  ***


