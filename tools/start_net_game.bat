@echo off

:: Script that launches a host and client and connects them together.

pushd .
cd ../tag/x64/Debug
start tag.exe -host
start tag.exe -connect localhost
popd
