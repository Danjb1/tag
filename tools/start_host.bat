@echo off

:: Script that launches a host.

pushd .
cd ../tag/x64/Debug
tag.exe -host
popd
pause
