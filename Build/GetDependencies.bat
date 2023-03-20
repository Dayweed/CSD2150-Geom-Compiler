@echo OFF
setlocal enabledelayedexpansion
cd %cd%

:COMPILATION
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White DOWNLOADING Dependencies RAPIDJSON
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
git clone https://github.com/Tencent/rapidjson.git "../Dependencies/rapidjson"
if %ERRORLEVEL% GEQ 1 goto :PAUSE
:COMPILATION
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White DOWNLOADING Dependencies XCORE
powershell write-host -fore White ------------------------------------------------------------------------------------------------------

echo.
rmdir "../Dependencies/xcore" /S /Q
git clone https://gitlab.com/LIONant/xcore.git "../Dependencies/xcore"
if %ERRORLEVEL% GEQ 1 goto :PAUSE
cd ../Dependencies/xcore/builds
call UpdateDependencies.bat "return"
if %ERRORLEVEL% GEQ 1 goto :PAUSE

:COMPILATION
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White DOWNLOADING Dependencies MESHOPTIMIZER
powershell write-host -fore White ------------------------------------------------------------------------------------------------------

git clone https://github.com/zeux/meshoptimizer "../../../Dependencies/meshoptimizer"
if %ERRORLEVEL% GEQ 1 goto :PAUSE
cd ../Dependencies/meshoptimizer/builds
call UpdateDependencies.bat "return"
if %ERRORLEVEL% GEQ 1 goto :PAUSE
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White ASSIMP CMAKING
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
git clone https://github.com/assimp/assimp.git "../../../Dependencies/assimp"
if %ERRORLEVEL% GEQ 1 goto :PAUSE
cd ../../../Dependencies/assimp
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
