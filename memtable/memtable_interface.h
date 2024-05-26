#pragma once

#include <string>
#include <future>

class MemtableInterface {
    public:
        virtual std::string Get(std::string& key) = 0;
        virtual bool Put(std::string& key, std::string& value) = 0;
        virtual bool Delete(std::string& key) = 0;

        virtual std::future<std::string> GetAsync(std::string& key) = 0;
        virtual std::future<bool> PutAsync(std::string& key, std::string& value) = 0;
        virtual std::future<bool> DeleteAsync(std::string& key) = 0;
};