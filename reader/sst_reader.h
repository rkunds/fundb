#pragma once

#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <chrono>

#include "table/block_based/metadata_block.h"

class SSTReader {
    public:
        SSTReader(std::string filename);
        bool Open();

        std::string Get(const std::string& key);

        MetadataBlock& GetMetadataBlock();

        size_t GetNumBlocks();

        class Iterator {
            public:
                Iterator(SSTReader* reader);
                Iterator& operator++();
                std::pair<std::string, std::string> operator*();
                bool operator!=(const Iterator& other) const;
                void SetToEnd();

            private:
                void ReadKeyVal();

                std::ifstream ifile_;
                std::string current_key_;
                std::string current_value_;

                size_t current_block_;
                size_t current_entry_;
                size_t num_entries_;
                size_t num_blocks_;

                SSTReader* reader_;

                bool end_ = false;
        };

        Iterator begin();
        Iterator end();

    private:
        ssize_t GetDataBlockOffset(const std::string& key);
        void ParseMetadataBlock();
        bool opened_ = false;

        std::string first_block_entry_;
        size_t first_block_offset_; 
        std::string last_block_entry_;
        size_t last_block_offset_;


    protected:
        std::string filename_;
        std::ifstream file_;
        MetadataBlock metadata_block_;

        size_t num_blocks_;
};

