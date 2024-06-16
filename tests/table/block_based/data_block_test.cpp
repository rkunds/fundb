#include <gtest/gtest.h>
#include "table/block_based/data_block.h"

TEST(DataBlockTest, DefaultConstructor) {
    DataBlock block;
    EXPECT_EQ(block.GetBlockSize(), sizeof(size_t));
    EXPECT_EQ(block.GetNumEntries(), 0);
    EXPECT_EQ(block.GetMinKey(), "");
    EXPECT_EQ(block.GetMaxKey(), "");
}

TEST(DataBlockTest, ConstructorWithTargetBlockSize) {
    const size_t target_size = 1024;
    DataBlock block(target_size);
    EXPECT_EQ(block.GetBlockSize(), sizeof(size_t));
    EXPECT_EQ(block.GetNumEntries(), 0);
    EXPECT_EQ(block.GetMinKey(), "");
    EXPECT_EQ(block.GetMaxKey(), "");
}

TEST(DataBlockTest, AddEntry) {
    DataBlock block;
    EXPECT_TRUE(block.AddEntry("key1", "value1"));
    EXPECT_EQ(block.GetNumEntries(), 1);
    EXPECT_EQ(block.GetMinKey(), "key1");
    EXPECT_EQ(block.GetMaxKey(), "key1");

    EXPECT_TRUE(block.AddEntry("key2", "value2"));
    EXPECT_EQ(block.GetNumEntries(), 2);
    EXPECT_EQ(block.GetMinKey(), "key1");
    EXPECT_EQ(block.GetMaxKey(), "key2");
}

TEST(DataBlockTest, AddEntryExceedsBlockSize) {
    const size_t target_size = 5;
    DataBlock block(target_size);
    EXPECT_TRUE(block.AddEntry("key1", "value1"));
    EXPECT_FALSE(block.AddEntry("key2", "value2"));
    EXPECT_EQ(block.GetNumEntries(), 1);
}

TEST(DataBlockTest, WriteToFile) {
    DataBlock block;
    block.AddEntry("key1", "value1");
    block.AddEntry("key2", "value2");

    std::ofstream file("test.dat", std::ios::binary);
    block.WriteToFile(file);
    file.close();

    // Verify the contents of the file
    std::ifstream input_file("test.dat", std::ios::binary);
    size_t num_entries;
    input_file.read((char*)&num_entries, sizeof(size_t));
    EXPECT_EQ(num_entries, 2);

    input_file.close();
    std::remove("test.dat");
}

TEST(DataBlockTest, Reset) {
    DataBlock block;
    block.AddEntry("key1", "value1");
    block.AddEntry("key2", "value2");
    EXPECT_EQ(block.GetNumEntries(), 2);

    block.Reset();
    EXPECT_EQ(block.GetBlockSize(), sizeof(size_t));
    EXPECT_EQ(block.GetNumEntries(), 0);
    EXPECT_EQ(block.GetMinKey(), "");
    EXPECT_EQ(block.GetMaxKey(), "");
}