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
        ~Node() {
            forward.clear();
        }
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

        class Iterator {
        public:
            Iterator(Node* node) : current(node) {}

            Iterator& operator++() {
                current = current->forward[0];
                return *this;
            }

            std::pair<std::string, std::string> operator*() {
                return std::make_pair(current->key, current->value);
            }

            bool operator!=(const Iterator& other) const {
                return current != other.current;
            }

        private:
            Node* current;
        };

        Iterator begin() {
            return Iterator(head->forward[0]);
        }

        Iterator end() {
            return Iterator(nullptr);
        }   


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