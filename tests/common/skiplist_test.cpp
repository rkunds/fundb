#include <gtest/gtest.h>
#include <string>
#include "common/structures/skiplist.h"

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

    EXPECT_STREQ(v1.c_str(), "value1");
    EXPECT_STREQ(v2.c_str(), "value2");
}

TEST(SkipListTest, InsertOverwrite) {
    SkipList skiplist(16, 0.5);
    std::string key1 = "key1";
    std::string value1 = "value1";

    skiplist.Insert(key1, value1);
    std::string new_val = "new_value";

    size_t size1 = skiplist.Insert(key1, new_val);

    std::string value = skiplist.Get(key1);
    EXPECT_EQ(value, "new_value");
    EXPECT_EQ(size1, 13); // "key1" (4) + "new_value" (5)
}

TEST(SkipListTest, Exists) {
    SkipList skiplist(16, 0.5);

    std::string key1 = "key1";
    std::string key2 = "key2";
    std::string key3 = "key3";
    std::string val1 = "value1";
    std::string val2 = "value2";

    skiplist.Insert(key1, val1);
    skiplist.Insert(key2, val2);
    
    EXPECT_TRUE(skiplist.Exists(key1));
    EXPECT_TRUE(skiplist.Exists(key2));
    EXPECT_FALSE(skiplist.Exists(key3));
}

TEST(SkipListTest, Delete) {
    SkipList skiplist(16, 0.5);
    std::string key1 = "key1";
    std::string key2 = "key2";
    std::string val1 = "value1";
    std::string val2 = "value2";

    skiplist.Insert(key1, val1);
    skiplist.Insert(key2, val2);

    skiplist.Delete(key1);
    EXPECT_FALSE(skiplist.Exists(key1));
    EXPECT_TRUE(skiplist.Exists(key2));

    skiplist.Delete(key2);
    EXPECT_FALSE(skiplist.Exists(key1));
    EXPECT_FALSE(skiplist.Exists(key2));
}

TEST(SkipListTest, InsertMany) {
    SkipList skiplist(16, 0.5);
    for (int i = 0; i < 10000; i++) {
        std::string key = "key" + std::to_string(i);
        std::string value = "value" + std::to_string(i);
        skiplist.Insert(key, value);
    }

    for (int i = 0; i < 10000; i++) {
        std::string key = "key" + std::to_string(i);
        std::string value = "value" + std::to_string(i);
        std::string ret = skiplist.Get(key);
        EXPECT_STREQ(ret.c_str(), value.c_str());
    }

    EXPECT_EQ(skiplist.GetNumEntries(), 10000);
}

TEST(SkipListTest, Iterator) {
    SkipList skiplist(16, 0.5);
    std::string key;
    std::string value;


    for (int i = 0; i < 1000; i++)
    {
        key = "key" + std::to_string(i);
        value = "value" + std::to_string(i);

        skiplist.Insert(key, value);
    }   
    int i = 0;
    std::string prev = "";

    for (auto it = skiplist.begin(); it != skiplist.end(); ++it) {
        auto [k, v] = *it;
        EXPECT_LT(prev, k);
        prev = k;
    }
}