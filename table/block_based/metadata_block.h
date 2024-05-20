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

        size_t max_val_size_;
        size_t max_key_size_;

    public:
        MetadataBlock();
        MetadataBlock(size_t index_block_offset, size_t num_blocks, size_t num_entries, const std::string& min_key, const std::string& max_key, size_t max_val_size, size_t max_key_size);

        void SetIndexBlockOffset(size_t offset);
        void SetNumBlocks(size_t num_blocks);
        void SetNumEntries(size_t num_entries);
        void SetMinKey(const std::string& min_key);
        void SetMaxKey(const std::string& max_key);
        void SetMaxValSize(size_t max_val_size);
        void SetMaxKeySize(size_t max_key_size);

        size_t GetBlockSize();
        size_t GetIndexBlockOffset();
        size_t GetNumBlocks();
        size_t GetNumEntries();
        size_t GetMaxValSize();
        size_t GetMaxKeySize();
        std::string GetMinKey();
        std::string GetMaxKey();
    
        bool WriteToFile(std::ofstream& file);
};