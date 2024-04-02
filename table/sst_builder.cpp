#include "sst_builder.h"

#include <iostream>

SSTBuilder::SSTBuilder(std::string file_name, size_t target_block_size) {
    file_name_ = file_name;
    target_block_size_ = target_block_size;
    num_blocks_ = 0;
    num_entries_ = 0;

    file_ = std::ofstream(file_name_, std::ios::binary);

    curr_data_block_ = DataBlock(target_block_size_);
    index_block_ = IndexBlock();
    metadata_block_ = MetadataBlock();

    curr_offset_ = 0;
    curr_block_size_ = 0;
}

void SSTBuilder::AddEntry(std::string& key, std::string& value) {
    bool added = curr_data_block_.AddEntry(key, value);

    if (num_entries_ == 0) {
        metadata_block_.SetMinKey(key);
    }

    if (!added) {
        curr_data_block_.WriteToFile(file_);
        num_blocks_++;
        index_block_.AddEntry(curr_data_block_.GetMinKey(), curr_offset_);
        std::cout << "block #" << num_blocks_ << " at: " << curr_offset_ << std::endl;
        curr_offset_ += curr_data_block_.GetBlockSize();
        metadata_block_.SetMaxKey(curr_data_block_.GetMaxKey());
        curr_data_block_.Reset();
    } 

    num_entries_++;

    return;
}

void SSTBuilder::CompleteSSTFile() {
    if (curr_data_block_.GetNumEntries() > 0) {
        curr_data_block_.WriteToFile(file_);
        num_blocks_++;
        std::cout << "block #" << num_blocks_ << " at: " << curr_offset_ << std::endl;

        index_block_.AddEntry(curr_data_block_.GetMinKey(), curr_offset_);
        metadata_block_.SetMaxKey(curr_data_block_.GetMaxKey());
        curr_offset_ += curr_data_block_.GetBlockSize();
    }

    metadata_block_.SetNumBlocks(num_blocks_);
    metadata_block_.SetNumEntries(num_entries_);

    index_block_.WriteToFile(file_);
    metadata_block_.SetIndexBlockOffset(curr_offset_);

    std::cout << "index block at: " << curr_offset_ << std::endl;
    curr_offset_ += index_block_.GetBlockSize();
    std::cout << "metadata block at: " << curr_offset_ << std::endl;
    metadata_block_.WriteToFile(file_);


    file_.seekp(0, std::ios::end);
    file_.write((char*) &curr_offset_, sizeof(size_t));
    file_.close();

}