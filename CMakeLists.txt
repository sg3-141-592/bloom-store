cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)
project(IlmBloomStore VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)

FetchContent_Declare(
    bloom_filter
    GIT_REPOSITORY https://github.com/ArashPartow/bloom.git
    GIT_TAG master
)
FetchContent_MakeAvailable(bloom_filter)

include_directories(${bloom_filter_SOURCE_DIR})

include(FetchContent)

FetchContent_Declare(
    libbloom
    GIT_REPOSITORY https://github.com/jvirkki/libbloom.git
    GIT_TAG v2.0
)
FetchContent_MakeAvailable(libbloom)

include_directories(${libbloom_SOURCE_DIR})
add_custom_target(build_libbloom ALL
    COMMAND make -C ${libbloom_SOURCE_DIR}
)

add_library(libbloom_static STATIC IMPORTED)
set_target_properties(libbloom_static PROPERTIES
    IMPORTED_LOCATION ${libbloom_SOURCE_DIR}/build/libbloom.a
)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

find_package(ZLIB REQUIRED)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(googletest)

set(SOURCES
    DataSource/FileDataSource.cpp
    DataSink/FolderDataSink.cpp
    DataSink/CompressBundle.cpp
    RecordProcessor/JsonDeserializer.cpp
    DataPipeline.cpp
    Archive.cpp
    Config.cpp
    ProgramLock.cpp
)

add_executable(ArchiveFiles ${SOURCES})

add_dependencies(ArchiveFiles build_libbloom)

target_link_libraries(ArchiveFiles PRIVATE nlohmann_json::nlohmann_json ZLIB::ZLIB libbloom_static)

# -------------------------------------------------------
# Searching
# -------------------------------------------------------
add_executable(Search Search.cpp Config.cpp DataSink/CompressBundle.cpp)

target_link_libraries(Search PRIVATE nlohmann_json::nlohmann_json ZLIB::ZLIB libbloom_static)

# -------------------------------------------------------
# Testing
# -------------------------------------------------------
enable_testing()

file(GLOB TEST_SOURCES tests/Test_*.cpp)

add_executable(
    test_archive_files
    ${TEST_SOURCES}
    RecordProcessor/JsonDeserializer.cpp
    Config.cpp
)

target_compile_options(test_archive_files PRIVATE -fprofile-instr-generate -fcoverage-mapping)
target_link_options(test_archive_files PRIVATE -fprofile-instr-generate -fcoverage-mapping)

target_link_libraries(
    test_archive_files
    PRIVATE
    gtest_main
    nlohmann_json::nlohmann_json
    ZLIB::ZLIB
)

add_test(
    NAME test_archive_files
    COMMAND test_archive_files
)