#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>

class FanoCoding {
    std::vector<wchar_t> *alphabet;
    std::unordered_map<wchar_t, float> probabilities;
private:

    std::vector<wchar_t> AlphabetSortedByProbabilities();
public:
    void FindProbabilities(const std::string& file_name);
    FanoCoding(std::vector<wchar_t> *alpha);
    void FanoCode(const std::string& file_name);
    void get_probabilities();
};
