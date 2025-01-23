cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)
project(IlmBloomStore VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

# find_package(Boost 1.87 REQUIRED COMPONENTS lockfree)

# if(Boost_FOUND)
#     include_directories(${Boost_INCLUDE_DIRS})
#     target_link_libraries(ArchiveFiles PRIVATE Boost::lockfree)
# endif()

set(SOURCES
    DataSource/FileDataSource.cpp
    DataSink/FolderDataSink.cpp
    # RecordProcessor/ExtractFields.cpp
    RecordProcessor/JsonDeserializer.cpp
    DataPipeline.cpp
    Archive.cpp
)

add_executable(ArchiveFiles ${SOURCES})

target_link_libraries(ArchiveFiles PRIVATE nlohmann_json::nlohmann_json)