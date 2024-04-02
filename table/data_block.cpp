#include "data_block.h"

DataBlock::DataBlock(size_t target_block_size) {
    data_buffer_ = "";
    num_entries_ = 0;    
    curr_block_size_ = sizeof(size_t);

    target_block_size_ = target_block_size;

    min_key_ = "";
    max_key_ = "";
}

DataBlock::DataBlock() {
    data_buffer_ = "";
    num_entries_ = 0;
    curr_block_size_ = sizeof(size_t);

    target_block_size_ = 4096;

    min_key_ = "";
    max_key_ = "";
}

bool DataBlock::AddEntry(const std::string &key, const std::string& value) {
    size_t key_size = key.size();
    size_t value_size = value.size();

    if (curr_block_size_ + key_size + value_size + 2 * sizeof(size_t) > target_block_size_ && num_entries_ > 0) {
        return false;
    }

    data_buffer_.append((char*) &key_size, sizeof(size_t));
    data_buffer_.append(key);
    data_buffer_.append((char*) &value_size, sizeof(size_t));
    data_buffer_.append(value);

    curr_block_size_ += key_size + value_size + 2 * sizeof(size_t);

    num_entries_++;

    if (min_key_ == "") {
        min_key_ = key;
    }

    max_key_ = key;

    return true;
}

void DataBlock::WriteToFile(std::ofstream &file) {
    file.write((char*) &num_entries_, sizeof(size_t));
    file.write(data_buffer_.c_str(), this->GetBlockSize() - sizeof(size_t));
}

size_t DataBlock::GetBlockSize() {
    // round up to nearest multiple of target block size
    return (curr_block_size_ + target_block_size_ - 1) / target_block_size_ * target_block_size_;
}

size_t DataBlock::GetNumEntries() {
    return num_entries_;
}

std::string DataBlock::GetMinKey() {
    return min_key_;
}

std::string DataBlock::GetMaxKey() {
    return max_key_;
}

void DataBlock::Reset() {
    data_buffer_ = "";
    num_entries_ = 0;
    curr_block_size_ = sizeof(size_t);

    min_key_ = "";
    max_key_ = "";
}