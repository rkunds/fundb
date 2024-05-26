#include "table/block_based/block_builder.h"
#include "common/utils/generate.h"
#include "common/utils/threadpool.h"
#include "common/structures/skiplist.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <random>

void WriteSST();
void ReadSST();

int main() {
    // ReadSST();
    WriteSST(); 
    ReadSST();
    return 0;
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
    std::ifstream file("1KSST", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return;
    }

    size_t file_size = 0;
    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file_size = file_size - sizeof(size_t);

    file.seekg(file_size, std::ios::beg);

    size_t metadata_offset = 0;
    file.read((char*) &metadata_offset, sizeof(size_t));
    std::cout << "Metadata offset: " << metadata_offset << std::endl;


    size_t index_offset = 0;
    size_t num_blocks = 0;
    size_t num_entries = 0;
    size_t min_key_size = 0;
    size_t max_key_size = 0;
    std::string min_key = "";
    std::string max_key = "";

    file.seekg(metadata_offset, std::ios::beg);
    std::cout << "curr pos: " << file.tellg() << std::endl;
    file.read((char*) &index_offset, sizeof(size_t));
    std::cout << "Index offset: " << index_offset << std::endl;
    file.read((char*) &num_blocks, sizeof(size_t));
    std::cout << "Num blocks: " << num_blocks << std::endl;
    file.read((char*) &num_entries, sizeof(size_t));
    std::cout << "Num entries: " << num_entries << std::endl;
    file.read((char*) &min_key_size, sizeof(size_t));
    std::cout << "Min key size: " << min_key_size << std::endl;
    min_key.resize(min_key_size);
    file.read(&min_key[0], min_key_size);
    std::cout << "Min key: " << min_key << std::endl;
    file.read((char*) &max_key_size, sizeof(size_t));
    std::cout << "Max key size: " << max_key_size << std::endl;
    max_key.resize(max_key_size);
    file.read(&max_key[0], max_key_size);
    std::cout << "Max key: " << max_key << std::endl;

    size_t largest_val = 0;
    size_t largest_key = 0;

    file.read((char*) &largest_val, sizeof(size_t));
    std::cout << "Largest val size: " << largest_val << std::endl;
    file.read((char*) &largest_key, sizeof(size_t));
    std::cout << "Largest key size: " << largest_key << std::endl;

    file.close();
}