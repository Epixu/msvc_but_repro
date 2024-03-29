cmake_minimum_required(VERSION 3.24)
project(bug_repro)
add_executable(repro Main.cpp)

target_compile_features(repro PRIVATE cxx_std_20)
if (MSVC)
    target_compile_options(repro PRIVATE /arch:AVX)
else()
    target_compile_options(repro PRIVATE -mavx)
endif()

include(FetchContent)
message(STATUS "Fetching Catch2...")
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY  https://github.com/catchorg/Catch2.git
    GIT_TAG         d4b0b34
    SOURCE_DIR      "${CMAKE_SOURCE_DIR}/external/Catch2-src"
    SUBBUILD_DIR    "${CMAKE_SOURCE_DIR}/external/Catch2-subbuild"
)
FetchContent_MakeAvailable(Catch2)

string(TOLOWER Catch2 LOWERCASE_NAME)
set(Catch2_SOURCE_DIR "${${LOWERCASE_NAME}_SOURCE_DIR}" CACHE INTERNAL "")
set(Catch2_BINARY_DIR "${${LOWERCASE_NAME}_BINARY_DIR}" CACHE INTERNAL "")

target_link_libraries(repro PRIVATE Catch2)

enable_testing()
add_test(NAME repro COMMAND repro)
