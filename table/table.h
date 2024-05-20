
#pragma once

#include <fstream>
#include <string>

class SSTable {
    public:
        virtual ~SSTable() {}

        virtual bool Open() = 0;

        virtual void AddEntry(std::string& key, std::string& value) = 0;

        virtual bool WriteAndClose() = 0;

        virtual size_t GetFileSize() = 0;

    protected:
        std::string filename;
        std::ifstream file;
        size_t file_size;
};