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

void FanoCoding::MakeFanoCodes(int start, int end) {
    if (start >= end) return;

    double sum = 0;
    std::vector<wchar_t> sorted_alpha(AlphabetSortedByProbabilities());
    for(int i = start; i <= end; i++) {
        sum += probabilities[sorted_alpha[i]];
    }

    double tmp_sum = 0;
    int sep = start;
    for(int i = start; i <= end; i++) {
        if(tmp_sum + probabilities[sorted_alpha[i]] < sum / 2) {
            tmp_sum += probabilities[sorted_alpha[i]];
            fano_codes[sorted_alpha[i]] += L"0";
        } else {
            fano_codes[sorted_alpha[i]] += L"1";
            if(sep == start) sep = i;
        }
    }

    MakeFanoCodes(start, sep);
    MakeFanoCodes(sep + 1, end);
}

void FanoCoding::get_fano_codes() {
    for (auto &elem: fano_codes) {
        std::wcout << elem.first << L':' << elem.second;
        std::wcout << L'\n';
    }
}
