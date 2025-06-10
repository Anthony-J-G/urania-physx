@ECHO OFF

pushd %~dp0

REM Command file for Sphinx documentation

if "%SPHINXBUILD%" == "" (
	set SPHINXBUILD=sphinx-autobuild
)
set SOURCEDIR=source
set BUILDDIR=build

%SPHINXBUILD% >NUL 2>NUL
if errorlevel 9009 (
	echo.
	echo.The 'sphinx-autobuild' command was not found. Make sure you have Sphinx
	echo.autobuild installed, then set the SPHINXBUILD environment variable
	echo.to point to the full path of the 'sphinx-autobuild' executable.
	echo.Alternatively you may add the Sphinx directory to PATH.
	echo.
	echo.If you don't have Sphinx installed, grab it from
	echo.https://www.sphinx-doc.org/
	exit /b 1
)

if "%1" == "" goto help

%SPHINXBUILD% %* %SOURCEDIR% %BUILDDIR%
goto end

:help
%SPHINXBUILD% %SOURCEDIR% %BUILDDIR%

:end
popd
