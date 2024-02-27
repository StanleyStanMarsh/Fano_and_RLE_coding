#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <fstream>

class FanoCoding {
    std::vector<wchar_t> *alphabet;
    std::unordered_map<wchar_t, float> probabilities;
    std::unordered_map<wchar_t, std::wstring> fano_codes;
private:
    std::vector<wchar_t> AlphabetSortedByProbabilities();
public:
    void FindProbabilities(const std::string& file_name);
    FanoCoding(std::vector<wchar_t> *alpha);
    void MakeFanoCodes(int start, int end);
    void get_probabilities();
    void get_fano_codes();
};
