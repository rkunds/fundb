cmake_minimum_required(VERSION 3.25)
project(PetDB)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/f8d7d77.zip
)


set(COMMON
    common/utils/generate_kv.cpp
)

set(TABLE_SOURCES
    table/table.cpp

    table/block_based/data_block.cpp
    table/block_based/index_block.cpp
    table/block_based/metadata_block.cpp
    table/block_based/block_builder.cpp
)

set(MEMTABLE_SOURCES
    memtable/memtable.cpp
)

set(READER_SOURCES
    reader/sst_reader.cpp
)

set(TEST_SOURCES
    tests/table/block_based/data_block_test.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${COMMON} ${TABLE_SOURCES})
add_executable(reader reader/reader.cpp ${COMMON} ${TABLE_SOURCES} ${READER_SOURCES})

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()



add_executable(${PROJECT_NAME}_test tests/test_all.cpp ${COMMON} ${TEST_SOURCES} ${TABLE_SOURCES} ${READER_SOURCES})
target_link_libraries(${PROJECT_NAME}_test gtest_main)

add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)
target_link_libraries(${PROJECT_NAME}_test gtest_main)


