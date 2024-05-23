#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <mutex>

/**
 * Creating a thread safe skiplist data structure to hold string key-value pairs
*/

class Node {
    public:
        std::string key;
        std::string value;
        std::vector<Node*> forward;

        Node(std::string key, std::string value, size_t level) : key(key), value(value), forward(level, nullptr) {}
};

class SkipList {
    public:
        SkipList(size_t max_level, double p);
        ~SkipList();

        size_t Insert(std::string key, std::string value);
        bool Exists(std::string& key);
        std::string Get(std::string& key);
        void Delete(std::string& key);
        void Print();

        size_t GetTotalSize() const;
        size_t GetNumEntries() const;
    private:
        size_t max_level;
        size_t total_size; // in bytes of all keys and values
        size_t num_entries;
        double p;
        size_t level;
        Node* head;
        mutable std::mutex mutex;

        size_t RandomLevel();
};