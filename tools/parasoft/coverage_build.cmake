# CMake injection for the dedicated Parasoft coverage build.
#
# Supplied with CMAKE_PROJECT_INCLUDE by coverage.sh. This is not used by
# ordinary product or regression builds.

if(NOT DEFINED CPPTEST_RUNTIME_OBJECT OR
   NOT EXISTS "${CPPTEST_RUNTIME_OBJECT}")
    message(FATAL_ERROR "CPPTEST_RUNTIME_OBJECT must name the built runtime object")
endif()
if(NOT DEFINED CPPTEST_RUNTIME_INCLUDE_DIR OR
   NOT IS_DIRECTORY "${CPPTEST_RUNTIME_INCLUDE_DIR}")
    message(FATAL_ERROR "CPPTEST_RUNTIME_INCLUDE_DIR must name the runtime headers")
endif()

add_compile_options(-DCPPTEST_COVERAGE=1)
include_directories(SYSTEM "${CPPTEST_RUNTIME_INCLUDE_DIR}")
add_link_options("${CPPTEST_RUNTIME_OBJECT}")
