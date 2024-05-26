#include "sst_manager.h"

SSTManager::SSTManager() {
    // makedir data_dir_
    std::filesystem::create_directory(data_dir_);
    l0_flush_pool_ = new ThreadPool(1);
    StartL0FlushThread();
}

SSTManager::~SSTManager() {
    l0_flush_pool_->Stop();
    delete l0_flush_pool_;
}

void SSTManager::StartL0FlushThread() {
    // l0_flush_pool_->AddTask([this]() {
    //     while (true && !l0_flush_pool_->IsStopped()) {
    //         if (!memtables_.empty()) {
    //             std::cout << "Flushing memtable to disk" << std::endl;
    //             // auto memtable = memtables_.front();
    //             std::unique_ptr<SkipList> memtable = std::move(memtables_.front());
    //             memtables_.pop();
    //             std::string f_name = GenerateRandomString(SST_FILE_LEN);
    //             std::string f_path = data_dir_ + "/" + f_name;
    //             BlockBuilder builder(f_path, 4096);
    //             builder.Open();
    //             for (auto it = memtable->begin(); it != memtable->end(); ++it) {
    //                 auto [k, v] = *it;
    //                 builder.AddEntry(k, v);
    //             }
    //             builder.WriteAndClose();

    //         }
    //     }
    // });
    return ;
}

void SSTManager::AddMemtable(SkipList* memtable) {
    l0_flush_pool_->AddTask([this, memtable]() {
        std::cout << "Flushing memtable to disk" << std::endl;
        std::string f_name = GenerateRandomString(SST_FILE_LEN);
        std::string f_path = data_dir_ + "/" + f_name;
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