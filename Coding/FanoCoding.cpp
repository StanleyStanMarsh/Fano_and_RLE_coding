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
    double file_len = 0;
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

    std::vector<wchar_t> sorted_alpha(AlphabetSortedByProbabilities());
    int sep = FindAlmostEqualSubvectors(start, end);

    for(int i = start; i < sep+1; i++) {
        fano_codes[sorted_alpha[i]] += L"1";
    }
    for(int i = sep+1; i < end+1; i++) {
        fano_codes[sorted_alpha[i]] += L"0";
    }

    MakeFanoCodes(start, sep);
    MakeFanoCodes(sep + 1, end);
}

void FanoCoding::get_fano_codes() {
    std::vector<wchar_t> sorted_alpha(AlphabetSortedByProbabilities());
    for (auto sym: sorted_alpha) {
        std::wcout << sym << L": " << fano_codes[sym] << L' ' << probabilities[sym] * 10000 << L'\n';
    }
//    for (auto &elem: fano_codes) {
//        std::wcout << elem.first << L':' << elem.second;
//        std::wcout << L'\n';
//    }
}

int FanoCoding::FindAlmostEqualSubvectors(int begin, int end) {
    std::vector<wchar_t> sorted_alpha(AlphabetSortedByProbabilities());
    int l = begin;
    int r = end;
    double left_sum = probabilities[sorted_alpha[l]];
    double right_sum = probabilities[sorted_alpha[r]];
    while(l < r) {
        if (std::abs(left_sum - right_sum) == DBL_MIN)
            return l;
        else if (left_sum < right_sum)
            left_sum += probabilities[sorted_alpha[++l]];
        else
            right_sum += probabilities[sorted_alpha[--r]];
    }
    if (l == r) right_sum -= probabilities[sorted_alpha[l]];
    if (begin + 1 != end) {
        double d1 = std::abs(left_sum - right_sum);
        double d2 = std::abs(left_sum - probabilities[sorted_alpha[l]] - (right_sum + probabilities[sorted_alpha[l]]));
        if (d2 < d1)
            return l - 1;
    }
    return l;
}
