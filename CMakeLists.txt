cmake_minimum_required(VERSION 3.25)
project(PetDB)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")

include(FetchContent)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/f8d7d77.zip
)


set(COMMON
    common/utils/generate.cpp
    common/utils/threadpool.cpp

    common/structures/skiplist.cpp
)

set(TABLE_SOURCES
    table/table.cpp

    table/block_based/data_block.cpp
    table/block_based/index_block.cpp
    table/block_based/metadata_block.cpp
    table/block_based/block_builder.cpp
)

set(COMPACTION_SOURCES
    compaction/sst_manager.cpp 
)

set(MEMTABLE_SOURCES
    memtable/memtable.cpp
    compaction/sst_manager.cpp 
)

set(READER_SOURCES
    reader/sst_reader.cpp
    reader/read_engine.cpp
)

set(TEST_SOURCES
    tests/table/block_based/data_block_test.cpp
    tests/memtable/skiplist_test.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${COMMON} ${MEMTABLE_SOURCES}  ${TABLE_SOURCES})
add_executable(reader reader/reader.cpp ${COMMON} ${TABLE_SOURCES} ${READER_SOURCES})

add_executable(c_test memory_test.cpp ${COMMON} ${TABLE_SOURCES} ${READER_SOURCES}  ${MEMTABLE_SOURCES})

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(${PROJECT_NAME}_test tests/test_all.cpp ${COMMON} ${TEST_SOURCES} ${MEMTABLE_SOURCES} ${TABLE_SOURCES} ${READER_SOURCES})
target_link_libraries(${PROJECT_NAME}_test gtest_main)

add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)
target_link_libraries(${PROJECT_NAME}_test gtest_main)
