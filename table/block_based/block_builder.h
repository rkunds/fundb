#pragma once


#include <string>
#include <fstream>

#include "../table.h"

#include "data_block.h"
#include "index_block.h"
#include "metadata_block.h"

class BlockBuilder : public SSTable {
    private:
        std::string file_name_;
        std::ofstream file_;

        size_t target_block_size_; 
        size_t num_blocks_;
        size_t num_entries_;

        DataBlock curr_data_block_;
        IndexBlock index_block_;
        MetadataBlock metadata_block_;

        size_t curr_offset_;
        size_t curr_block_size_;

        std::string min_key_;
        std::string max_key_;

        bool finished_ = false;

    public: 
        BlockBuilder(std::string file_name, size_t target_block_size);
        bool Open();
        void AddEntry(std::string& key, std::string& value);
        bool WriteAndClose();
        size_t GetFileSize();
};