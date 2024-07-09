#include "merge_iterator.h"

MergeIteratorBuilder::MergeIteratorBuilder() {
    pq_ = std::priority_queue<std::pair<std::string, std::string>, std::vector<std::pair<std::string, std::string>>, CompareFirst>();
    sst_files_ = std::vector<std::string>();
}


MergeIteratorBuilder& MergeIteratorBuilder::AddSSTFile(std::string sst_file) {
    sst_files_.push_back(sst_file);
    return *this;
}



