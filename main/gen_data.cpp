#include "common/utils/generate.h"
#include <fstream>

int main() {
    size_t num_files = 4;
    size_t target_size = 1024 * 1024 * 1024;

    for (size_t i = 0; i < num_files; i++) {
        std::string filename = "test" + std::to_string(i) + ".txt";
        std::ofstream file(filename);
        size_t size = 0;
        while (size < target_size) {
            std::string key = GenerateRandomString(30);
            std::string value = GenerateRandomString(100);
            // file << key << " " << value << std::endl;
            file << key << std::endl;
            file << value << std::endl;
            size += key.size() + value.size() + 1;
        }
        file.close();
    }
}