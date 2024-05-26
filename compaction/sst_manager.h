#pragma once

#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <filesystem>

#include "table/block_based/block_builder.h"
#include "common/utils/generate.h"
#include "common/utils/threadpool.h"
#include "common/structures/skiplist.h"

const int SST_FILE_LEN = 10;

class SSTManager {
    public:
        SSTManager();
        ~SSTManager();

        void AddMemtable(SkipList* memtable);

    private:
        std::string data_dir_ = "db_data/";
        std::vector<std::string> sst_files_;
        std::queue<std::unique_ptr<SkipList>> memtables_;
        ThreadPool* l0_flush_pool_;

        void StartL0FlushThread();
};