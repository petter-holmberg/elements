set (CMAKE_BUILD_TYPE Debug)
set (TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_executable (tests ${TEST_SOURCES})
target_link_libraries (tests Catch)
set_property (TARGET tests PROPERTY CXX_STANDARD 17)
target_compile_options (
    tests
    PUBLIC
    -Wall
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -Wno-old-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wlogical-op
    -Wuseless-cast
    -Wdouble-promotion
    -Wformat=2
    -Weffc++
    -Wno-unused-variable
    -pedantic
    -fconcepts)

add_test (elements tests)
