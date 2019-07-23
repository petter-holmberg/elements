set (CMAKE_BUILD_TYPE Debug)
set (TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/algebra.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/array.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/array_k.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/copy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/coordinate_point.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/coordinate_vector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/invocable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lexicographical.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/map.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ordered_algebra.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ordering.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pair.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/polynomial.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/position.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rational.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/reduce.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/reverse_position.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/search.cpp)

add_executable (tests ${TEST_SOURCES})
target_link_libraries (tests Catch)
#set_property (TARGET tests PROPERTY CXX_STANDARD 20)
target_compile_options (
    tests
    PUBLIC
    -Wall
    -Werror
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
    -std=c++2a
    -fconcepts
)

add_test (elements tests)
