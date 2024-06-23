#include "sst_manager.h"

SSTManager::SSTManager() {
    std::filesystem::create_directory(data_dir_);
    // create 5 levels for now, l0 - l4
    l0_flush_pool_ = new ThreadPool(1);
}

SSTManager::~SSTManager() {
    l0_flush_pool_->Stop();
    delete l0_flush_pool_;
}


void SSTManager::AddMemtable(SkipList* memtable) {
    l0_flush_pool_->AddTask([this, memtable]() {
        std::string f_name = GenerateRandomString(SST_FILE_LEN);
        std::string f_path = data_dir_ + "/" + GenerateUUID() + "-" + f_name;
        BlockBuilder builder(f_path, 4096);
        builder.Open();

        for (auto it = memtable->begin(); it != memtable->end(); ++it) {
            auto [k, v] = *it;
            builder.AddEntry(k, v);
        }

        builder.WriteAndClose();

        delete memtable;
    });
}