@echo off

:: Script that launches a client and tries to connect to a local server.

pushd .
cd ../tag/x64/Debug
tag.exe -connect localhost
popd
pause
