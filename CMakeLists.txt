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
    RecordProcessor/JsonDeserializer.cpp
    DataPipeline.cpp
    Archive.cpp
)

add_executable(ArchiveFiles ${SOURCES})

target_link_libraries(ArchiveFiles PRIVATE nlohmann_json::nlohmann_json ZLIB::ZLIB)

# -------------------------------------------------------
# Testing
# -------------------------------------------------------
enable_testing()

add_executable(
    test_archive_files
    tests/Test_JsonDeserializer.cpp
    RecordProcessor/JsonDeserializer.cpp
)

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