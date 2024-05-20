#pragma once

#include <string>
#include <fstream>

class DataBlock {
    private:
        std::string data_buffer_;
        size_t num_entries_;
        size_t target_block_size_;
        size_t curr_block_size_;

        std::string min_key_;
        std::string max_key_;

    public:
        DataBlock(size_t target_block_size);
        DataBlock();

        bool AddEntry(const std::string& key, const std::string& value);
        void WriteToFile(std::ofstream& file);

        size_t GetBlockSize();
        size_t GetNumEntries();
        std::string GetMinKey();
        std::string GetMaxKey();

        void Reset();
};