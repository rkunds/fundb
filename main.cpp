#include "table/sst_builder.h"
#include "common/utils/generate_kv.h"

#include <iostream>
#include <fstream>
#include <vector>

void WriteSST();
void ReadSST();

int main() {
    WriteSST();
    ReadSST();
    return 0;
}

void WriteSST() {
    SSTBuilder builder = SSTBuilder("testsst", (size_t) 4096);
    std::ofstream textcontents("textcontents.txt");
    std::vector<std::pair<std::string, std::string>> kv = GenerateRandomKV(100, 20, 5, 75, 10);

    for (auto& p : kv) {
        builder.AddEntry(p.first, p.second);
        textcontents << p.first << " " << p.second << std::endl;
        // std::cout << "Added entry: " << p.first << " " << p.second << std::endl;
    }

    builder.CompleteSSTFile();
}

void ReadSST() {
    std::ifstream file("testsst", std::ios::binary);
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

    

}