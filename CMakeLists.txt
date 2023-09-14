cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(Bulk VERSION ${PROJECT_VERSION})

configure_file(version.h.in version.h)

add_executable(Bulk_cli main.cpp)
add_library(Bulk lib.cpp)

set_target_properties(Bulk_cli Bulk PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(Bulk
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(Bulk_cli PRIVATE
    Bulk
)

if (MSVC)
    target_compile_options(Bulk_cli PRIVATE
        /W4  /WX
    )
    target_compile_options(Bulk PRIVATE
        /W4  /WX
    )
else ()
    target_compile_options(Bulk_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(Bulk PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS Bulk_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
