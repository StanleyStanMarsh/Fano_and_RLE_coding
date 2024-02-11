#include <iostream>
#include "File Generation/FileGenerator.h"
#include "Misc/alphabet.h"

int main() {
    FileGenerator *gen = new FileGenerator("test1.txt", 10000);
    gen->Generate(alphabet);
    delete gen;

    return 0;
}
