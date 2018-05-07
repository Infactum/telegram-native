xcopy ports\. "%vcpkg_path%\ports\" /E
vcpkg install openssl:%PLATFORM%-windows-static zlib:%PLATFORM%-windows-static icu:%PLATFORM%-windows-static tdjson:%PLATFORM%-windows-static