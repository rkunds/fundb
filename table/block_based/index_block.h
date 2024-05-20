#pragma once

#include <vector>
#include <string>
#include <fstream>

class IndexBlock {
    private:
        std::vector<std::string> keys_;
        std::vector<size_t> offsets_;
        size_t block_size_;

    public:
        IndexBlock();

        size_t GetBlockSize();
        void AddEntry(const std::string& key, size_t offset);
        void WriteToFile(std::ofstream& file);
};