#include <iostream>
#include <string>
#include <istream>
#include <fstream>

#include "table/block_based/metadata_block.h"
#include "reader/sst_reader.h"


int main() {
    SSTReader reader = SSTReader("1KSST");
    reader.Open();

    auto start = std::chrono::high_resolution_clock::now();
    std::string ret = reader.Get("01vQcwj3SA0jc5xWIiX5kkCoJhYH2Uqvabi");
    auto end = std::chrono::high_resolution_clock::now();

    if (ret.empty()) {
        std::cerr << "Key not found" << std::endl;
    } else {
        std::cout << "Value: " << ret << std::endl;
    }

    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns" << std::endl;

} 
 