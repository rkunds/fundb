#pragma once

#include <queue>

#include "reader/sst_reader.h"

/**
 * Class for an N-Way merge between N SST tables on disk. This will be used for compaction down the line.
 */

struct CompareFirst {
    bool operator()(const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b) {
        return a.first > b.first;
    }
};

class MergeIteratorBuilder {
    public:
        MergeIteratorBuilder();

        MergeIteratorBuilder& AddSSTFile(std::string sst_file) {
            sst_files_.push_back(sst_file);
            return *this;
        }

        MergeIterator Build() {
            return MergeIterator(sst_files_);
        }

    private:
        std::priority_queue<std::pair<std::string, std::string>, std::vector<std::pair<std::string, std::string>>, CompareFirst> pq_;
        std::vector<std::string> sst_files_;
        size_t level = 1;

        Verify();
};

class MergeIterator {
    public:
        MergeIterator(std::vector<std::string> sst_files, std::vector<std::unique_ptr<SSTReader>> readers, size_t level);
        void StartMerge();
    private:
        std::vector<std::unique_ptr<SSTReader>> readers_;
        std::vector<std::string> sst_files_;
        size_t level_;
        std::priority_queue<std::pair<std::string, std::string>, std::vector<std::pair<std::string, std::string>>, CompareFirst> pq_;
        bool end_ = false;
        bool started_ = false;
};