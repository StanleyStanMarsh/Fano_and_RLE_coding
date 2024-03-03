#include "RLECoding.h"

bool RLECoding::Encode(const std::string &file_name) {
    std::wifstream in;
    in.imbue( std::locale(std::locale(), new std::codecvt_utf8<wchar_t>) );
    in >> std::noskipws;
    in.open(file_name, std::ios::in | std::ios::binary);

    if (!in) {
        in.close();
        return false;
    }

    wchar_t sym;
    std::wstring file_code;
    while (in >> sym) {
        file_code += sym;
    }

    int count;

    for (int i = 0; i < file_code.length(); i++)
    {
        // count occurrences of character at index `i`
        count = 1;
        while (file_code[i] == file_code[i + 1] && i < file_code.length() - 1) {
            count++, i++;
        }

        // append current character and its count to the result
        code += std::to_wstring(count) + file_code[i];
        code_vector.emplace_back(count, file_code[i]);
    }
    return true;
}

std::wstring RLECoding::get_code() {
    return code;
}
