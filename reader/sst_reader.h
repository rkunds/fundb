#pragma once

#include <iostream>
#include <string>
#include <istream>
#include <fstream>

#include "table/block_based/metadata_block.h"

class SSTReader {
    public:
        SSTReader(std::string filename) : filename(filename) {}
        bool Open();

        std::string Get(const std::string& key);

        MetadataBlock& GetMetadataBlock();

    private:
        ssize_t GetDataBlockOffset(const std::string& key);
        void ParseMetadataBlock();

    std::string filename;
    std::ifstream file_;
    MetadataBlock metadata_block_;
    bool opened_ = false;

    size_t num_blocks_;
    std::string first_block_entry_;
    size_t first_block_offset_; 
    std::string last_block_entry_;
    size_t last_block_offset_;
};

