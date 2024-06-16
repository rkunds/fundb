#include "skiplist.h"

SkipList::SkipList(size_t max_level, double p) : max_level(max_level), p(p), total_size(0), level(0), num_entries(0) {
    head = new Node("", "", max_level+1);
    auto time = std::time(nullptr);
    SetSeed(static_cast<unsigned int>(time));
    level = 0;  
}

void SkipList::SetSeed(unsigned int seed) {
    std::srand(seed);
}

SkipList::~SkipList() {
    Node* curr = head;
    while (curr) {
        Node* next = curr->forward[0];
        delete curr;
        curr = next;
    }

    head = nullptr;

    return;
}

size_t SkipList::RandomLevel() {
    size_t lvl = 0;
    while (std::rand() / (double) RAND_MAX < p && lvl < max_level) {
        ++lvl;
    }

    return lvl;
}

size_t SkipList::Insert(std::string& key, std::string& value) {
    if (value == "") {
        std::cout << "empty value ?? " << std::endl;
    }
    std::lock_guard<std::mutex> lock(mutex);

    std::vector<Node*> update(max_level+1, nullptr);
    Node* curr = head;

    for (int i = level; i >= 0; i--) {
        while (curr->forward[i] && curr->forward[i]->key < key) {
            curr = curr->forward[i];
        }

        update[i] = curr;
    }    

    curr = curr->forward[0];

    if (curr && curr->key == key) {
        total_size -= curr->value.size();
        curr->value = value;
        total_size += value.size();
        return total_size;
    }

    size_t new_level = RandomLevel();

    if (new_level > level) {
        for (size_t i = level + 1; i < new_level + 1; i++) {
            update[i] = head;
        }

        level = new_level;
    }

    Node* new_node = new Node(key, value, max_level + 1);

    for (size_t i = 0; i <= new_level; i++) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }

    total_size += key.size() + value.size();
    num_entries += 1;

    return total_size;
}

bool SkipList::Exists(std::string& key) {
    std::lock_guard<std::mutex> lock(mutex);

    Node* curr = head;

    for (int i = level; i >= 0; i--) {
        while (curr->forward[i] && curr->forward[i]->key < key) {
            curr = curr->forward[i];
        }
    }

    curr = curr->forward[0];

    return curr && curr->key == key && curr->value != "";
}

std::string SkipList::Get(std::string& key) {
    std::lock_guard<std::mutex> lock(mutex);

    Node* curr = head;

    for (int i = level; i >= 0; i--) {
        while (curr->forward[i] && curr->forward[i]->key < key) {
            curr = curr->forward[i];
        }
    }

    curr = curr->forward[0];

    return (curr && curr->key == key) ? curr->value : "";
}

void SkipList::Delete(std::string& key) {
    std::lock_guard<std::mutex> lock(mutex);

    std::vector<Node*> update(max_level+1, nullptr);
    Node* curr = head;

    for (int i = level; i >= 0; i--) {
        while (curr->forward[i] && curr->forward[i]->key < key) {
            curr = curr->forward[i];
        }

        update[i] = curr;
    }

    curr = curr->forward[0];

    if (curr && curr->key == key) {
        total_size -= curr->value.size();
        curr->value = ""; // represent deleted records as empty strings
        num_entries -= 1;
    }

    return;
}

void SkipList::Print() {
    std::lock_guard<std::mutex> lock(mutex);

    Node* curr = head->forward[0];

    while (curr) {
        std::cout << "Key: " << curr->key << " Value: " << curr->value << std::endl;
        curr = curr->forward[0];
    }

    return;
}

size_t SkipList::GetTotalSize() const {
    std::lock_guard<std::mutex> lock(mutex);
    return total_size;
}

size_t SkipList::GetNumEntries() const {
    return num_entries;
}