#include <iostream>
#include "compaction/sst_manager.h"
#include "memtable/memtable.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    MemtableBuilder builder;
    SSTManager sst_manager = SSTManager();
    Memtable* memtable = builder.SetFlushThreshold(100000).SetSSTManager(&sst_manager).Build();

    while (true) {
        auto key = GenerateRandomString(10);
        auto value = GenerateRandomString(100);
        memtable->Put(key, value);
    }   

    delete memtable;

    return 0;

}   

// int main() {
//     SkipList * sl = new SkipList(5, 0.5);

//     for (size_t i = 0; i < 1000; i++) {
//         std::string key = GenerateRandomString(10);
//         std::string value = GenerateRandomString(100);
//         sl->Insert(key, value);
//     }

//     delete sl;

//     return 0;
// }