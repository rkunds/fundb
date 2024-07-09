#include "table/block_based/block_builder.h"
#include "common/utils/generate.h"
#include "common/utils/threadpool.h"
#include "common/structures/skiplist.h"
#include "reader/sst_reader.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <random>

void WriteSST();
void ReadSST();

int main() {
    // SSTBuilder builder = SSTBuilder("1KSST", 4096);
    BlockBuilder builder = BlockBuilder("1KSST", 4096);
    std::vector<std::pair<std::string, std::string>> kv = GenerateRandomKV(1000, 30, 5, 100, 10);

    for (auto& p : kv) {
        builder.AddEntry(p.first, p.second);
    }

    builder.WriteAndClose();

    SSTReader reader = SSTReader("1KSST");
    reader.Open();
    size_t i = 0;
    for (auto it = reader.begin(); it != reader.end(); ++it) {
        auto [k, v] = *it;
        // check if the key-value pair is the same as the one we inserted
        if (kv[i].first != k || kv[i].second != v) {
            std::cout << "key-value pair mismatch" << std::endl;
        }
        i++;
    }

    std::cout << "done" << std::endl;
}
// int main() {
//     SkipList sl = SkipList(5, 0.5);
//     std::vector<std::pair<std::string, std::string>> kv = GenerateRandomKV(1000, 30, 5, 100, 10);

    

//     ThreadPool tp = ThreadPool(8);
//     // each thread will have a vector where we append the time it took to insert each key
//     std::vector<double> times = std::vector<double>(kv.size(), 0.0);

//     for (size_t i = 0; i < kv.size(); i++) {
//         std::string key = kv[i].first;
//         std::string value = kv[i].second;
//         tp.AddTask([key, value, &sl, &times, i] () {
//             auto start = std::chrono::high_resolution_clock::now();
//             sl.Insert(key, value);
//             auto end = std::chrono::high_resolution_clock::now();
//             times[i] = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
//         });
//     }

//     tp.Stop();



//     std::cout << "memtable size: " << sl.GetTotalSize() << " bytes" << std::endl;
//     std::cout << "num entries: " << sl.GetNumEntries() << std::endl;
//     size_t j = 0;
//     for (auto it = sl.begin(); it != sl.end(); ++it) {
//         auto [k, v] = *it;
//         std::cout << j++ << ": " << k << " " << v << std::endl;
//     }
// }

void WriteSST() {
    BlockBuilder builder = BlockBuilder("1KSST", (size_t) 4096);
    std::ofstream textcontents("textcontents.txt");
    std::vector<std::pair<std::string, std::string>> kv = GenerateRandomKV(1000, 30, 5, 100, 10);

    for (auto& p : kv) {
        builder.AddEntry(p.first, p.second);
        textcontents << p.first << " " << p.second << std::endl;
    }

    builder.WriteAndClose();
}

void ReadSST() {
    std::string filename = "/Users/rkunds/code/fundb/build/db_data/BCF5AD16-6D68-45A8-A149-E8EC6C3B4017-6aO3qVjPWc";
    SSTReader reader = SSTReader(filename);


    // create file reader with filename

    std::ifstream r(filename, std::ios::binary);

    size_t num_blocks = reader.GetNumBlocks();
    MetadataBlock meta = reader.GetMetadataBlock();

    std::cout << "num entries: " << meta.GetNumEntries() << std::endl;
    std::cout << "num blocks: " << num_blocks << std::endl;

    size_t num_entries = 0;
    size_t total_entries = 0;

    for (size_t i = 0; i < num_blocks; i++) {
        size_t entries_in_block = 0;
        r.read((char*) &entries_in_block, sizeof(size_t));

        for (size_t j = 0; j < entries_in_block; j++) {
            size_t key_size = 0;
            size_t val_size = 0;    
            std::string key;
            std::string val;

            r.read((char*) &key_size, sizeof(size_t));
            key.resize(key_size);
            r.read(&key[0], key_size);

            r.read((char*) &val_size, sizeof(size_t));
            val.resize(val_size);
            r.read(&val[0], val_size);


            // std::cout << key << " " << val << std::endl;
            total_entries++;
        }
    }
    
    std::cout << "total entries: " << total_entries << std::endl;
}