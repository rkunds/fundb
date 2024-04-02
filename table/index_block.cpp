#include "index_block.h"
#include <iostream>

IndexBlock::IndexBlock() {
    block_size_ = 0;
    keys_ = std::vector<std::string>();
    offsets_ = std::vector<size_t>();
}

size_t IndexBlock::GetBlockSize() {
    return block_size_;
}

void IndexBlock::AddEntry(const std::string& key, size_t offset) {
    keys_.push_back(key);
    offsets_.push_back(offset);
}

void IndexBlock::WriteToFile(std::ofstream& file) {
    // file.seekp(0, std::ios::end);
    std::cout << "$ curr file position: " << file.tellp() << std::endl;

    size_t num_entries = keys_.size();
 
    std::vector<size_t> key_offsets = std::vector<size_t>();
    size_t curr_offset = sizeof(size_t) + num_entries * sizeof(size_t);
    for (int i = 0; i < num_entries; i++) {
        key_offsets.push_back(curr_offset);
        curr_offset += keys_[i].size() + 2 * sizeof(size_t);
    }

    file.write((char*) &num_entries, sizeof(size_t));
    block_size_ += sizeof(size_t);

    for (int i = 0; i < num_entries; i++) {
        file.write((char*) &key_offsets[i], sizeof(size_t));
        block_size_ += sizeof(size_t);
    }

    for (int i = 0; i < num_entries; i++) {
        size_t key_size = keys_[i].size();
        file.write((char*) &key_size, sizeof(size_t));
        block_size_ += sizeof(size_t);
        file.write(keys_[i].c_str(), key_size);
        block_size_ += key_size;
        file.write((char*) &offsets_[i], sizeof(size_t));
        block_size_ += sizeof(size_t);
    }

    std::cout << "$ end file position: " << file.tellp() << std::endl;
    std::cout << "$ block size: " << block_size_ << std::endl;
}

