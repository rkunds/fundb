#include "generate.h"

std::string GenerateRandomString(double length) {
    std::string str;

    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    if (length <= 0 || length > 500) {
        std::cout << "hit zero" << std::endl;
        length = 10;
    }

    str.reserve(length);


    for (size_t i = 0; i < length; i++) {

        str.push_back(alphanum[rand() % (sizeof(alphanum) - 1)]);
    }

    return str;
}

std::vector<std::string> GenerateRandomStrings(size_t num_strings, double mean_length, double stddev_length) {
    std::vector<std::string> strings;
    strings.reserve(num_strings);

    std::mt19937 gen(1234);
    std::normal_distribution<> d(mean_length, stddev_length);

    for (size_t i = 0; i < num_strings; i++) {
        double length = d(gen);
        if (length <= 0 || length > 200) {
            length = 5;
        }
        
        strings.push_back(GenerateRandomString(length));
    }

    return strings;
}

std::vector<std::pair<std::string, std::string>> GenerateRandomKV(size_t num_entries, size_t mean_key_length, size_t stddev_key_length, size_t mean_value_length, size_t stddev_value_length) {
    std::vector<std::pair<std::string, std::string>> kv;
    kv.reserve(num_entries);

    std::mt19937 gen(1234);
    std::normal_distribution<> key_d(mean_key_length, stddev_key_length);
    std::normal_distribution<> value_d(mean_value_length, stddev_value_length);

    for (size_t i = 0; i < num_entries; i++) {
        double key_length = key_d(gen);
        double value_length = value_d(gen);

        if (key_length <= 0) {
            key_length = 5;
        }

        if (value_length <= 0) {
            value_length = 5;
        }

        kv.push_back({GenerateRandomString(key_length), GenerateRandomString(value_length)});
    }

    std::sort(kv.begin(), kv.end(), [](const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b) {
        return a.first < b.first;
    });

    kv.erase(std::unique(kv.begin(), kv.end(), [](const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b) {
        return a.first == b.first;
    }), kv.end());

    return kv;
}

std::vector<std::string> GenNStrings(size_t num_strings, size_t length) {
    std::vector<std::string> strings;
    strings.reserve(num_strings);

    for (size_t i = 0; i < num_strings; i++) {
        strings.push_back(GenerateRandomString(length));
    }

    return strings;
}

std::string GenerateUUID() {
    uuid_t uuid;
    uuid_generate_random(uuid);
    char uuid_str[37];
    uuid_unparse(uuid, uuid_str);
    return std::string(uuid_str);
}

