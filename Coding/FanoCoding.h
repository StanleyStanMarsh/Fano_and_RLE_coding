#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <float.h>
#include <bitset>
#include <fstream>

class FanoCoding {
    std::vector<wchar_t> *alphabet;
    std::unordered_map<wchar_t, double> probabilities;
    std::unordered_map<wchar_t, std::wstring> fano_codes;
private:
    std::vector<wchar_t> AlphabetSortedByProbabilities();
    int FindAlmostEqualSubvectors(int begin, int end);
    void FindProbabilities(std::wifstream &in_stream);
    void MakeFanoCodes(int start, int end);
public:
    FanoCoding(std::vector<wchar_t> *alpha);
    bool Encode(const std::string &file_name);
    void get_probabilities();
    void get_fano_codes();
};
