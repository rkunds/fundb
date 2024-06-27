#pragma once

#include <random>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

std::string GenerateRandomString(double length);

std::vector<std::string> GenerateRandomStrings(size_t num_strings, double mean_length, double stddev_length);

std::vector<std::pair<std::string, std::string>> GenerateRandomKV(size_t num_entries, size_t mean_key_length, 
                                                size_t stddev_key_length, size_t mean_value_length, size_t stddev_value_length);

std::string GenerateUUID();