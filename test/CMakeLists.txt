set (CMAKE_BUILD_TYPE Debug)
set (TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/affine_space.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/algebra.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/array_double_ended.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/array_circular.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/array_single_ended.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/array_k.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/copy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/combinatorics.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/count.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fill.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/for_each.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/functional.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/invocable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lexicographical.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/list_singly_linked_front.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/list_singly_linked_front_back.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/map.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ordered_algebra.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ordering.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pair.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/partition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/polynomial.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/position.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quantify.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rational.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/reduce.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/result.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/reverse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rotate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/search.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/search_binary.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/swap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/zip.cpp)

add_executable (tests ${TEST_SOURCES})
target_link_libraries (tests -lpthread Catch)
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
    #-Weffc++
    -Wno-unused-variable
    -pedantic
    -std=c++2a
    -fconcepts
)

add_test (elements tests)
