include(vcpkg_common_functions)

if(NOT VCPKG_LIBRARY_LINKAGE STREQUAL static)
  message( FATAL_ERROR "Only static build is supported" )
endif()

set(VERSION 1.3.0)
set(SOURCE_PATH ${CURRENT_BUILDTREES_DIR}/src/tdlib-${VERSION})

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO tdlib/td
    REF v${VERSION}
    SHA512 d69a0d0821e4d06b2d15c1eab15b6a2f374a022eecebe9f9fc7e8115213cc6f26fd1ff8dd72b77d469f10cb62548c326516b1b3cdf9a8164aae071cda0490c67
    HEAD_REF master
)
vcpkg_find_acquire_program(GPERF)

vcpkg_apply_patches(
    SOURCE_PATH ${SOURCE_PATH}
    PATCHES
    ${CMAKE_CURRENT_LIST_DIR}/tdjson_static.patch
)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS -DGPERF_EXECUTABLE:FILEPATH="${GPERF}"
)
vcpkg_install_cmake()

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)
vcpkg_fixup_cmake_targets(CONFIG_PATH lib/cmake/Td)

file(INSTALL ${SOURCE_PATH}/LICENSE_1_0.txt DESTINATION ${CURRENT_PACKAGES_DIR}/share/tdjson RENAME copyright)

vcpkg_copy_pdbs()