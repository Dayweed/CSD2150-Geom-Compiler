@echo OFF
setlocal enabledelayedexpansion
cd %cd%

:COMPILATION
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White DOWNLOADING DEPENDENCIES RAPIDJSON
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
git clone https://github.com/Tencent/rapidjson.git "../dependencies/rapidjson"
if %ERRORLEVEL% GEQ 1 goto :PAUSE
:COMPILATION
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White DOWNLOADING DEPENDENCIES XCORE
powershell write-host -fore White ------------------------------------------------------------------------------------------------------

echo.
rmdir "../dependencies/xcore" /S /Q
git clone https://gitlab.com/LIONant/xcore.git "../dependencies/xcore"
if %ERRORLEVEL% GEQ 1 goto :PAUSE
cd ../dependencies/xcore/builds
call UpdateDependencies.bat "return"
if %ERRORLEVEL% GEQ 1 goto :PAUSE

:COMPILATION
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White DOWNLOADING DEPENDENCIES MESHOPTIMIZER
powershell write-host -fore White ------------------------------------------------------------------------------------------------------

git clone https://github.com/zeux/meshoptimizer "../../../dependencies/meshoptimizer"
if %ERRORLEVEL% GEQ 1 goto :PAUSE
cd ../dependencies/meshoptimizer/builds
call UpdateDependencies.bat "return"
if %ERRORLEVEL% GEQ 1 goto :PAUSE
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White ASSIMP CMAKING
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
git clone https://github.com/assimp/assimp.git "../../../dependencies/assimp"
if %ERRORLEVEL% GEQ 1 goto :PAUSE
cd ../../../dependencies/assimp
cmake CMakeLists.txt 
cmake --build .

:DONE
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White DONE!!
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
goto :PAUSE

:ERROR
powershell write-host -fore Red ------------------------------------------------------------------------------------------------------
powershell write-host -fore Red DONE WITH ERRORS!!
powershell write-host -fore Red ------------------------------------------------------------------------------------------------------

:PAUSE
rem if no one give us any parameters then we will pause it at the end, else we are assuming that another batch file called us
if %1.==. pause
