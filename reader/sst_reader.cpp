#include "sst_reader.h"

SSTReader::SSTReader(std::string filename) {
    this->filename_ = filename;
    
    try {
        Open();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

bool SSTReader::Open() {
    file_.open(filename_, std::ios::binary);
    if (!file_.is_open()) {
        std::cerr << "Failed to open file: " << filename_ << std::endl;
        return false;
    }

    opened_ = true;

    ParseMetadataBlock();

    return true;
}

void SSTReader::ParseMetadataBlock() {
    size_t file_size = 0;
    file_.seekg(0, std::ios::end);
    file_size = file_.tellg();
    file_size -= sizeof(size_t);

    first_block_offset_ = 0;
    last_block_offset_ = 0;

    file_.seekg(file_size, std::ios::beg);
    size_t metadata_offset = 0;
    file_.read((char*) &metadata_offset, sizeof(size_t));

    file_.seekg(metadata_offset, std::ios::beg);

    size_t index_offset = 0;
    size_t num_blocks = 0;
    size_t num_entries = 0;
    size_t min_key_size = 0;
    size_t max_key_len = 0;
    size_t max_val_len = 0;

    std::string min_key = "";
    std::string max_key = "";

    file_.read((char*) &index_offset, sizeof(size_t));
    file_.read((char*) &num_blocks, sizeof(size_t));
    file_.read((char*) &num_entries, sizeof(size_t));

    file_.read((char*) &min_key_size, sizeof(size_t));
    min_key.resize(min_key_size);
    file_.read(&min_key[0], min_key_size);

    file_.read((char*) &max_key_len, sizeof(size_t));
    max_key.resize(max_key_len);
    file_.read(&max_key[0], max_key_len);

    file_.read((char*) &max_val_len, sizeof(size_t));
    file_.read((char*) &max_key_len, sizeof(size_t));

    metadata_block_ = MetadataBlock(index_offset, num_blocks, num_entries, min_key, max_key, max_val_len, max_key_len);

    num_blocks_ = num_blocks;
    size_t index_block_offset = metadata_block_.GetIndexBlockOffset();
    file_.seekg(index_block_offset + sizeof(size_t), std::ios::beg);

    size_t first_key_offset = 0;
    file_.read((char*) &first_key_offset, sizeof(size_t));
    size_t last_entry = (num_blocks_ - 1) * sizeof(size_t);
    file_.seekg(index_block_offset + sizeof(size_t) + last_entry, std::ios::beg);
    size_t last_key_offset = 0;
    file_.read((char*) &last_key_offset, sizeof(size_t));


    size_t first_i_entry = 0;
    file_.seekg(index_block_offset + first_key_offset, std::ios::beg);
    file_.read((char*) &first_i_entry, sizeof(size_t));
    first_block_entry_.resize(first_i_entry);
    file_.read(&first_block_entry_[0], first_i_entry);
    file_.read((char*)&first_block_offset_, sizeof(size_t));


    size_t last_i_entry = 0;
    file_.seekg(index_block_offset + last_key_offset, std::ios::beg);
    file_.read((char*) &last_i_entry, sizeof(size_t));
    last_block_entry_.resize(last_i_entry);
    file_.read(&last_block_entry_[0], last_i_entry);
    file_.read((char*)&last_block_offset_, sizeof(size_t));
}

MetadataBlock& SSTReader::GetMetadataBlock() {
    return metadata_block_;
}

/**
 * We take advantage of the index block in the SST File that has references to
 * the offsets to the blocks that contain the data. We perform a binary search 
 * on the first keys of each block in order to find the block that contains the
 * key of interest.
*/
ssize_t SSTReader::GetDataBlockOffset(const std::string& key) {
    if (!opened_) {
        throw std::runtime_error("File not opened");
    }

    if (key < first_block_entry_ || key > metadata_block_.GetMaxKey()) {
        return -1;
    }

    if (key >= last_block_entry_) {
        return last_block_offset_;
    }

    size_t curr = 0;
    size_t end = num_blocks_ - 1;

    while (curr <= end) {
        size_t mid = (curr + end) / 2;
        size_t mid_offset = metadata_block_.GetIndexBlockOffset() + mid * sizeof(size_t);
        file_.seekg(mid_offset, std::ios::beg);

        size_t key_offset = 0;
        file_.read((char*) &key_offset, sizeof(size_t));
        file_.seekg(metadata_block_.GetIndexBlockOffset() + key_offset, std::ios::beg);

        size_t key_size = 0;
        size_t block_offset = 0;
        file_.read((char*) &key_size, sizeof(size_t));
        std::string curr_key;
        curr_key.resize(key_size);
        file_.read(&curr_key[0], key_size);
        file_.read((char*) &block_offset, sizeof(size_t));

        size_t next_key_size = 0;
        std::string next_key_str;
        size_t next_key_offset = 0;

        file_.read((char*) &next_key_size, sizeof(size_t));
        next_key_str.resize(next_key_size);
        file_.read(&next_key_str[0], next_key_size);
        file_.read((char*) &next_key_offset, sizeof(size_t));

        if (key >= curr_key && key < next_key_str) {
            return block_offset;
        } else if (curr_key < key) {
            curr = mid + 1;
        } else {
            end = mid - 1;
        }
    }

    return -1;
}

std::string SSTReader::Get(const std::string& key) {
    if (!opened_) {
        throw std::runtime_error("File not opened");
    }

    ssize_t block_offset = GetDataBlockOffset(key);

    if (block_offset == -1) {
        return "";
    }

    file_.seekg(block_offset, std::ios::beg);

    size_t num_entries = 0;
    file_.read((char*) &num_entries, sizeof(size_t));

    for (int i = 0; i < num_entries; i++) {
        std::string key_val;
        std::string data_val;
        size_t key_size = 0;
        file_.read((char*) &key_size, sizeof(size_t));
        key_val.resize(key_size);
        file_.read(&key_val[0], key_size);
        size_t val_size = 0;
        file_.read((char*) &val_size, sizeof(size_t));
        data_val.resize(val_size);
        file_.read(&data_val[0], val_size);


        if (key_val == key) {
            return data_val;
        }

        if (key_val > key) {
            return "";
        }
    }

    return "";
}

size_t SSTReader::GetNumBlocks() {
    return num_blocks_;
}

SSTReader::Iterator::Iterator(SSTReader* reader) : reader_(reader) {
    current_block_ = 0;
    current_entry_ = 0;
    num_entries_ = 0;

    ifile_.open(reader_->filename_, std::ios::binary); 
    ifile_.seekg(0, std::ios::beg);

    num_blocks_ = reader_->GetNumBlocks();  
    ifile_.read((char*) &num_entries_, sizeof(size_t));

    ReadKeyVal();
}

void SSTReader::Iterator::ReadKeyVal() {
    if (current_entry_ >= num_entries_) {
        current_block_++;
        size_t entries_in_block = 0;
        ifile_.read((char*) &entries_in_block, sizeof(size_t));
        num_entries_ = entries_in_block;
        current_entry_ = 0;
    }

    if (current_block_ >= num_blocks_) {
        current_key_ = "";
        current_value_ = "";
        end_ = true;

        SetToEnd();

        return;
    }

    size_t key_size = 0;
    size_t val_size = 0;
    ifile_.read((char*) &key_size, sizeof(size_t));
    current_key_.resize(key_size);
    ifile_.read(&current_key_[0], key_size);

    ifile_.read((char*) &val_size, sizeof(size_t));
    current_value_.resize(val_size);
    ifile_.read(&current_value_[0], val_size);

    current_entry_++;
}

SSTReader::Iterator& SSTReader::Iterator::operator++() {
    ReadKeyVal();
    return *this;
}

std::pair<std::string, std::string> SSTReader::Iterator::operator*() {
    return std::make_pair(current_key_, current_value_);
}

bool SSTReader::Iterator::operator!=(const Iterator& other) const {
    return current_block_ != other.current_block_ ||
           current_entry_ != other.current_entry_  ||
            end_ != other.end_;
}

SSTReader::Iterator SSTReader::begin() {
    return Iterator(this);
}

SSTReader::Iterator SSTReader::end() {
    Iterator it(this);
    it.SetToEnd();
    return it;
}

void SSTReader::Iterator::SetToEnd() {
    current_block_ = num_blocks_;
    current_entry_ = (size_t) - 1;
    end_ = true;
}