#include "memtable.h"

Memtable::Memtable(size_t flush_threshold, size_t max_level, double p) {
    flush_threshold_ = flush_threshold;
    max_level_ = max_level;
    p_ = p;
    skiplist_ = new SkipList(max_level_, p_);
    total_size_ = 0;
    num_entries_ = 0;

}

Memtable::Memtable(size_t flush_threshold) {
    flush_threshold_ = flush_threshold;
    skiplist_ = new SkipList(max_level_, p_);
    total_size_ = 0;
    num_entries_ = 0;
}

Memtable::~Memtable() {
    delete skiplist_;
}

std::string Memtable::Get(std::string& key) {
    return skiplist_->Get(key);
}

bool Memtable::Put(std::string& key, std::string& value) {
    size_t new_size = skiplist_->Insert(key, value);
    total_size_ = new_size;
    num_entries_++;

    if (total_size_ >= flush_threshold_) {
        std::unique_lock<std::mutex> lock(flush_mtx_);
        if (flushing_) {
            std::cout << std::this_thread::get_id() << " is waiting" << std::endl;
            flush_cv_.wait(lock, [this] {return !flushing_;});
        } else {
            std::cout << std::this_thread::get_id() << " is initiating the flush" << std::endl;
            flushing_ = true;
            this->FlushToL0();
            flushing_ = false;
            std::cout << std::this_thread::get_id() << " is notifying the other threads" << std::endl;
            flush_cv_.notify_all();
        }
    }

    return true;
}

bool Memtable::Delete(std::string& key) {
    skiplist_->Delete(key);
    num_entries_--;

    return true;
}

std::future<std::string> Memtable::GetAsync(std::string &key) {
    throw std::runtime_error("Not implemented");
}

std::future<bool> Memtable::PutAsync( std::string& key,  std::string& value) {
    throw std::runtime_error("Not implemented");
}

std::future<bool> Memtable::DeleteAsync(std::string& key) {
    throw std::runtime_error("Not implemented");
}

void Memtable::FlushToL0() {
    if (skiplist_ && skiplist_->GetTotalSize() <= flush_threshold_) {
        return;
    }

    sst_manager_->AddMemtable(skiplist_);
    skiplist_ = new SkipList(max_level_, p_);
    total_size_ = 0;
    num_entries_ = 0;
}

void Memtable::SetSSTManager(SSTManager* sst_manager) {
    sst_manager_ = sst_manager;
}

