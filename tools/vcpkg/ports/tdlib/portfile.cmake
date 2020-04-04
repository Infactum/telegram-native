include(vcpkg_common_functions)

set(VERSION 1.6.1)
set(SOURCE_PATH ${CURRENT_BUILDTREES_DIR}/src/tdlib-${VERSION})

vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO tdlib/td
        REF c407b244a086e71773e0b9f8f64d4fb0536bc447
        SHA512 5e3cec2d7568bad9cb5a5e938aeab821cbac7b51477b581c25439bdfb2e398368a431a0a62dadf5030460865ddbec00e30b0afa29fbc05dcd75dd4d81eb2bd47
        HEAD_REF master
)

if (VCPKG_LIBRARY_LINKAGE STREQUAL static)
    vcpkg_apply_patches(
            SOURCE_PATH ${SOURCE_PATH}
            PATCHES
            ${CMAKE_CURRENT_LIST_DIR}/static.patch
            ${CMAKE_CURRENT_LIST_DIR}/openssl.patch
    )
endif()

vcpkg_find_acquire_program(GPERF)
vcpkg_configure_cmake(
        SOURCE_PATH ${SOURCE_PATH}
        PREFER_NINJA
        OPTIONS -DGPERF_EXECUTABLE:FILEPATH="${GPERF}"
)

vcpkg_install_cmake()
vcpkg_copy_pdbs()
vcpkg_fixup_cmake_targets(CONFIG_PATH lib/cmake/Td)

configure_file(${SOURCE_PATH}/LICENSE_1_0.txt ${CURRENT_PACKAGES_DIR}/share/${PORT}/copyright COPYONLY)

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)
if (VCPKG_LIBRARY_LINKAGE STREQUAL static)
    file(REMOVE_RECURSE
            ${CURRENT_PACKAGES_DIR}/bin
            ${CURRENT_PACKAGES_DIR}/debug/bin)
endif()