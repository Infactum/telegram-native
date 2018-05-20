@echo off

set _VCPKG_PATH=%~1
set _TARGET_PLATFORM=%~2

xcopy ports\. "%_VCPKG_PATH%\ports\" /E
vcpkg install openssl:%_TARGET_PLATFORM%-windows-static zlib:%_TARGET_PLATFORM%-windows-static icu:%_TARGET_PLATFORM%-windows-static tdjson:%_TARGET_PLATFORM%-windows-static