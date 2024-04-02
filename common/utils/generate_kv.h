#pragma once

#include <random>
#include <string>
#include <vector>

std::string GenerateRandomString(size_t length) {
    std::string str;
    str.reserve(length);

    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (size_t i = 0; i < length; i++) {
        str.push_back(alphanum[rand() % (sizeof(alphanum) - 1)]);
    }

    return str;
}

std::vector<std::string> GenerateRandomStrings(size_t num_strings, size_t mean_length, size_t stddev_length) {
    std::vector<std::string> strings;
    strings.reserve(num_strings);

    // std::random_device rd;
    std::mt19937 gen(1234);
    std::normal_distribution<> d(mean_length, stddev_length);

    for (size_t i = 0; i < num_strings; i++) {
        double length = d(gen);
        if (length <= 0) {
            length = 5;
        }
        
        strings.push_back(GenerateRandomString(length));
    }

    return strings;
}

std::vector<std::pair<std::string, std::string>> GenerateRandomKV(size_t num_entries, size_t mean_key_length, size_t stddev_key_length, size_t mean_value_length, size_t stddev_value_length) {
    std::vector<std::pair<std::string, std::string>> kv;
    kv.reserve(num_entries);

    // std::random_device rd;
    std::mt19937 gen(1234);
    std::normal_distribution<> key_d(mean_key_length, stddev_key_length);
    std::normal_distribution<> value_d(mean_value_length, stddev_value_length);

    for (size_t i = 0; i < num_entries; i++) {
        size_t key_length = key_d(gen);
        size_t value_length = value_d(gen);

        kv.push_back({GenerateRandomString(key_length), GenerateRandomString(value_length)});
    }

    // sort by key
    std::sort(kv.begin(), kv.end(), [](const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b) {
        return a.first < b.first;
    });

    // remove duplicates
    kv.erase(std::unique(kv.begin(), kv.end(), [](const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b) {
        return a.first == b.first;
    }), kv.end());

    return kv;
}