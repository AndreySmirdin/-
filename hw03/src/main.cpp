#include <iostream>
#include <cstring>
#include "huffman.h"

int main(int argc, char** argv){

    if (argc != 6){
        std::cout << "Wrong amount of arguments!";
        return 1;
    }

    std::string in, out;

    if ((std::string(argv[2]) == "-f" || std::string(argv[2]) == "--file") &&
        (std::string(argv[4]) == "-o" || std::string(argv[4]) == "--output")) {
        in = argv[3];
        out = argv[5];
    }
    else if ((std::string(argv[4]) == "-f" || std::string(argv[4]) == "--file") &&
            (std::string(argv[2]) == "-o" || std::string(argv[2]) == "--output")) {
        in = argv[5];
        out = argv[3];
    }
    else {
        std::cout << "Wrong input/output information";
        return 1;
    }

    FILE* f = fopen(in.c_str(), "r");
    if (f == NULL){
        std::cout << "File doesnt exists!";
        return 0;
    }
    fclose(f);

    std::ifstream ifs = std::ifstream(in, std::ifstream::binary);

    if (std::string(argv[1]) == "-c") {
        std::ofstream ofs = std::ofstream(out, std::ifstream::binary);
        HuffmanEncoder encoder(&ifs, &ofs);
        encoder.encode();
    } else if (std::string(argv[1]) == "-u") {
        std::ofstream ofs = std::ofstream(out);
        HuffmanDecoder decoder(&ifs, &ofs);
        decoder.decode();
    } else {
        std::cout << "Wrong argument #1";
        return 1;
    }

    return 0;
}