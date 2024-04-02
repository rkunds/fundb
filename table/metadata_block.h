#pragma once

#include <string>
#include <fstream>

class MetadataBlock {
    private:
        size_t index_block_offset_;
        size_t num_blocks_;
        size_t num_entries_;

        std::string min_key_;
        std::string max_key_;

    public:
        MetadataBlock();

        void SetIndexBlockOffset(size_t offset);
        void SetNumBlocks(size_t num_blocks);
        void SetNumEntries(size_t num_entries);
        void SetMinKey(const std::string& min_key);
        void SetMaxKey(const std::string& max_key);

        size_t GetBlockSize();
        bool WriteToFile(std::ofstream& file);
};