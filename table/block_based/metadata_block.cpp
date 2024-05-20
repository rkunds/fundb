#include "metadata_block.h"

#include <iostream>

MetadataBlock::MetadataBlock() {
    index_block_offset_ = 0;
    num_blocks_ = 0;
    num_entries_ = 0;

    max_val_size_ = 0;
    max_key_size_ = 0;

    min_key_ = "";
    max_key_ = "";
}

MetadataBlock::MetadataBlock(size_t index_block_offset, size_t num_blocks, size_t num_entries, const std::string& min_key, const std::string& max_key, size_t max_val_size, size_t max_key_size) {
    index_block_offset_ = index_block_offset;
    num_blocks_ = num_blocks;
    num_entries_ = num_entries;

    min_key_ = min_key;
    max_key_ = max_key;

    max_val_size_ = max_val_size;
    max_key_size_ = max_key_size;
}

void MetadataBlock::SetIndexBlockOffset(size_t offset) {
    index_block_offset_ = offset;
}

void MetadataBlock::SetNumBlocks(size_t num_blocks) {
    num_blocks_ = num_blocks;
}

void MetadataBlock::SetNumEntries(size_t num_entries) {
    num_entries_ = num_entries;
}

void MetadataBlock::SetMinKey(const std::string& min_key) {
    min_key_ = min_key;
}

void MetadataBlock::SetMaxKey(const std::string& max_key) {
    max_key_ = max_key;
}

void MetadataBlock::SetMaxValSize(size_t max_val_size) {
    this->max_val_size_ = max_val_size;
}

void MetadataBlock::SetMaxKeySize(size_t max_key_size) {
    max_key_size_ = max_key_size;
}

size_t MetadataBlock::GetBlockSize() {
    return sizeof(size_t) * 3 + min_key_.size() + max_key_.size() + 4 * sizeof(size_t);
}

size_t MetadataBlock::GetIndexBlockOffset() {
    return index_block_offset_;
}

size_t MetadataBlock::GetNumBlocks() {
    return num_blocks_;
}

size_t MetadataBlock::GetNumEntries() {
    return num_entries_;
}

std::string MetadataBlock::GetMinKey() {
    return min_key_;
}

std::string MetadataBlock::GetMaxKey() {
    return max_key_;
}

size_t MetadataBlock::GetMaxValSize() {
    return max_val_size_;
}

size_t MetadataBlock::GetMaxKeySize() {
    return max_key_size_;
}

bool MetadataBlock::WriteToFile(std::ofstream& file) {
    file.write((char*) &index_block_offset_, sizeof(size_t));
    file.write((char*) &num_blocks_, sizeof(size_t));
    file.write((char*) &num_entries_, sizeof(size_t));

    size_t min_key_size = min_key_.size();
    file.write((char*) &min_key_size, sizeof(size_t));
    file.write(min_key_.c_str(), min_key_size);

    size_t max_key_size = max_key_.size();
    file.write((char*) &max_key_size, sizeof(size_t));
    file.write(max_key_.c_str(), max_key_size);

    file.write((char*) &max_val_size_, sizeof(size_t));
    file.write((char*) &max_key_size_, sizeof(size_t));

    return true;
}

