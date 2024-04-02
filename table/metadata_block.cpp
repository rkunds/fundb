#include "metadata_block.h"

#include <iostream>

MetadataBlock::MetadataBlock() {
    index_block_offset_ = 0;
    num_blocks_ = 0;
    num_entries_ = 0;

    min_key_ = "";
    max_key_ = "";
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


size_t MetadataBlock::GetBlockSize() {
    return sizeof(size_t) * 3 + min_key_.size() + max_key_.size() + 2 * sizeof(size_t);
}

bool MetadataBlock::WriteToFile(std::ofstream& file) {
    // file.seekp(0, std::ios::end);
    std::cout << "# index block offset: " << index_block_offset_ << std::endl;
    std::cout << "# current file position: " << file.tellp() << std::endl;
    file.write((char*) &index_block_offset_, sizeof(size_t));
    std::cout << "# num blocks: " << num_blocks_ << std::endl;
    file.write((char*) &num_blocks_, sizeof(size_t));
    std::cout << "# num entries: " << num_entries_ << std::endl;
    file.write((char*) &num_entries_, sizeof(size_t));

    size_t min_key_size = min_key_.size();
    file.write((char*) &min_key_size, sizeof(size_t));
    file.write(min_key_.c_str(), min_key_size);

    size_t max_key_size = max_key_.size();
    file.write((char*) &max_key_size, sizeof(size_t));
    file.write(max_key_.c_str(), max_key_size);

    return true;
}
