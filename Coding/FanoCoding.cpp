#include <codecvt>
#include "FanoCoding.h"

FanoCoding::FanoCoding(std::vector<wchar_t> *alpha) {
    this->alphabet = alpha;
}

void FanoCoding::FindProbabilities(std::wifstream &in_stream) {
    in_stream.imbue( std::locale(std::locale(), new std::codecvt_utf8<wchar_t>) );

    wchar_t sym;
    double file_len = 0;
    while(in_stream >> sym) {
        probabilities[sym] += 1;
        file_len += 1;
    }

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

bool FanoCoding::Encode(const std::string &file_name) {
    std::wifstream in;
    in.imbue( std::locale(std::locale(), new std::codecvt_utf8<wchar_t>) );
    in >> std::noskipws;
    in.open(file_name, std::ios::in | std::ios::binary);

    if (!in) {
        in.close();
        return false;
    }

    FindProbabilities(in);
    MakeFanoCodes(0, alphabet->size());
    in.close();

    std::ofstream out;
    out.open(file_name + ".compressed", std::ios::in | std::ios::binary);
    in.open(file_name, std::ios::in | std::ios::binary);
    in >> std::noskipws;

    if (!out) {
        out.close();
        in.close();
        return false;
    }

    wchar_t sym;
    char buffer = 0;
    int bit = 1;
    while(in >> sym) {
        for (wchar_t b : fano_codes[sym]) {
            if (b == '1') buffer |= 0b00000001;
            if (bit == CHAR_BIT) {
                out << buffer;
                buffer = 0;
                bit = 1;
            }
            else {
                buffer <<= 1;
                bit++;
            }
        }
    }
    if (bit != 0) {
        for (; bit <= CHAR_BIT; bit++) {
            buffer <<= 1;
        }
        out << buffer;
    }

    out.close();
    in.close();
    return true;
}

bool FanoCoding::Decode(const std::string &file_name) {
    if (!file_name.ends_with(".compressed")) return false;

    std::ifstream in;
    in.open(file_name, std::ios::in | std::ios::binary);

    in.imbue( std::locale(std::locale(), new std::codecvt_utf8<wchar_t>) );

    if (!in) {
        in.close();
        return false;
    }

    char sym;
    std::wstring file_code;
    in >> std::noskipws;
    while(in >> sym) {
        std::bitset<8> bits(sym);
        std::string sym_code = bits.to_string();
        for (auto c: sym_code) {
            if (c == '1') file_code += L"1";
            else file_code += L"0";
        }
    }
    in.close();

    std::wstring decoded_msg;
    bool found = true;
    while (found) {
        for (auto code: fano_codes) {
            found = false;
            if (file_code.starts_with(code.second)) {
                file_code = file_code.substr(code.second.size());
                decoded_msg += code.first;
                found = true;
                break;
            }
        }
    }

    std::wofstream out;
    std::string new_file_name = file_name;
    new_file_name.erase(file_name.rfind(".compressed"), 11);
    out.open("decompressed_" + new_file_name, std::ios::in | std::ios::binary);
    out.imbue( std::locale(std::locale(), new std::codecvt_utf8<wchar_t>) );

    if (!out) {
        out.close();
        return false;
    }
    out << decoded_msg;

    out.close();
    return true;
}
