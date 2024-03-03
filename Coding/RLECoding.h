#pragma once
#include <iostream>
#include <algorithm>
#include <codecvt>
#include <string>
#include <vector>
#include <fstream>

class RLECoding {
    std::wstring code;
    std::vector<std::pair<int, wchar_t>> code_vector;
public:
    RLECoding() { code = L""; }
    bool Encode(const std::string &file_name);
    std::wstring get_code();
};
