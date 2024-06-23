#include <iostream>
#include "memtable/memtable.h"

int main() {
    SSTManager sst_manager = SSTManager();
    Memtable *m =  MemtableBuilder().SetSSTManager(&sst_manager).SetFlushThreshold(100).Build();
    std::unique_ptr<Memtable> m_ptr(m);

    for (int i = 0; i < 100; i++) {
        std::string key = std::to_string(i);
        std::string value = "hehe " + std::to_string(i);
        m_ptr->Put(key, value);
        if (i % 10 == 0) {
            m_ptr->Delete(key);
        }
    }    
}