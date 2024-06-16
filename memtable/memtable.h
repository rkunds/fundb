#pragma once

#include "memtable_interface.h"
#include "common/structures/skiplist.h"
#include "compaction/sst_manager.h"

class Memtable : public MemtableInterface {
    public:
        Memtable(size_t flush_threshold, size_t max_level, double p);
        Memtable(size_t flush_threshold);

        ~Memtable();

        std::string Get(std::string& key) override;
        bool Put(std::string& key, std::string& value) override;
        bool Delete(std::string& key) override;

        std::future<std::string> GetAsync(std::string &key) override;
        std::future<bool> PutAsync( std::string& key,  std::string& value) override;
        std::future<bool> DeleteAsync( std::string& key) override;

        void SetSSTManager(SSTManager* sst_manager);
        //     sst_manager_ = sst_manager;
        // }

        void FlushToL0();


    private:
        size_t max_level_ = 16;
        double p_ = 0.5;
        size_t flush_threshold_;
        SkipList* skiplist_;
        size_t total_size_;
        size_t num_entries_;

        SSTManager* sst_manager_;

        std::mutex flush_mtx_;
        std::condition_variable flush_cv_;
        bool flushing_ = false;

};

class MemtableBuilder {
    public:
        MemtableBuilder& SetFlushThreshold(size_t flush_threshold) {
            flush_threshold_ = flush_threshold;
            return *this;
        }

        MemtableBuilder& SetMaxLevel(size_t max_level) {
            max_level_ = max_level;
            return *this;
        }

        MemtableBuilder& SetP(double p) {
            p_ = p;
            return *this;
        }

        MemtableBuilder& SetSSTManager(SSTManager* sst_manager) {
            sst_manager_ = sst_manager;
            return *this;
        }

        Memtable* Build() {
            if (sst_manager_ == nullptr) {
                throw std::runtime_error("SSTManager must be set before building Memtable");
            }
            Memtable* tb = new Memtable(flush_threshold_, max_level_, p_);
            tb->SetSSTManager(sst_manager_);

            return tb;
        }

    private:
        size_t flush_threshold_ = 1000;
        size_t max_level_ = 16;
        double p_ = 0.5;
        SSTManager* sst_manager_ = nullptr;
};