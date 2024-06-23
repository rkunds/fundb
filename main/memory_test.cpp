#include <iostream>
#include "compaction/sst_manager.h"
#include "memtable/memtable.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    MemtableBuilder builder;
    SSTManager sst_manager = SSTManager();
    Memtable* memtable = builder.SetFlushThreshold(64 * 1024 * 1024).SetSSTManager(&sst_manager).Build();
    size_t N = 100000000;

    ThreadPool pool(4);

    for (size_t i = 0; i < 4; i++) {
        pool.AddTask([N, i, memtable] {
            std::cout << "Inserting " << N << " entries" << std::endl;
            std::string f_name = "test" + std::to_string(i) + ".txt";
            std::ifstream file(f_name);

            while (true) {
                std::string key, value;
                if (!std::getline(file, key)) {
                    break;
                }
                std::getline(file, value);
                memtable->Put(key, value);
            }
        });
    }  

    std::this_thread::sleep_for(std::chrono::seconds(5));

    size_t prev = 0;
    while (true) {
        if (prev == memtable->GetNumEntries()) {
            break;
        }
        prev = memtable->GetNumEntries();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}   