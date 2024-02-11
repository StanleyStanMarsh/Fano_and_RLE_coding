#include <fstream>
#include <string>
#include <vector>

class FileGenerator {
    std::ofstream out;
public:
    FileGenerator(std::string file_name);
    void Generate(std::vector<wchar_t> alpha);
};
