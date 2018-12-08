cmake_minimum_required (VERSION 3.11)

project (Elements)
set (ELEMENTS_VERSION_MAJOR 0)
set (ELEMENTS_VERSION_MINOR 1)
configure_file (
    "${PROJECT_SOURCE_DIR}/include/config.h.in"
    "${PROJECT_BINARY_DIR}/include/config.h")

include_directories ("${PROJECT_SOURCE_DIR}/include")
include_directories ("${PROJECT_SOURCE_DIR}/include/algorithms")
include_directories ("${PROJECT_SOURCE_DIR}/include/concepts")
include_directories ("${PROJECT_SOURCE_DIR}/include/type_functions")
set (CMAKE_BUILD_TYPE Release)
set (CMAKE_CXX_COMPILER g++)

add_custom_target (
    cppcheck
    COMMAND cppcheck
    --enable=warning,style,performance,portability,information,unusedFunction,missingInclude
    --std=c++11
    --library=qt.cfg
    --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)"
    --verbose
    --check-config
    --suppress=missingInclude missingIncludeSystem
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/concepts/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/type_functions/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/algorithms/*.h)

enable_testing ()
add_subdirectory (test)
set (CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/catch)
add_library (Catch INTERFACE)
target_include_directories (Catch INTERFACE ${CATCH_INCLUDE_DIR})

add_custom_target(
    check
    DEPENDS elements
    COMMAND make cppcheck
    COMMAND make test)
