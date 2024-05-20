#include <string>
#include <future>

class MemtableInterface {
    public:
        virtual std::string Get(const std::string& key) = 0;
        virtual bool Put(const std::string& key, const std::string& value) = 0;
        virtual bool Delete(const std::string& key) = 0;

        virtual std::future<std::string> GetAsync(const std::string& key) = 0;
        virtual std::future<bool> PutAsync(const std::string& key, const std::string& value) = 0;
        virtual std::future<bool> DeleteAsync(const std::string& key) = 0;
};