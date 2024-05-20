#include "block_builder.h"

#include <iostream>

BlockBuilder::BlockBuilder(std::string file_name, size_t target_block_size) {
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
    file_size = 0;
}

bool BlockBuilder::Open() {
    if (!file_.is_open()) {
        return false;
    }

    return true;
}

void BlockBuilder::AddEntry(std::string& key, std::string& value) {
    bool added = curr_data_block_.AddEntry(key, value);
    size_t val_size = value.size();

    if (num_entries_ == 0) {
        metadata_block_.SetMinKey(key);
        metadata_block_.SetMaxValSize(val_size);
    }

    if (!added) {
        curr_data_block_.WriteToFile(file_);
        num_blocks_++;
        index_block_.AddEntry(curr_data_block_.GetMinKey(), curr_offset_);
        curr_offset_ += curr_data_block_.GetBlockSize();
        file_size += curr_data_block_.GetBlockSize();
        metadata_block_.SetMaxKey(curr_data_block_.GetMaxKey());
        curr_data_block_.Reset();

        curr_data_block_.AddEntry(key, value);
    } 

    if (metadata_block_.GetMaxValSize() < val_size) {
        metadata_block_.SetMaxValSize(val_size);
    }

    if (metadata_block_.GetMaxKeySize() < key.size()) {
        if (key.size() > 1000) {
            std::cout << "key size  " << key.size() << std::endl;
        }
        metadata_block_.SetMaxKeySize(key.size());
    }

    num_entries_++;

    return;
}

bool BlockBuilder::WriteAndClose() {
    if (curr_data_block_.GetNumEntries() > 0) {
        curr_data_block_.WriteToFile(file_);
        num_blocks_++;

        index_block_.AddEntry(curr_data_block_.GetMinKey(), curr_offset_);
        metadata_block_.SetMaxKey(curr_data_block_.GetMaxKey());
        curr_offset_ += curr_data_block_.GetBlockSize();
        file_size += curr_data_block_.GetBlockSize();
    }

    metadata_block_.SetNumBlocks(num_blocks_);
    metadata_block_.SetNumEntries(num_entries_);

    index_block_.WriteToFile(file_);
    metadata_block_.SetIndexBlockOffset(curr_offset_);

    curr_offset_ += index_block_.GetBlockSize();
    file_size += index_block_.GetBlockSize();
    metadata_block_.WriteToFile(file_);
    file_size += metadata_block_.GetBlockSize() + sizeof(size_t);


    file_.seekp(0, std::ios::end);
    file_.write((char*) &curr_offset_, sizeof(size_t));

    file_.close();
    if (file_.is_open()) {
        return false;
    }

    return true;
}

size_t BlockBuilder::GetFileSize() {
    return file_size;
}