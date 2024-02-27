#include <codecvt>
#include "FanoCoding.h"

FanoCoding::FanoCoding(std::vector<wchar_t> *alpha) {
    this->alphabet = alpha;
}

void FanoCoding::FindProbabilities(const std::string& file_name) {
    std::wifstream in;
    in.open(file_name);
    in.imbue( std::locale(std::locale(), new std::codecvt_utf8<wchar_t>) );

    wchar_t sym;
    float file_len = 0;
    while(in >> sym) {
        probabilities[sym] += 1;
        file_len += 1;
    }

    in.close();

    for (auto &elem: probabilities) {
        elem.second = elem.second / file_len;
    }
}

void FanoCoding::get_probabilities() {
    for (auto &elem: probabilities) {
        std::wcout << elem.first << L':' << elem.second;
        std::wcout << L'\n';
    }
}

std::vector<wchar_t> FanoCoding::AlphabetSortedByProbabilities() {
    std::vector<wchar_t> sorted_alphabet;
    for (auto &c: probabilities)
        sorted_alphabet.push_back(c.first);
    std::sort(sorted_alphabet.begin(), sorted_alphabet.end(),
              [this](wchar_t &a, wchar_t &b)->bool{
        return probabilities[a] > probabilities[b];
    });
    return sorted_alphabet;
}