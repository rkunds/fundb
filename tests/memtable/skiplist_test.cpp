#include <gtest/gtest.h>
#include <string>
#include "memtable/skiplist.h"

TEST(SkipListTest, InsertAndGet) {
    SkipList skiplist(16, 0.5);
    std::string key1 = "key1";
    std::string key2 = "key2";
    std::string value1 = "value1";
    std::string value2 = "value2";

    skiplist.Insert(key1, value1);
    skiplist.Insert(key2, value2);

    std::string v1 = skiplist.Get(key1);
    std::string v2 = skiplist.Get(key2);

    bool ret = skiplist.Exists(key2);

    skiplist.Print();
    std::cout << skiplist.GetNumEntries() << std::endl;

    // EXPECT_STREQ(v1, "value1");
    EXPECT_STREQ(v1.c_str(), "value1");
    EXPECT_STREQ(v2.c_str(), "value2");
}

TEST(SkipListTest, InsertOverwrite) {
    SkipList skiplist(16, 0.5);
    skiplist.Insert("key1", "value1");
    size_t size1 = skiplist.Insert("key1", "new_value");

    std::string key1 = "key1";
    std::string value = skiplist.Get(key1);
    EXPECT_EQ(value, "new_value");
    EXPECT_EQ(size1, 13); // "key1" (4) + "new_value" (5)
}

TEST(SkipListTest, Exists) {
    SkipList skiplist(16, 0.5);
    skiplist.Insert("key1", "value1");
    skiplist.Insert("key2", "value2");

    std::string key1 = "key1";
    std::string key2 = "key2";
    std::string key3 = "key3";

    EXPECT_TRUE(skiplist.Exists(key1));
    EXPECT_TRUE(skiplist.Exists(key2));
    EXPECT_FALSE(skiplist.Exists(key3));
}

TEST(SkipListTest, Delete) {
    SkipList skiplist(16, 0.5);
    skiplist.Insert("key1", "value1");
    skiplist.Insert("key2", "value2");

    std::string key1 = "key1";
    std::string key2 = "key2";

    skiplist.Delete(key1);
    EXPECT_FALSE(skiplist.Exists(key1));
    EXPECT_TRUE(skiplist.Exists(key2));

    skiplist.Delete(key2);
    EXPECT_FALSE(skiplist.Exists(key1));
    EXPECT_FALSE(skiplist.Exists(key2));
}